package com.shz.videodecode;

import android.view.Surface;

/**
 * Created by Administrator on 2017/11/22.
 */

public class Player {

    public native static void play(String path, Surface surface);
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("avcodec");
        System.loadLibrary("avdevice");
        System.loadLibrary("avfilter");
        System.loadLibrary("avformat");
        System.loadLibrary("avutil");
        System.loadLibrary("swresample");
        System.loadLibrary("swscale");
    }

}
