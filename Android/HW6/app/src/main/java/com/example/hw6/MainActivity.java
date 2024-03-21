package com.example.hw6;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.AnimatorSet;
import android.animation.ObjectAnimator;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.SwitchCompat;

public class MainActivity extends AppCompatActivity {
    private SwitchCompat theme_switcher;
    private Button animatedButton;
    private TextView animatedTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        theme_switcher = findViewById(R.id.theme_switcher);
        animatedButton = findViewById(R.id.animatedButton);
        animatedTextView = findViewById(R.id.animatedTextView);

        animatedButton.setOnClickListener(v -> animateButtonTranslationRight(v));
        animatedTextView.setOnClickListener(v -> animateTextViewUp(v));

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
    }

    private void animateButton(View view) {
        ObjectAnimator scaleX = ObjectAnimator.ofFloat(view, "scaleX", 1.2f);
        ObjectAnimator scaleY = ObjectAnimator.ofFloat(view, "scaleY", 1.2f);
        AnimatorSet animatorSet = new AnimatorSet();
        animatorSet.playTogether(scaleX, scaleY);
        animatorSet.setDuration(300);
        animatorSet.start();
    }
    private void animateButtonTranslationRight(View view) {
        ObjectAnimator translationX = ObjectAnimator.ofFloat(view, "translationX", 100f);
        ObjectAnimator translationY = ObjectAnimator.ofFloat(view, "translationY", 100f);
        AnimatorSet animatorSet = new AnimatorSet();
        animatorSet.playTogether(translationX, translationY);
        animatorSet.setDuration(500);
        view.setOnClickListener(v -> animateButtonTranslationLeft(v));
        animatorSet.start();
    }
    private void animateButtonTranslationLeft(View view) {
        ObjectAnimator translationX = ObjectAnimator.ofFloat(view, "translationX", -100f);
        ObjectAnimator translationY = ObjectAnimator.ofFloat(view, "translationY", -100f);
        AnimatorSet animatorSet = new AnimatorSet();
        animatorSet.playTogether(translationX, translationY);
        animatorSet.setDuration(500);
        view.setOnClickListener(v -> animateButtonTranslationRight(v));
        animatorSet.start();
    }
    private void animateTextViewUp(View view) {
        ObjectAnimator rotation = ObjectAnimator.ofFloat(view, "rotation", 0f, 360f);
        ObjectAnimator translationY = ObjectAnimator.ofFloat(view, "translationY", 100f);
        AnimatorSet animatorSet = new AnimatorSet();
        animatorSet.playTogether(rotation, translationY);
        animatorSet.setDuration(500);
        view.setOnClickListener(v -> animateTextViewDown(v));
        animatorSet.start();
    }
    private void animateTextViewDown(View view) {
        ObjectAnimator rotation = ObjectAnimator.ofFloat(view, "rotation", 0f, 360f);
        ObjectAnimator translationY = ObjectAnimator.ofFloat(view, "translationY", -100f);
        AnimatorSet animatorSet = new AnimatorSet();
        animatorSet.playTogether(rotation, translationY);
        animatorSet.setDuration(500);
        view.setOnClickListener(v -> animateTextViewUp(v));
        animatorSet.start();
    }
}