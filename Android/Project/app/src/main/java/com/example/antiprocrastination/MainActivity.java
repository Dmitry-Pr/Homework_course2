package com.example.antiprocrastination;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Vibrator;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.SwitchCompat;

import java.util.Locale;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {
    private SwitchCompat theme_switcher;
    private TextView lastSessionText;
    private Boolean isServiceRunning = false;
    private TextView recordText;
    private BroadcastReceiver receiver;
    private SharedPreferences sharedPreferences;
    private ProgressBar recordProgressBar;
    private CountDownTimer countDownTimer;
    private CountDownTimer countDownTimerText;
    private long recordTimeInSeconds;
    private long millisUntilFinished;
    private long millisUntilFinishedText;
    private TextView timerText;
    SwitchCompat buttonStart;

    @RequiresApi(api = Build.VERSION_CODES.TIRAMISU)
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
            if (buttonView.isPressed()) {
                if (isServiceRunning) {
                    return;
                }
//                SharedPreferences.Editor editor = sharedPreferences.edit();
//                editor.putInt("progress", recordProgressBar.getProgress());
//                if (countDownTimer != null) {
//                    editor.putLong("remainingTime", millisUntilFinished);
//                }
//                if (countDownTimerText != null) {
//                    editor.putLong("remainingTimeText", millisUntilFinishedText);
//                }
//                editor.putBoolean("isServiceRunning", isServiceRunning);
//                editor.apply();
                if (isChecked) {
                    AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES);
                } else {
                    AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
                }
            }
        });

        lastSessionText = findViewById(R.id.last_session_text);
        recordText = findViewById(R.id.record_text);
        sharedPreferences = getSharedPreferences("appPreferences", MODE_PRIVATE);
        long recordTime = sharedPreferences.getLong("recordTime", 0);
//        isServiceRunning = sharedPreferences.getBoolean("isServiceRunning", false);
        recordText.setText(formatTime(recordTime));
        lastSessionText.setText(sharedPreferences.getString("lastSessionText", "00:00:00"));
        lastSessionText.setText(formatTime(0));
        recordProgressBar = findViewById(R.id.recordProgressBar);
        timerText = findViewById(R.id.timerText);
//        if (isServiceRunning) {
//            int progress = sharedPreferences.getInt("progress", 0);
//            recordProgressBar.setProgress(progress);
//            long remainingTime = sharedPreferences.getLong("remainingTime", 0);
//            if (remainingTime > 0) {
//                startCountDownTimer(TimeUnit.MILLISECONDS.toSeconds(remainingTime));
//            }
//            long remainingTimeText = sharedPreferences.getLong("remainingTimeText", 0);
//
//
//        }
        recordTimeInSeconds = recordTime;
        MainActivity obj = this;
        buttonStart = findViewById(R.id.button_start);
        buttonStart.setOnCheckedChangeListener((view, isChecked) -> {
            if (isChecked) {
                isServiceRunning = true;

                Intent intent = new Intent(obj, SensorService.class);
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                    startForegroundService(intent);

                } else {
                    startService(intent);
                }
                startCountDownTimer(recordTimeInSeconds);
                startCountDownTimerText();
            } else {
                isServiceRunning = false;
                Intent intent = new Intent(obj, SensorService.class);
                stopService(intent);
                if (countDownTimer != null) {
                    countDownTimer.cancel();
                }
                if (countDownTimerText != null) {
                    countDownTimerText.cancel();
                }
                timerText.setText("00:00:00");
                recordProgressBar.setProgress(0);
            }
        });

        receiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String stationaryTime = intent.getStringExtra("stationaryTime");
                lastSessionText.setText(stationaryTime);
                SharedPreferences.Editor editor = sharedPreferences.edit();
                editor.putString(lastSessionText.getText().toString(), "lastSessionText");
                editor.apply();
                String recordTime = intent.getStringExtra("recordTime"); // Получаем recordTime из intent
                recordText.setText(recordTime); // Обновляем recordText
                recordTimeInSeconds = TimeUnit.HOURS.toSeconds(Integer.parseInt(recordTime.split(":")[0]))
                        + TimeUnit.MINUTES.toSeconds(Integer.parseInt(recordTime.split(":")[1]))
                        + Integer.parseInt(recordTime.split(":")[2]);
                if (isServiceRunning) {
                    startCountDownTimer(recordTimeInSeconds);
                    startCountDownTimerText();
                }
            }
        }

        ;
