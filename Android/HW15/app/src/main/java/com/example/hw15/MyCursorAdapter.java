package com.example.hw15;

import static android.content.Context.MODE_PRIVATE;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;

import android.content.DialogInterface;
import android.widget.SimpleCursorAdapter;

public class MyCursorAdapter extends SimpleCursorAdapter {
    private int layout_;
    private Cursor cursor;
    String[] from;
    int[] to;
    ListView listView;
    EditText edit2;

    public MyCursorAdapter(Context context, int layout, Cursor
            c, String[] from, int[] to) {
        super(context, layout, c, from, to);
        layout_ = layout;
        cursor = c;
    }

    @Override
    public void bindView(View view, Context _context, Cursor
            cursor) {
        String data =
                cursor.getString(cursor.getColumnIndexOrThrow("Name"));
        int id = cursor.getInt(cursor.getColumnIndexOrThrow("_id"));
        TextView text =
                view.findViewById(R.id.textViewListItemText);
        text.setText(data);
        Button butdel = view.findViewById(R.id.buttonDelete);
        Button butedit = view.findViewById(R.id.buttonEdit);
        listView = MainActivity.listView;
        butdel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                SQLiteDatabase db =
                        _context.openOrCreateDatabase("DBName", MODE_PRIVATE, null);
                db.execSQL("DELETE FROM MyTable5 WHERE _id=" + id + "");
                Cursor cursor = db.rawQuery("SELECT * FROM Mytable5", null);
                from = new String[]{"Name"};
                to = new int[]{R.id.textViewListItemText};
                MyCursorAdapter scAdapter = new
                        MyCursorAdapter(_context, R.layout.list_item, cursor, from, to);
                listView.setAdapter(scAdapter);
                db.close();
                Toast.makeText(_context, "row deleted from the db id = " + id, Toast.LENGTH_LONG).show();
            }
        });
        butedit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                AlertDialog.Builder dialog = new AlertDialog.Builder(_context);
                dialog.setMessage("Enter new value:");
                dialog.setTitle("Changing the item");

// Создаем EditText динамически
                EditText edit2 = new EditText(_context);
                edit2.setId(View.generateViewId()); // Генерируем уникальный ID для EditText
                edit2.setText(text.getText().toString());

                dialog.setView(edit2); // Добавляем EditText в диалоговое окно

                dialog.setNeutralButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int i) {
                        SQLiteDatabase db = _context.openOrCreateDatabase("DBName", MODE_PRIVATE, null);
                        db.execSQL("UPDATE MyTable5 SET Name = '" + edit2.getText().toString() + "'WHERE _id = " + id + "");
                        Cursor cursor = db.rawQuery("SELECT * FROM Mytable5", null);
                        from = new String[]{"Name"};
                        to = new int[]{R.id.textViewListItemText};
                        MyCursorAdapter scAdapter = new MyCursorAdapter(_context, R.layout.list_item, cursor, from, to);
                        listView.setAdapter(scAdapter);
                        db.close();
                        Toast.makeText(_context, "row edited from the db row id = " + id, Toast.LENGTH_LONG).show();
                        dialog.dismiss();
                    }
                });

                dialog.setIcon(R.mipmap.ic_launcher_round);
                AlertDialog alertDialog = dialog.create();
                alertDialog.show();
            }
        });

    }

    @Override
    public View newView(Context context, Cursor cursor,
                        ViewGroup parent) {
        LayoutInflater inflater = (LayoutInflater)
                context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View view = inflater.inflate(layout_, parent, false);
        return view;
    }
}