package com.example.hw10;


import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.SwitchCompat;

import android.app.DatePickerDialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import org.w3c.dom.Text;

import java.util.Calendar;


public class MainActivity extends AppCompatActivity {
    private SwitchCompat theme_switcher;

    @Override
    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        TextView dateText = findViewById(R.id.date_text);
        String dateTextString = dateText.getText().toString();
        outState.putString("dateText", dateTextString);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (savedInstanceState != null) {
            String savedDateText = savedInstanceState.getString("dateText", "");
            TextView dateText = findViewById(R.id.date_text);
            dateText.setText(savedDateText);
        }
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


        Button button = findViewById(R.id.button);
        button.setOnClickListener(v -> {
            AlertDialog.Builder dialog = new
                    AlertDialog.Builder(MainActivity.this);
            dialog.setMessage("Вы действительно хотите выйти?");
            dialog.setCancelable(false);
            dialog.setPositiveButton("Да", (dialog12, which) -> MainActivity.this.finish());
            dialog.setNegativeButton("Нет", (dialog1, which) -> dialog1.cancel());
            AlertDialog alertDialog = dialog.create();
            alertDialog.show();
        });

        Button dateButton = findViewById(R.id.date_button);
        dateButton.setOnClickListener(v -> {
            DatePickerDialog datePickerDialog = new DatePickerDialog(
                    MainActivity.this,
                    (view, year, month, dayOfMonth) -> {
                        String selectedDate = dayOfMonth + "/" + (month + 1) + "/" + year;
                        SharedPreferences sharedPreferences = getSharedPreferences("MySharedPref", MODE_PRIVATE);
                        setDate(selectedDate);
                        SharedPreferences.Editor myEdit = sharedPreferences.edit();
                        myEdit.putString("dateText", selectedDate);
                        myEdit.apply();
                    },
                    Calendar.getInstance().get(Calendar.YEAR),
                    Calendar.getInstance().get(Calendar.MONTH),
                    Calendar.getInstance().get(Calendar.DAY_OF_MONTH)
            );
            datePickerDialog.show();
        });

        SharedPreferences sharedPreferences = getSharedPreferences("MySharedPref", MODE_PRIVATE);
        String dateTextString = sharedPreferences.getString("dateText", "");
        if (!dateTextString.equals("")) {
            setDate(dateTextString);
        }
    }

    private void setDate(String selectedDate) {
        TextView dateInfo = findViewById(R.id.date_info);
        dateInfo.setVisibility(TextView.VISIBLE);
        TextView dateText = findViewById(R.id.date_text);
        dateText.setText(selectedDate);
        dateText.setVisibility(TextView.VISIBLE);
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
            AlertDialog.Builder dialog = new
                    AlertDialog.Builder(MainActivity.this);
            try {
                dialog.setMessage(getTitle().toString() + " версия " +
                        getPackageManager().getPackageInfo(getPackageName(), 0).versionName +
                        "\r\n\nПрограмма с примером выполнения диалогового окна \r\n\n Автор - " +
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
        return super.onOptionsItemSelected(item);
    }
}