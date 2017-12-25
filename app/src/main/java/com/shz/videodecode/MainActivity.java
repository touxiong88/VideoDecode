package com.shz.videodecode;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;

import com.shz.videodecode.view.VideoView;

import java.io.File;

public class MainActivity
        extends Activity
{
    private VideoView mVideoView;

    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mVideoView= (VideoView)findViewById(R.id.video_view);

    }
    /*播放 @param btn*/
    public void mPlay(View btn){
        /*视频的路径 压缩的视频数据 */
        String path = new File(Environment.getExternalStorageDirectory(),"hehuoren.flv").getAbsolutePath();
        Player.play(path,mVideoView.getHolder().getSurface());
    }
}
