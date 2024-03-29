package com.example.hw15;


import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDelegate;
import androidx.appcompat.widget.SwitchCompat;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;


public class MainActivity extends AppCompatActivity {
    private SwitchCompat theme_switcher;
    private EditText edit1;
    Integer i;
    String[] from;
    int[] to;
    static ListView listView;

    @SuppressLint("Range")
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
                if (isChecked) {
                    AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_YES);
                } else {
                    AppCompatDelegate.setDefaultNightMode(AppCompatDelegate.MODE_NIGHT_NO);
                }
            }
        });
        edit1 = findViewById(R.id.editText);
        SharedPreferences save = getSharedPreferences("SAVE", 0);
        edit1.setText(save.getString("text", ""));

        from = new String[]{"Name"};
        to = new int[]{R.id.textViewListItemText};
        Button btnadd = findViewById(R.id.buttonAdd);

        final EditText editadd =
                findViewById(R.id.editTextAddingARecord);
        SQLiteDatabase db =
                openOrCreateDatabase("DBName", MODE_PRIVATE, null);
        db.execSQL("CREATE TABLE IF NOT EXISTS MyTable5 (_id INTEGER PRIMARY KEY AUTOINCREMENT, Name VARCHAR);");
        Cursor cursor = db.rawQuery("SELECT * FROM Mytable5",
                null);
        i = cursor.getCount() + 1;
        if (cursor.getCount() > 0) {
            MyCursorAdapter scAdapter = new
                    MyCursorAdapter(MainActivity.this, R.layout.list_item, cursor, from
                    , to);
            listView = findViewById(R.id.list);
            listView.setAdapter(scAdapter);
        }
        db.close();
        btnadd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SQLiteDatabase db =
                        openOrCreateDatabase("DBName", MODE_PRIVATE, null);
                Cursor cursor2 = db.rawQuery("SELECT * FROM Mytable5", null);
                i = cursor2.getCount() + 1;
//цикл для того, чтобы подбирать значения _id и не допускать повторения одинаковых значений (primary key как-никак)
                for (int k = 1; k <= i; k++) {
                    Cursor cursor3 = db.rawQuery("SELECT * FROM Mytable5 WHERE _id = " + k + "", null);
                    if (cursor3.getCount() == 0) {
                        i = k;
                        break;
                    }
                }
                db.execSQL("INSERT INTO MyTable5 VALUES ('" + i + "','" + editadd.getText().toString() + "');");
//i++;
                Cursor cursor = db.rawQuery("SELECT * FROM Mytable5", null);
                MyCursorAdapter scAdapter = new MyCursorAdapter(
                        MainActivity.this, R.layout.list_item, cursor, from, to);
                listView = getListView();
                listView.setAdapter(scAdapter);
                db.close();
                Toast.makeText(getListView().getContext(), "a row added to the table", Toast.LENGTH_LONG).show();
            }
        });

    }

    private ListView getListView() {
        return findViewById(R.id.list);
    }

    @Override
    protected void onStop() {
        super.onStop();
        SharedPreferences save = getSharedPreferences("SAVE", 0);
        SharedPreferences.Editor editor = save.edit();
        editor.putString("text", edit1.getText().toString());
        editor.apply();
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
                        "\r\n\nПрограмма с БД \r\n\n Автор - " +
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
            Intent intent = new Intent(MainActivity.this,
                    SettingsActivity.class);
            startActivity(intent);
        }
        return super.onOptionsItemSelected(item);
    }
}