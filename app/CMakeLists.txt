# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)


find_library( log-lib
              log )

find_library( android-lib
              android )

set(distribution_DIR src/../libs)
#set(Prebuilt_DIR src/main/cpp/Prebuilt/)

#avcodec  avdevice  avfilter avformat avutil swresample swscale
add_library(avcodec
            SHARED
            IMPORTED)
set_target_properties(  avcodec
                        PROPERTIES IMPORTED_LOCATION
                        ${distribution_DIR}/${ANDROID_ABI}/libavcodec.so)
add_library(avdevice
            SHARED
            IMPORTED)
set_target_properties(  avdevice
                        PROPERTIES IMPORTED_LOCATION
                        ${distribution_DIR}/${ANDROID_ABI}/libavdevice.so)
add_library(avfilter
            SHARED
            IMPORTED)
set_target_properties(  avfilter
                        PROPERTIES IMPORTED_LOCATION
                        ${distribution_DIR}/${ANDROID_ABI}/libavfilter.so)
add_library(avformat
            SHARED
            IMPORTED)
set_target_properties(  avformat
                        PROPERTIES IMPORTED_LOCATION
                        ${distribution_DIR}/${ANDROID_ABI}/libavformat.so)
add_library(avutil
            SHARED
            IMPORTED)
set_target_properties(  avutil
                        PROPERTIES IMPORTED_LOCATION
                        ${distribution_DIR}/${ANDROID_ABI}/libavutil.so)
add_library(swresample
            SHARED
            IMPORTED)
set_target_properties(  swresample
                        PROPERTIES IMPORTED_LOCATION
                        ${distribution_DIR}/${ANDROID_ABI}/libswresample.so)
add_library(swscale
            SHARED
            IMPORTED)
set_target_properties(  swscale
                        PROPERTIES IMPORTED_LOCATION
                        ${distribution_DIR}/${ANDROID_ABI}/libswscale.so)
add_library(yuv
            SHARED
            IMPORTED)
set_target_properties(  yuv
                        PROPERTIES IMPORTED_LOCATION
                        ${distribution_DIR}/${ANDROID_ABI}/libyuv.so)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

add_library(VideoDecode
            SHARED
            src/main/cpp/VideoDecode.cpp
            )
include_directories(libs/include/libavcodec)
include_directories(libs/include/libavdevice)
include_directories(libs/include/libavfilter)
include_directories(libs/include/libavformat)
include_directories(libs/include/libavutil)
include_directories(libs/include/libpostproc)
include_directories(libs/include/libswresample)
include_directories(libs/include/libswscale)
include_directories(libs/include/libyuv)
include_directories(libs/include/libyuv/libyuv)
include_directories(src/main/cpp)

target_link_libraries(
                       VideoDecode
                       yuv
                       avcodec
                       avdevice
                       avfilter
                       avformat
                       avutil
                       swresample
                       swscale
                       ${log-lib}
                       ${android-lib} )