//        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
//            registerReceiver(receiver, new IntentFilter("com.example.antiprocrastination.UPDATE_TIME"), Context.RECEIVER_EXPORTED);
//        }


    }

    private void startCountDownTimer(long recordTimeInSeconds) {
        if (countDownTimer != null) {
            countDownTimer.cancel();
        }
        countDownTimer = new CountDownTimer(recordTimeInSeconds * 1000, 100) {
            @Override
            public void onTick(long millisUntilFinished) {
                MainActivity.this.millisUntilFinished = millisUntilFinished;
                int progress = (int) ((1 - (double) millisUntilFinished / (recordTimeInSeconds * 1000)) * 100);
                recordProgressBar.setProgress(progress);
            }

            @Override
            public void onFinish() {
                recordProgressBar.setProgress(100);
            }
        }.start();
    }

    private void startCountDownTimerText() {
        if (countDownTimerText != null) {
            countDownTimerText.cancel();
        }
        countDownTimerText = new CountDownTimer(Long.MAX_VALUE, 1000) {
            @Override
            public void onTick(long millisUntilFinished) {
                MainActivity.this.millisUntilFinishedText = millisUntilFinished;
                long secondsPassed = (Long.MAX_VALUE - millisUntilFinished) / 1000;
                String time = formatTime(secondsPassed);
                timerText.setText(time);
            }

            @Override
            public void onFinish() {
                recordProgressBar.setProgress(100);
            }
        }.start();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (countDownTimer != null) {
            countDownTimer.cancel();
        }
        if (countDownTimerText != null) {
            countDownTimerText.cancel();
        }
    }

    @RequiresApi(api = Build.VERSION_CODES.TIRAMISU)
    @Override
    protected void onResume() {
        super.onResume();
        registerReceiver(receiver, new IntentFilter("com.example.antiprocrastination.UPDATE_TIME"), Context.RECEIVER_EXPORTED);
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(receiver);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.about) {
            AlertDialog.Builder dialog = new AlertDialog.Builder(MainActivity.this);
            try {
                dialog.setMessage(getTitle().toString() + " версия " +
                        getPackageManager().getPackageInfo(getPackageName(), 0).versionName +
                        "\r\n\nПроект для борьбы с прокрастинацией \r\n\n Автор - " +
                        getString(R.string.person));
            } catch (PackageManager.NameNotFoundException e) {
                e.printStackTrace();
            }
            dialog.setTitle("О программе");
            dialog.setNeutralButton("OK", (dialog1, which) -> dialog1.dismiss());
            dialog.setIcon(R.mipmap.ic_launcher_round);
            AlertDialog alertDialog = dialog.create();
            alertDialog.show();
            return true;
        }
        if (id == R.id.settings) {
            new AlertDialog.Builder(MainActivity.this)
                    .setTitle("Переход в настройки")
                    .setMessage("Вы уверены, что хотите перейти в настройки? Это остановит текущую сессию.")
                    .setPositiveButton("ОК", (dialog, which) -> {
                        buttonStart.setChecked(false);
                        Intent intent = new Intent(this, SettingsActivity.class);
                        startActivity(intent);
                    })
                    .setNegativeButton("Отмена", null)
                    .show();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private String formatTime(long timeInSeconds) {
        long hours = TimeUnit.SECONDS.toHours(timeInSeconds);
        long minutes = TimeUnit.SECONDS.toMinutes(timeInSeconds) % 60;
        long seconds = timeInSeconds % 60;
        return String.format(Locale.getDefault(), "%02d:%02d:%02d", hours, minutes, seconds);
    }
}