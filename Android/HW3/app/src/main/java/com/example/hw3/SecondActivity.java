package com.example.hw3;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class SecondActivity extends AppCompatActivity {
    private TextView outputField;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_second);
        outputField = findViewById(R.id.output_field);
        Intent intent = getIntent();
        if (intent.hasExtra("userInput")) {
            String userInput = intent.getStringExtra("userInput");
            outputField.setText(userInput);
        }
    }
    public void startMainActivity(View v) {
        Intent intent = new Intent(SecondActivity.this, MainActivity.class);
        String userInput = outputField.getText().toString();
        intent.putExtra("userInput", userInput);
        startActivity(intent);
    }
}