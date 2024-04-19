cmake_minimum_required(VERSION 3.5)

project(testFFMPEG LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories("lib/ffmpeg/include")
# link_directories("lib/ffmpeg/lib")

# 添加ffmpeg和SDL2
# 需要自己下载ffmpeg和SDL2库，然后解压到lib目录下
# 然后使用cmake生成相应的库文件
set(FFMPEG_DIR "../lib/ffmpeg")
set(SDL2_DIR "../lib/SDL2/2.30.2")
include_directories(${FFMPEG_DIR}/include)
link_directories(${FFMPEG_DIR}/lib)
include_directories(${SDL2_DIR}/include)
link_directories(${SDL2_DIR}/lib)



add_executable(testFFMPEG main.cpp)

target_link_libraries(testFFMPEG avcodec avdevice avfilter avformat avutil postproc swresample swscale)

install(TARGETS testFFMPEG
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
