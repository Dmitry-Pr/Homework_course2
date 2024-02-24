package com.example.hw2;


import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.SwitchCompat;

import android.content.res.Configuration;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {
    private SwitchCompat theme_switcher;
    private Button output_button;
    private EditText input_field;
    private TextView output_field;

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

        output_button = findViewById(R.id.output_button);
        input_field = findViewById(R.id.input_field);
        output_field = findViewById(R.id.output_field);


        String current_text = getString(R.string.output_hint);
        if(savedInstanceState != null) {
            String savedText = savedInstanceState.getString("current_text");
            if(savedText != null) {
                current_text = savedText;
            }
        }
        output_field.setText(current_text);
        output_button.setOnClickListener(v -> {
            String text = input_field.getText().toString();
            if (text.isEmpty()) {
                text = getString(R.string.output_hint);
            }
            output_field.setText(text);
        });
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putString("current_text", output_field.getText().toString());
    }
}