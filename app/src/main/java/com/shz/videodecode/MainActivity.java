package com.shz.videodecode;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.shz.videodecode.view.VideoView;

import java.io.File;

public class MainActivity
        extends AppCompatActivity
{
    private VideoView videoView;

    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        videoView= (VideoView)findViewById(R.id.video_view);

    }
    /*播放 @param btn*/
    public void mPlay(View btn){
        /*视频的路径 压缩的视频数据 */
        String path = new File(Environment.getExternalStorageDirectory(),"hehuoren.flv").getAbsolutePath();
        Player.play(path,videoView.getHolder().getSurface());
    }
}
