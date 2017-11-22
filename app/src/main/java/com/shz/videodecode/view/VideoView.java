package com.shz.videodecode.view;


import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.jar.Attributes;

/**
 * Created by Administrator on 2017/11/22.
 */

public class VideoView
        extends SurfaceView
{
    public VideoView(Context context) {
        super(context);
        init();
    }

    public VideoView(Context context, Attributes attrs){
        super(context, (AttributeSet) attrs);
        init();
    }

    public VideoView(Context context,Attributes attrs,int defStyle){
        super(context, (AttributeSet) attrs, defStyle);
        init();
    }

    private void init(){
        /*初始化 sufaceView 绘制的像素格式*/
        SurfaceHolder holder= getHolder();
        holder.setFormat(PixelFormat.RGBA_8888);
    }

}
