package com.example.hw7;


import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.SwitchCompat;

import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.content.SharedPreferences;
import android.widget.Button;


public class MainActivity extends AppCompatActivity {
    private SwitchCompat theme_switcher;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        theme_switcher = findViewById(R.id.theme_switcher);
        int currentNightMode = getResources().getConfiguration().uiMode & Configuration.UI_MODE_NIGHT_MASK;
        SharedPreferences sharedPreferences = getSharedPreferences("ThemePref", MODE_PRIVATE);
        SharedPreferences.Editor editor = sharedPreferences.edit();
        switch (currentNightMode) {
            case Configuration.UI_MODE_NIGHT_NO:
                // Night mode is not active, we're in day light mode
                theme_switcher.setChecked(false);
                editor.putBoolean("isDarkTheme", false);
                break;
            case Configuration.UI_MODE_NIGHT_YES:
                // Night mode is active, we're in dark mode
                theme_switcher.setChecked(true);
                editor.putBoolean("isDarkTheme", true);
                break;
        }
        theme_switcher.setOnCheckedChangeListener((buttonView, isChecked) -> {
            if (buttonView.isPressed()) {
                if (!isChangingConfigurations()) {
                    if (isChecked) {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES);
                        editor.putBoolean("isDarkTheme", true);
                    } else {
                        AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
                        editor.putBoolean("isDarkTheme", false);
                    }
                    editor.apply();
                }
            }
        });
        Button button = findViewById(R.id.maps_button);
        button.setOnClickListener(v -> {
            Intent intent = new Intent(MainActivity.this, MapsActivity.class);
            startActivity(intent);
        });
    }

}