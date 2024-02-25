package com.example.hw3;


import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.SwitchCompat;

import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;

import android.widget.EditText;


public class MainActivity extends AppCompatActivity {
    private SwitchCompat theme_switcher;
    private EditText inputField;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        theme_switcher = findViewById(R.id.theme_switcher);
        int currentNightMode = getResources().getConfiguration().uiMode & Configuration.UI_MODE_NIGHT_MASK;
        switch (currentNightMode) {
            case Configuration.UI_MODE_NIGHT_NO:
                // Night mode is not active, we're in day light mode
                theme_switcher.setChecked(false);
                break;
            case Configuration.UI_MODE_NIGHT_YES:
                // Night mode is active, we're in dark mode
                theme_switcher.setChecked(true);
                break;
        }
        theme_switcher.setOnClickListener(v -> {
            boolean checked = theme_switcher.isChecked();

            if (checked) {
                AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES);
            } else {
                AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
            }
        });

        inputField = findViewById(R.id.input_field);
        Intent intent = getIntent();
        if (intent.hasExtra("userInput")) {
            String userInput = intent.getStringExtra("userInput");
            inputField.setText(userInput);
        }

    }

    public void startSecondActivity(View v) {
        Intent intent = new Intent(MainActivity.this, SecondActivity.class);
        String userInput = inputField.getText().toString();
        intent.putExtra("userInput", userInput);
        startActivity(intent);
    }
}