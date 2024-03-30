package com.example.antiprocrastination;

import android.annotation.SuppressLint;
import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.os.IBinder;
import android.os.Vibrator;
import android.util.Log;

import androidx.core.app.NotificationCompat;

import java.util.Locale;
import java.util.concurrent.TimeUnit;

public class SensorService extends Service implements SensorEventListener {
    private SensorManager sensorManager;
    private Sensor accelerometer;
    private Vibrator vibrator;
    private static final String CHANNEL_ID = "SensorServiceChannel";
    private long lastMovementTime;
    private long stationaryTime;
    private long sessionRecord;
    private long recordTime;
    private SharedPreferences sharedPreferences;

    @Override
    public void onCreate() {
        super.onCreate();

        sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
        if (accelerometer != null) {
            sensorManager.registerListener(this, accelerometer, 250000);
        } else {
            Log.e("SensorService", "Device does not support accelerometer");
        }
        sharedPreferences = getSharedPreferences("appPreferences", MODE_PRIVATE);
        stationaryTime = sharedPreferences.getLong("stationaryTime", 0);
        recordTime = sharedPreferences.getLong("recordTime", 0);
        lastMovementTime = System.currentTimeMillis();
        sessionRecord = sharedPreferences.getLong("sessionRecord", 0);
    }

    @SuppressLint("ForegroundServiceType")
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        createNotificationChannel();
        Intent notificationIntent = new Intent(this, MainActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this,
                0, notificationIntent, PendingIntent.FLAG_IMMUTABLE);

        Notification notification = new NotificationCompat.Builder(this, CHANNEL_ID)
                .setContentTitle("Anti Procrastination Service")
                .setContentText("Service is running...")
                .setSmallIcon(R.drawable.ic_sun)
                .setContentIntent(pendingIntent)
                .build();

        startForeground(1, notification);

        return START_STICKY;
    }

    private void createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel serviceChannel = new NotificationChannel(
                    CHANNEL_ID,
                    "Anti Procrastination Service Channel",
                    NotificationManager.IMPORTANCE_DEFAULT
            );

            NotificationManager manager = getSystemService(NotificationManager.class);
            manager.createNotificationChannel(serviceChannel);
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        stationaryTime = System.currentTimeMillis() - lastMovementTime;
        SharedPreferences.Editor editor = sharedPreferences.edit();
        long stationaryTimeInSeconds = TimeUnit.MILLISECONDS.toSeconds(stationaryTime);
        editor.putLong("last_session_time", stationaryTimeInSeconds);
        Intent intent = new Intent("com.example.antiprocrastination.UPDATE_TIME");
        if (stationaryTimeInSeconds > recordTime) {
            recordTime = stationaryTimeInSeconds;
            editor.putLong("recordTime", recordTime);
            Intent intent_notify = new Intent("com.example.antiprocrastination.NOTIFICATION");
            intent_notify.putExtra("message", "Ваше сообщение здесь");
            sendBroadcast(intent);
        }
        if (stationaryTimeInSeconds > sessionRecord) {
            sessionRecord = stationaryTimeInSeconds;
            editor.putLong("sessionRecord", 0);
        }
        editor.apply();

        intent.putExtra("stationaryTime", formatTime(sessionRecord));
        intent.putExtra("recordTime", formatTime(recordTime));
        sendBroadcast(intent);

        sensorManager.unregisterListener(this);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];

            if (z < 9 && z > -9) {
                if (vibrator.hasVibrator()) {
                    vibrator.vibrate(10000);
                }
                if (lastMovementTime != 0) {
                    stationaryTime = System.currentTimeMillis() - lastMovementTime;
                    SharedPreferences.Editor editor = sharedPreferences.edit();
                    long stationaryTimeInSeconds = TimeUnit.MILLISECONDS.toSeconds(stationaryTime);
                    editor.putLong("last_session_time", stationaryTimeInSeconds);
                    Intent intent = new Intent("com.example.antiprocrastination.UPDATE_TIME");
                    if (stationaryTimeInSeconds > recordTime) {
                        recordTime = stationaryTimeInSeconds;
                        editor.putLong("recordTime", recordTime);
                    }
                    if (stationaryTimeInSeconds > sessionRecord) {
                        sessionRecord = stationaryTimeInSeconds;
                        editor.putLong("sessionRecord", sessionRecord);
                    }
                    editor.apply();
                    intent.putExtra("stationaryTime", formatTime(sessionRecord));
                    intent.putExtra("recordTime", formatTime(recordTime));
                    sendBroadcast(intent);
                }
                lastMovementTime = System.currentTimeMillis();
            } else {
                vibrator.cancel();
            }
        }
    }

    private String formatTime(long timeInSeconds) {
        long hours = TimeUnit.SECONDS.toHours(timeInSeconds);
        long minutes = TimeUnit.SECONDS.toMinutes(timeInSeconds) % 60;
        long seconds = timeInSeconds % 60;
        return String.format(Locale.getDefault(), "%02d:%02d:%02d", hours, minutes, seconds);
    }

    @Override
    public void onTaskRemoved(Intent rootIntent) {
        long currentTime = System.currentTimeMillis();
        if (lastMovementTime != 0) {
            long currentDuration = currentTime - lastMovementTime;
            long currentDurationInSeconds = TimeUnit.MILLISECONDS.toSeconds(currentDuration);
            if (currentDurationInSeconds > recordTime) {
                recordTime = currentDurationInSeconds;
                SharedPreferences.Editor editor = sharedPreferences.edit();
                editor.putLong("recordTime", recordTime);
                editor.apply();
            }
            if (currentDurationInSeconds > sessionRecord) {
                sessionRecord = currentDurationInSeconds;
            }
            Intent intent = new Intent("com.example.antiprocrastination.UPDATE_TIME");
            intent.putExtra("stationaryTime", formatTime(sessionRecord));
            intent.putExtra("recordTime", formatTime(recordTime));
            sendBroadcast(intent);
        }
        super.onTaskRemoved(rootIntent);
        stopSelf();
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        // Not used
    }
}