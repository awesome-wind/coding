#include <_types/_uint8_t.h>
#include <stdint.h>
#include <stdio.h>

#include <iostream>

#include "include/libyuv.h"

enum { k_clip = 0, k_scale = 1, k_rotation = 2, k_mirror = 3, k_blend = 4 };

void clip(uint8_t* srcYuvData, uint8_t* dstYuvData, int width, int height, int cropX, int cropY, int cropWidth,
          int cropHeight) {
    std::cout << "execute clip." << std::endl;
    ConvertToI420(srcYuvData, width * height * 3 / 2, dstYuvData, cropWidth, dstYuvData + cropWidth * cropHeight,
                  (cropWidth + 1) / 2,
                  dstYuvData + cropWidth * cropHeight + ((cropWidth + 1) / 2) * ((cropHeight + 1) / 2),
                  (cropWidth + 1) / 2, cropX, cropY, width, height, cropWidth, cropHeight, libyuv::kRotate0,
                  libyuv::FOURCC_YU12);
}

void scale(uint8_t* src_yuv, uint8_t* dst_yuv, int width, int height, int dst_width, int dst_height) {
    std::cout << "execute scale." << std::endl;
    I420Scale(src_yuv, width, src_yuv + width * height, (width + 1) / 2,
              src_yuv + width * height + ((width + 1) / 2) * ((height + 1) / 2), (width + 1) / 2, width, height,
              dst_yuv, dst_width, dst_yuv + dst_width * dst_height, (dst_width + 1) / 2,
              dst_yuv + dst_width * dst_height + ((dst_width + 1) / 2) * ((dst_height + 1) / 2), (dst_width + 1) / 2,
              dst_width, dst_height, libyuv::kFilterNone);
}

void rotation(uint8_t* src_yuv, uint8_t* dst_yuv, int width, int height) {
    std::cout << "execute rotation." << std::endl;
    I420Rotate(src_yuv, width, src_yuv + width * height, (width + 1) / 2,
               src_yuv + width * height + ((width + 1) / 2) * ((height + 1) / 2), (width + 1) / 2, dst_yuv, width,
               dst_yuv + width * height, (width + 1) / 2,
               dst_yuv + width * height + ((width + 1) / 2) * ((height + 1) / 2), (width + 1) / 2, width, height,
               libyuv::kRotate90);
}

void mirror(uint8_t* src_yuv, uint8_t* dst_yuv, int width, int height) {
    uint8_t* src_y = src_yuv;
    int src_stride_y = width;
    uint8_t* src_u = src_yuv + width * height;
    int src_stride_u = (width + 1) / 2;
    uint8_t* src_v = src_yuv + width * height + ((width + 1) / 2) * ((height + 1) / 2);
    int src_stride_v = (width + 1) / 2;

    uint8_t* dst_y = dst_yuv;
    int dst_stride_y = width;
    uint8_t* dst_u = dst_yuv + width * height;
    int dst_stride_u = (width + 1) / 2;
    uint8_t* dst_v = dst_yuv + width * height + ((width + 1) / 2) * ((height + 1) / 2);
    int dst_stride_v = (width + 1) / 2;
    libyuv::I420Mirror(src_y, src_stride_y, src_u, src_stride_u, src_v, src_stride_v, dst_y, dst_stride_y, dst_u,
                       dst_stride_u, dst_v, dst_stride_v, width, height);
}

void blend(uint8_t* src_yuv, uint8_t* dst_yuv, int width, int height) {
    uint8_t rotation_yuv[width * height * 3 / 2];
    rotation(src_yuv, rotation_yuv, width, height);

    //adjust transparency
    uint8_t alpha[width * height];
    memset(alpha, 0X88, width * height);

    libyuv::I420Blend(src_yuv, width, src_yuv + width * height, (width + 1) / 2,
                      src_yuv + width * height + ((width + 1) / 2) * ((height + 1) / 2), (width + 1) / 2, rotation_yuv,
                      width, rotation_yuv + width * height, (width + 1) / 2,
                      rotation_yuv + width * height + ((width + 1) / 2) * ((height + 1) / 2), (width + 1) / 2, alpha,
                      width, dst_yuv, width, dst_yuv + width * height, (width + 1) / 2,
                      dst_yuv + width * height + ((width + 1) / 2) * ((height + 1) / 2), (width + 1) / 2, width,
                      height);
}

int get_mode(std::string mode) {
    int res = -1;
    if (mode == "clip") {
        res = k_clip;
    } else if (mode == "scale") {
        res = k_scale;
    } else if (mode == "rotation") {
        res = k_rotation;
    } else if (mode == "mirror") {
        res = k_mirror;
    } else if (mode == "blend") {
        res = k_blend;
    }
    std::cout << "mode:" << mode << " res:" << res << std::endl;
    return res;
}

int main(int argc, char* argv[]) {
    for (int i = 0; i < argc; ++i) {
        std::cout << "param :" << i << " value:" << argv[i] << std::endl;
    }

    int width = 700, height = 700;
    int dst_width, dst_height;

    int mode = get_mode(argv[1]);
    switch (mode) {
        case k_clip:
            dst_width = 200;
            dst_height = 200;
            break;
        case k_scale:
            dst_width = 300;
            dst_height = 300;
            break;
        case k_rotation:
        case k_mirror:
        case k_blend:
            dst_width = 700;
            dst_height = 700;
            break;
        default:
            return 1;
    }

    uint8_t yuv[width * height * 3 / 2];
    uint8_t new_yuv[dst_width * dst_height * 3 / 2];

    FILE* yuv420pFile = fopen("rainbow-yuv420p.yuv", "rb");
    fread(yuv, sizeof(yuv), 1, yuv420pFile);

    switch (mode) {
        case k_clip:
            clip(yuv, new_yuv, width, height, 0, 0, dst_width, dst_height);
            break;
        case k_scale:
            scale(yuv, new_yuv, width, height, dst_width, dst_height);
            break;
        case k_rotation:
            rotation(yuv, new_yuv, width, height);
            break;
        case k_mirror:
            mirror(yuv, new_yuv, width, height);
            break;
        case k_blend:
            blend(yuv, new_yuv, width, height);
            break;
        default:
            return 1;
    }

    std::string new_file = "rainbow-yuv420p-" + std::string(argv[1]) + ".yuv";
    FILE* yuv_new = fopen(new_file.c_str(), "wb");
    fwrite(new_yuv, sizeof(new_yuv), 1, yuv_new);

    fclose(yuv_new);
    fclose(yuv420pFile);
    return 0;
}