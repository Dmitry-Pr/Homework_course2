package com.example.antiprocrastination;

import static java.security.AccessController.getContext;

import android.content.SharedPreferences;
import android.os.Bundle;
import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;
import androidx.preference.Preference;
import androidx.preference.PreferenceFragmentCompat;
import androidx.preference.PreferenceManager;

public class SettingsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.settings_activity);

        if (savedInstanceState == null) {
            getSupportFragmentManager()
                    .beginTransaction()
                    .replace(R.id.settings, new SettingsFragment())
                    .commit();
        }
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
        }
    }

    public static class SettingsFragment extends PreferenceFragmentCompat {
        @Override
        public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
            setPreferencesFromResource(R.xml.root_preferences, rootKey);

            Preference deleteRecord = findPreference("delete_record");
            if (deleteRecord != null) {
                deleteRecord.setOnPreferenceClickListener(preference -> {
                    new AlertDialog.Builder(requireContext())
                            .setTitle(R.string.delete_record_title)
                            .setMessage(R.string.delete_record_confirmation)
                            .setPositiveButton(R.string.ok, (dialog, which) -> {
                                // Удаление значения рекорда
                                SharedPreferences sharedPreferences = requireContext().getSharedPreferences("appPreferences", MODE_PRIVATE);
                                SharedPreferences.Editor editor = sharedPreferences.edit();
                                editor.remove("recordTime");
                                editor.apply();
                            })
                            .setNegativeButton(R.string.cancel, null)
                            .show();
                    return true;
                });
            }
        }
    }
}