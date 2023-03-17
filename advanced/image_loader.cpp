#include "libyuv.h"

#include "image_loader.h"

#include <cmath>

#include "bytertc/video/common/rx_video_frame_check.h"
#include "libyuv.h"
#include "realx/base/rx_checks.h"
#include "realx/base/rx_logging.h"
#include "realx/base/video/rx_video_frame.h"

int main(){
#ifdef _WIN32
    std::wstring wfilename;
    int len = MultiByteToWideChar(CP_UTF8, 0, file_path_.c_str(), file_path_.length(), NULL, 0);
    if (len > 0) {
        wfilename.resize(len);
        MultiByteToWideChar(CP_UTF8, 0, file_path_.c_str(), file_path_.length(), &wfilename[0], len);
    }
    FILE* fp = _wfopen(wfilename.c_str(), L"rb");

#else
    FILE* fp = fopen(file_path_.c_str(), "rb");
#endif

    if (!fp) {
        LInfo(kMediaLogTag) << "open file failed";
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    auto size = ftell(fp);
    rewind(fp);
    auto* buf = (uint8_t*)malloc(size);
    fread(buf, 1, size, fp);
    fclose(fp);
    size_t image_len = 0;
    uint8_t* image_buf = NULL;
    int image_decoded_width = 0;
    int image_decoded_height = 0;
    if (image_loader_->decode_with_data((const char*)buf, size, &image_decoded_width, &image_decoded_height, &image_buf, &image_len) < 0) {
        if (image_buf) {
            delete[] image_buf;
            image_buf = nullptr;
        }
        free(buf);
        return -1;
    }
    free(buf);

    image_file_raw_buffer_ = RXVideoFrame::create_frame_with_type(image_decoded_width, image_decoded_height, capture_settings_.format,
                                                                   realx::RXMemoryType::kByteMemory, memory_pool_.get());
    if (image_file_raw_buffer_ == nullptr || image_file_raw_buffer_->alloc_frame_memory(realx::RXMemoryType::kByteMemory, memory_pool_.get())){
        if (image_buf) {
            delete[] image_buf;
            image_buf = nullptr;
        }
        return -1;
    }
    if (capture_settings_.format == realx::RXPixelFormat::kI420) {
        uint8_t* src_y_buf = image_buf;
        uint8_t* src_u_buf = image_buf + image_decoded_width * image_decoded_height;
        uint8_t* src_v_buf = image_buf + image_decoded_width * image_decoded_height * 5 / 4;
        libyuv::I420Copy(src_y_buf, image_decoded_width, src_u_buf, image_decoded_width / 2, src_v_buf, image_decoded_width / 2, image_file_raw_buffer_->data(0), image_file_raw_buffer_->stride(0), image_file_raw_buffer_->data(1), image_file_raw_buffer_->stride(1), image_file_raw_buffer_->data(2), image_file_raw_buffer_->stride(2), image_decoded_width, image_decoded_height);
    } else if (capture_settings_.format == realx::RXPixelFormat::kRgba) {
        memcpy(image_file_raw_buffer_->data(0), image_buf, image_len);
    }

    if (image_buf) {
        delete[] image_buf;
        image_buf = NULL;
    }
    return 0;
}



namespace realx {

RXImageLoader::RXImageLoader(RXImageLoadConfig config) : config_(config), init_status_(init_decode(config)) {
}

RXImageLoader::~RXImageLoader() {
    release();
}

RXImageLoadDecodeId RXImageLoader::get_image_type(const char* file_path) {
    RXImageLoadDecodeId image_type = RXImageLoadDecodeId::kImageDecodeInvalid;
    if(file_path == nullptr) {
        RX_LOG(kRXLSError) << "invalid file path!";
        return image_type;
    }
    std::string strPath = file_path;
#ifdef _WIN32
    std::wstring wfilename;
    int len = MultiByteToWideChar(CP_UTF8, 0, strPath.c_str(), strPath.length(), NULL, 0);
    if (len > 0) {
        wfilename.resize(len);
        MultiByteToWideChar(CP_UTF8, 0, strPath.c_str(), strPath.length(), &wfilename[0], len);
    }
    FILE* fp = _wfopen(wfilename.c_str(), L"rb");

#else
    FILE* fp = fopen(strPath.c_str(), "rb");
#endif

    if (!fp) {
        RX_LOG(kRXLSError) << "open file failed!";
        return image_type;
    }
    fclose(fp);
    std::string strExt;
    auto iter = std::find(strPath.rbegin(), strPath.rend(), '.');
    if (iter == strPath.rend()) {
        image_type = RXImageLoadDecodeId::kImageDecodeInvalid;
    } else {
        strExt = strPath.substr(strPath.rend() - iter);
        for (char& c : strExt) {
            c = tolower(c);
        }
        if (strExt == "jpg" || strExt == "jpeg") {
            image_type = RXImageLoadDecodeId::kImageDecodeJPG;
        } else if (strExt == "png"){
            image_type = RXImageLoadDecodeId::kImageDecodePNG;
        } else if (strExt == "bmp"){
            image_type = RXImageLoadDecodeId::kImageDecodeBMP;
        } else if (strExt == "heif" || strExt == "heic"){
            image_type = RXImageLoadDecodeId::kImageDecodeInvalid;
        }
    }
    return image_type;
}

int RXImageLoader::decode_with_data(const char* src_data, size_t src_len, int* width, int* height, uint8_t** dst_data,
                                    size_t* dst_len, uint8_t background_y, uint8_t background_u, uint8_t background_v) {
    if (init_status_ == -1) {
        RX_LOG(kRXLSError) << "init decoder error!";
        return -1;
    }

    uint8_t* buf;
    bool need_free = false;
    int size = 0;

    switch (config_.data_type_) {
        case RXImageLoadDataType::kImageDataBase64:
            buf = reinterpret_cast<uint8_t*>(malloc(src_len));
            need_free = true;
            size = av_base64_decode(buf, src_data, src_len);
            if (size <= 0) {
                free(buf);
                return -1;
            }
            break;
        case RXImageLoadDataType::kImageDataBinary:
            buf = reinterpret_cast<uint8_t*>(const_cast<char*>(src_data));
            need_free = false;
            size = src_len;
            break;
        default:
            RX_LOG(kRXLSWarning) << "decode_with_data with invalid data type: " << static_cast<int>(config_.data_type_);
            return -1;
    }

    packet_.data = buf;
    packet_.size = size;

    int got_picture = 0;

    while (packet_.size > 0) {
        int dec_len = avcodec_decode_video2(codec_ctx_, frame_, &got_picture, &packet_);
        if (dec_len < 0) {
            RX_LOG(kRXLSError) << "error while decoding frame";
            if (need_free) {
                free(buf);
            }
            return -1;
        }

        if (got_picture) {
            RX_DLOG(kRXLSInfo) << "got a yuv frame";

            if (need_free)
                free(buf);

            if (dst_width_) {
                *width = dst_width_;
            } else {
                *width = codec_ctx_->width;
            }

            if (dst_height_) {
                *height = dst_height_;
            } else {
                *height = codec_ctx_->height;
            }

            bool aspect_ratio_changed = (codec_ctx_->width * (*height)) != (codec_ctx_->height * (*width));
            bool force_no_crop = false;
            if (codec_ctx_->width % 2 || codec_ctx_->height % 2) {
                force_no_crop = true;
                RX_LOG(kRXLSInfo) << "origin resolution " << codec_ctx_->width << " * " << codec_ctx_->height
                                  << ", pixfmt " << codec_ctx_->pix_fmt;
            }

            // CropAndFill
            if ((!force_no_crop) && aspect_ratio_changed && config_.keep_aspect_ratio_) {
                RX_DLOG(kRXLSInfo) << "aspect ratio changed, and keep aspect ratio flag set."
                                   << "origin res:" << codec_ctx_->width << "*" << codec_ctx_->height
                                   << ",target res:" << (*width) << "*" << (*height);

                img_convert_ctx_ = sws_getContext(
                        codec_ctx_->width, codec_ctx_->height, codec_ctx_->pix_fmt, codec_ctx_->width,
                        codec_ctx_->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
                uint8_t* out_buffer =
                        reinterpret_cast<uint8_t*>(av_malloc(avpicture_get_size(
                                AV_PIX_FMT_YUV420P, codec_ctx_->width, codec_ctx_->height)));
                avpicture_fill(reinterpret_cast<AVPicture*>(frame_yuv_), out_buffer,
                                                   AV_PIX_FMT_YUV420P, codec_ctx_->width, codec_ctx_->height);
                sws_scale(img_convert_ctx_, (const uint8_t* const*)frame_->data, frame_->linesize, 0,
                                            codec_ctx_->height, frame_yuv_->data, frame_yuv_->linesize);
                // i420buffer --> CropAndFill
                std::unique_ptr<RXVideoFrame> dst_buffer =
                        RXVideoFrame::create_i420_frame(codec_ctx_->width, codec_ctx_->height, nullptr);
                if (rx_video_frame_check("image_loader", dst_buffer, true) != kRXCodecErrorNone) {
                    RX_LOG(kRXLSError) << "fail to create i420 frame";
                    return -1;
                }

                unsigned char* y_buf = frame_yuv_->data[0];
                unsigned char* u_buf = frame_yuv_->data[1];
                unsigned char* v_buf = frame_yuv_->data[2];
                libyuv::I420Copy(y_buf, codec_ctx_->width, u_buf, codec_ctx_->width / 2, v_buf, codec_ctx_->width / 2,
                                 dst_buffer->data(0), dst_buffer->stride(0), dst_buffer->data(1), dst_buffer->stride(1),
                                 dst_buffer->data(2), dst_buffer->stride(2), codec_ctx_->width, codec_ctx_->height);

                dst_buffer = dst_buffer->crop_and_scale((*width), (*height), RXVideoScaleMode::kFitWithFilling);
                if (!dst_buffer) {
                    RX_LOG(kRXLSError) << "fail to crop and scale frame";
                    return -1;
                }

                if (fmt_ == AV_PIX_FMT_YUV420P) {  // target is i420
                    RX_LOG(kRXLSDebug) << "Target is i420";
                    *dst_len = (*width) * (*height) * 3 / 2;
                    *dst_data = new uint8_t[*dst_len]();
                    uint8_t* dst_y_buf = *dst_data;
                    uint8_t* dst_u_buf = *dst_data + (*width) * (*height);
                    uint8_t* dst_v_buf = *dst_data + (*width) * (*height) * 5 / 4;
                    libyuv::I420Copy(dst_buffer->data(0), dst_buffer->stride(0), dst_buffer->data(1),
                                     dst_buffer->stride(1), dst_buffer->data(2), dst_buffer->stride(2), dst_y_buf,
                                     (*width), dst_u_buf, (*width) / 2, dst_v_buf, (*width) / 2, (*width), (*height));

                    av_free(out_buffer);
                    sws_freeContext(img_convert_ctx_);
                    return 0;
                } else if (fmt_ == AV_PIX_FMT_RGBA) {  // target is rgba
                    RX_LOG(kRXLSDebug) << "Target is rgba";
                    // convert i420 to rgba
                    (*dst_len) = (*width) * (*height) * 4;
                    *dst_data = new uint8_t[*dst_len]();
                    libyuv::I420ToABGR(dst_buffer->data(0), dst_buffer->stride(0), dst_buffer->data(1),
                                       dst_buffer->stride(1), dst_buffer->data(2), dst_buffer->stride(2), *dst_data,
                                       (*width) * 4, (*width), (*height));
                    av_free(out_buffer);
                    sws_freeContext(img_convert_ctx_);
                    return 0;
                }

            } else {
                RX_DLOG(kRXLSInfo) << "aspect ratio not changed, or keep aspect ratio flag set false."
                                   << "origin res:" << codec_ctx_->width << "*" << codec_ctx_->height
                                   << ",target res:" << (*width) << "*" << (*height);
                // already keep aspec ratio or we don't care,just scale it.
                img_convert_ctx_ =
                        sws_getContext(codec_ctx_->width, codec_ctx_->height, codec_ctx_->pix_fmt,
                                                         (*width), (*height), fmt_, SWS_BICUBIC, NULL, NULL, NULL);

                uint8_t* out_buffer = reinterpret_cast<uint8_t*>(
                        av_malloc(avpicture_get_size(fmt_, (*width), (*height))));

                avpicture_fill(reinterpret_cast<AVPicture*>(frame_yuv_), out_buffer, fmt_, (*width),
                                                   (*height));

                sws_scale(img_convert_ctx_, (const uint8_t* const*)frame_->data, frame_->linesize, 0,
                                            codec_ctx_->height, frame_yuv_->data, frame_yuv_->linesize);
                switch (fmt_) {
                    case AV_PIX_FMT_YUV420P: {
                        RX_LOG(kRXLSDebug) << "Target is i420";
                        int scale_width = (*width);
                        int scale_height = (*height);
                        int scale_width_half = scale_width >> 1;
                        int scale_height_half = scale_height >> 1;

                        unsigned char* y_buf = frame_yuv_->data[0];
                        unsigned char* u_buf = frame_yuv_->data[1];
                        unsigned char* v_buf = frame_yuv_->data[2];

                        int dst_len_scale = scale_width * scale_height * 3 / 2;
                        uint8_t* dst_data_scale = new uint8_t[dst_len_scale]();

                        // save y
                        memcpy(dst_data_scale, y_buf, scale_width * scale_height);
                        // save u
                        memcpy(dst_data_scale + scale_width * scale_height, u_buf,
                               scale_width_half * scale_height_half);
                        // save v
                        memcpy(dst_data_scale + scale_width * scale_height * 5 / 4, v_buf,
                               scale_width_half * scale_height_half);

                        // circle-lized
                        if (config_.circled_) {
                            RX_LOG(kRXLSDebug) << "Target set circled";
                            int radius = scale_width >> 1;

                            for (int j = 0; j < scale_height; j++) {
                                for (int i = 0; i < scale_width; i++) {
                                    if (radius * radius <
                                        (abs(radius - i) * abs(radius - i)) + (abs(radius - j) * abs(radius - j))) {
                                        dst_data_scale[i + scale_width * j] = background_y;
                                    }
                                }
                            }

                            radius >>= 1;
                            for (int j = 0; j < scale_height_half; j++) {
                                for (int i = 0; i < scale_width_half; i++) {
                                    if (radius * radius <
                                        (abs(radius - i) * abs(radius - i)) + (abs(radius - j) * abs(radius - j))) {
                                        dst_data_scale[scale_width * scale_height + i + scale_width_half * j] =
                                                background_u;
                                    }
                                }
                            }

                            for (int j = 0; j < scale_height_half; j++) {
                                for (int i = 0; i < scale_width_half; i++) {
                                    if (radius * radius <
                                        (abs(radius - i) * abs(radius - i)) + (abs(radius - j) * abs(radius - j))) {
                                        dst_data_scale[scale_width * scale_height * 5 / 4 + i + scale_width_half * j] =
                                                background_v;
                                    }
                                }
                            }

                            *dst_len = (*width) * (*height) * 3 / 2;
                            *dst_data = new uint8_t[*dst_len]();

                            const uint8_t* src_y_buf = dst_data_scale;
                            const uint8_t* src_u_buf = dst_data_scale + scale_width * scale_height;
                            const uint8_t* src_v_buf = dst_data_scale + scale_width * scale_height * 5 / 4;

                            uint8_t* dst_y_buf = *dst_data;
                            uint8_t* dst_u_buf = *dst_data + (*width) * (*height);
                            uint8_t* dst_v_buf = *dst_data + (*width) * (*height) * 5 / 4;

                            libyuv::I420Scale(src_y_buf, scale_width, src_u_buf, scale_width_half, src_v_buf,
                                              scale_width_half, scale_width, scale_height, dst_y_buf, (*width),
                                              dst_u_buf, (*width) / 2, dst_v_buf, (*width) / 2, (*width), (*height),
                                              libyuv::kFilterBox);

                            delete[] dst_data_scale;

                        } else {
                            *dst_len = dst_len_scale;
                            *dst_data = dst_data_scale;
                        }
                        av_free(out_buffer);
                        sws_freeContext(img_convert_ctx_);
                        return 0;
                    }
                    case AV_PIX_FMT_RGBA: {
                        RX_LOG(kRXLSDebug) << "target is rgba";
                        *dst_len = (*width) * (*height) * 4;
                        *dst_data = new uint8_t[*dst_len]();

                        for (int i = 0; i < (*height); i++) {
                            memcpy(*dst_data + (i * (*width) * 4), frame_yuv_->data[0] + i * frame_yuv_->linesize[0],
                                   (*width) * 4);
                        }
                        av_free(out_buffer);
                        sws_freeContext(img_convert_ctx_);
                        return 0;
                    }
                    default:
                        RX_LOG(kRXLSDebug) << "unsupport pixel format.";
                        av_free(out_buffer);
                        sws_freeContext(img_convert_ctx_);
                        return -1;
                }
            }
        }
        packet_.data += dec_len;
        packet_.size -= dec_len;
    }

    return -1;
}

int RXImageLoader::init_decode(RXImageLoadConfig config) {

    if (config.width_)
        dst_width_ = config.width_;
    if (config.height_)
        dst_height_ = config.height_;

    switch (config.wants_type_) {
        case RXImageLoadWantsType::kImageWantsRGBA:
            fmt_ = AV_PIX_FMT_RGBA;
            break;
        case RXImageLoadWantsType::kImageWantsI420:
            fmt_ = AV_PIX_FMT_YUV420P;
            break;
        default:
            fmt_ = AV_PIX_FMT_YUV420P;
    }

    av_register_all();

    AVCodecID codec_id = AV_CODEC_ID_NONE;

    if (config.codec_id_ == RXImageLoadDecodeId::kImageDecodePNG) {
        codec_id = AV_CODEC_ID_PNG;
    } else if (config.codec_id_ == RXImageLoadDecodeId::kImageDecodeJPG) {
        codec_id = AV_CODEC_ID_MJPEG;
    } else if (config.codec_id_ == RXImageLoadDecodeId::kImageDecodeBMP){
        codec_id = AV_CODEC_ID_BMP;
    }else {
        codec_id = AV_CODEC_ID_GIF;
    }

    codec_ = avcodec_find_decoder(codec_id);
    if (codec_ == NULL) {
        RX_LOG(kRXLSError) << "find decoder error: " << static_cast<int>(config.codec_id_);
        return -1;
    }

    codec_ctx_ = avcodec_alloc_context3(codec_);
    if (!codec_ctx_) {
        RX_LOG(kRXLSWarning) << "can not alloc context";
        return -1;
    }

    if (avcodec_open2(codec_ctx_, codec_, NULL) < 0) {
        RX_LOG(kRXLSWarning) << "cannot open software codec!";
        return -1;
    }

    av_init_packet(&packet_);

    frame_ = av_frame_alloc();
    if (frame_ == NULL) {
        RX_LOG(kRXLSWarning) << "av_frame_alloc ERROR";
        return -1;
    }

    frame_yuv_ = av_frame_alloc();
    if (frame_yuv_ == NULL) {
        RX_LOG(kRXLSWarning) << "av_frame_alloc ERROR";
        return -1;
    }

    return 0;
}

void RXImageLoader::release() {

    init_status_ = -1;

    av_frame_free(&frame_);
    av_frame_free(&frame_yuv_);
    avcodec_close(codec_ctx_);
    avcodec_free_context(&codec_ctx_);
}

}  // namespace realx
