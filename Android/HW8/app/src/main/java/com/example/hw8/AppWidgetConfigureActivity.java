package com.example.hw8;

import android.app.Activity;
import android.app.PendingIntent;
import android.appwidget.AppWidgetManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RemoteViews;

import androidx.annotation.Nullable;

import com.example.hw8.databinding.AppWidgetConfigureBinding;

/**
 * The configuration screen for the {@link AppWidget AppWidget} AppWidget.
 */
public class AppWidgetConfigureActivity extends Activity {

    private AppWidgetConfigureActivity context;
    private int widgetID;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.app_widget_configure);
        setResult(RESULT_CANCELED);
        context = this;
        Bundle extras = getIntent().getExtras();
        if (extras != null) {
            widgetID =
                    extras.getInt(AppWidgetManager.EXTRA_APPWIDGET_ID,
                            AppWidgetManager.INVALID_APPWIDGET_ID);
            final AppWidgetManager widgetManager =
                    AppWidgetManager.getInstance(context);
            final RemoteViews views = new
                    RemoteViews(context.getPackageName(), R.layout.app_widget);
            final EditText editText = (EditText)
                    findViewById(R.id.appwidget_text);
            Button button = (Button) findViewById(R.id.add_button);
            button.setOnClickListener(v -> {
                Intent intent = new Intent(Intent.ACTION_VIEW,
                        Uri.parse(editText.getText().toString()));
                PendingIntent pending = PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_IMMUTABLE );
                views.setOnClickPendingIntent(R.id.appwidget_text, pending);
                widgetManager.updateAppWidget(widgetID, views);
                Intent resultValue = new Intent();
                resultValue.putExtra(AppWidgetManager.EXTRA_APPWIDGET_ID,
                        widgetID);
                setResult(RESULT_OK, resultValue);
                finish();
            });
        }
    }
}