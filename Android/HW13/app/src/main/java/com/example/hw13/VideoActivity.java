package com.example.hw13;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.MediaController;
import android.widget.VideoView;

import androidx.appcompat.app.AppCompatActivity;

public class VideoActivity extends AppCompatActivity {

    private VideoView videoView;
    private EditText urlInput;
    private Button returnButton;
    private Button playButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_video);
        videoView = findViewById(R.id.second_videoView);
        urlInput = findViewById(R.id.urlInput);
        returnButton = findViewById(R.id.return_button);
        playButton = findViewById(R.id.play_button);

        SharedPreferences sharedPreferences = getSharedPreferences("sharedPrefs", MODE_PRIVATE);
        String userInput = sharedPreferences.getString("userInput", "");
        urlInput.setText(userInput);

        playButton.setOnClickListener(v -> {
            String videoPath = urlInput.getText().toString();
            videoView.setVideoPath(videoPath);

            videoView.setMediaController(new MediaController(this));
            videoView.start();
            videoView.requestFocus();
        });

        returnButton.setOnClickListener(v -> {
            sharedPreferences.edit().putString("userInput", urlInput.getText().toString()).apply();
            Intent intent = new Intent(VideoActivity.this, MainActivity.class);
            startActivity(intent);
        });
    }
}