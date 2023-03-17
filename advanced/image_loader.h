#pragma once

#include <cstdlib>
#include <cstring>
#include <memory>

#include "realx/base/video/ffmpeg/rx_ffmpeg_wrapper.h"
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/base64.h"
#include "libswscale/swscale.h"
}

namespace realx {

enum class RXImageLoadDataType {
    kImageDataBase64,
    kImageDataBinary,
};

enum class RXImageLoadDecodeId {
    kImageDecodePNG,
    kImageDecodeJPG,
    kImageDecodeGIF,
    kImageDecodeBMP,
    kImageDecodeInvalid,
};

enum class RXImageLoadWantsType {
    kImageWantsI420,
    kImageWantsRGBA,
};

struct RXImageLoadConfig {
    RXImageLoadDataType data_type_;
    RXImageLoadDecodeId codec_id_;
    RXImageLoadWantsType wants_type_;

    int width_ = 0;
    int height_ = 0;
    int circled_ = 0;
    int keep_aspect_ratio_ = 0;

    RXImageLoadConfig() {
    }
};

class RXImageLoader {
public:
    explicit RXImageLoader(RXImageLoadConfig config);

    ~RXImageLoader();

    static RXImageLoadDecodeId get_image_type(const char* file_path);

    /**
    * @brief decode images from binary data
    *
    * @param src_data : raw binary data
    * @param src_len : the length of raw binary data
    $ @param width : the width of decoding image
    * @param height : the height of decoding image
    * @param das_data : the decoded picture memory
    * @param dst_len : the length of decoded picture memory
    * @param y_bg_value : Y - channel background color
    * @param u_bg_value : U - channel background color
    * @param v_bg_value : V - channel background color
    * @return returns 0 on success
    */
    int decode_with_data(const char* src_data, size_t src_len, int* width, int* height, uint8_t** dst_data,
                         size_t* dst_len, uint8_t background_y = 16, uint8_t background_u = 128,
                         uint8_t background_v = 128);

private:
    /**
    * @brief Initialize the decoder
    *
    * @param config : the configure of decoder
    * @return returns 0 on success
    */
    int init_decode(RXImageLoadConfig config);

    void release();

    RXImageLoadConfig config_;
    AVCodecContext* codec_ctx_ = nullptr;
    AVCodec* codec_ = nullptr;
    AVFrame* frame_ = nullptr;
    AVFrame* frame_yuv_ = nullptr;
    int dst_width_ = 0;
    int dst_height_ = 0;
    AVPacket packet_;
    SwsContext* img_convert_ctx_;

    AVPixelFormat fmt_ = AV_PIX_FMT_NONE;

    int init_status_ = -1;
};

}  // namespace realx
