package com.example.hw9;


import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.SwitchCompat;

import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.content.res.Configuration;
import android.graphics.Color;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.style.BackgroundColorSpan;
import android.text.style.ForegroundColorSpan;
import android.view.ContextMenu;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity {
    private SwitchCompat theme_switcher;

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
        theme_switcher.setOnCheckedChangeListener((buttonView, isChecked) -> {
            if (isChecked) {
                AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES);
            } else {
                AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
            }
        });
        TextView textView = findViewById(R.id.text1);
        registerForContextMenu(textView);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.options_menu, menu);
        return true;
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v, ContextMenu.ContextMenuInfo menuInfo) {
        getMenuInflater().inflate(R.menu.context_menu, menu);
        for (int i = 0; i < menu.size(); i++) {
            MenuItem item = menu.getItem(i);
            SpannableString spanString = new SpannableString(menu.getItem(i).getTitle().toString());
            int currentNightMode = getResources().getConfiguration().uiMode & Configuration.UI_MODE_NIGHT_MASK;
            if (currentNightMode == Configuration.UI_MODE_NIGHT_YES) {
                spanString.setSpan(new ForegroundColorSpan(Color.GRAY), 0, spanString.length(), 0);
            } else {
                spanString.setSpan(new ForegroundColorSpan(Color.BLACK), 0, spanString.length(), 0);
            }
            item.setTitle(spanString);
        }
        super.onCreateContextMenu(menu, v, menuInfo);
    }

    @Override
    public boolean onContextItemSelected(@NonNull MenuItem item) {

        int id = item.getItemId();
        if (id == R.id.color_red) {
            TextView textView = findViewById(R.id.text1);
            textView.setTextColor(Color.parseColor("red"));
        }
        if (id == R.id.color_black) {
            TextView textView = findViewById(R.id.text1);
            textView.setTextColor(Color.parseColor("black"));
        }
        return super.onContextItemSelected(item);
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.show_text) {
            if (item.isChecked()) {
                TextView textView = findViewById(R.id.text1);
                textView.setVisibility(TextView.VISIBLE);
                item.setChecked(false);
            } else {
                TextView textView = findViewById(R.id.text1);
                textView.setVisibility(TextView.INVISIBLE);
                item.setChecked(true);
            }
            return true;
        } else if (id == R.id.about) {
            Toast.makeText(this, R.string.person, Toast.LENGTH_LONG).show();
            return true;
        } else if (id == R.id.loop) {
            ObjectAnimator rotation = ObjectAnimator.ofFloat(findViewById(R.id.button), "rotation", 0f, 360f);
            AnimatorSet animatorSet = new AnimatorSet();
            animatorSet.play(rotation);
            animatorSet.setDuration(500);
            animatorSet.start();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

}