run the program

g++ -std=c++14 yuv_op.cpp -o main lib/libyuv.a -I include/

check the result 

ffplay -f rawvideo -pixel_format yuv420p -video_size 700x700 rainbow-yuv420p-rotation.yuv