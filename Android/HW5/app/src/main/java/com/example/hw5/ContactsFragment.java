package com.example.hw5;

import android.content.ContentResolver;
import android.content.ContentUris;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.provider.ContactsContract;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.Manifest;
import android.content.pm.PackageManager;

import androidx.core.app.ActivityCompat;
import androidx.fragment.app.Fragment;
import androidx.vectordrawable.graphics.drawable.VectorDrawableCompat;

import com.example.hw5.contacts.Contact;
import com.example.hw5.contacts.ContactAdapter;

import java.util.ArrayList;

public class ContactsFragment extends Fragment {

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_contacts, container, false);
        ListView contactsListView = view.findViewById(R.id.contacts_list_view);

        if (ActivityCompat.checkSelfPermission(getActivity(), Manifest.permission.READ_CONTACTS) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(getActivity(), new String[]{Manifest.permission.READ_CONTACTS}, 1);
        } else {
            loadContacts(contactsListView);
        }

        return view;
    }

    private void loadContacts(ListView contactsListView) {
        ArrayList<Contact> contacts = new ArrayList<>();
        ContentResolver contentResolver = getActivity().getContentResolver();
        Cursor cursor = contentResolver.query(ContactsContract.Contacts.CONTENT_URI, null, null, null, null);

        if (cursor != null && cursor.getCount() > 0) {
            while (cursor.moveToNext()) {
                int nameIndex = cursor.getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME);
                if (nameIndex == -1) {
                    continue;
                }
                String name = cursor.getString(nameIndex);

                Bitmap image = null;
                int contactIndex = cursor.getColumnIndex(ContactsContract.Contacts._ID);
                if (contactIndex == -1) {
                    continue;
                }
                String contactId = cursor.getString(contactIndex);
                Uri contactUri = ContentUris.withAppendedId(ContactsContract.Contacts.CONTENT_URI, Long.parseLong(contactId));
                Uri photoUri = Uri.withAppendedPath(contactUri, ContactsContract.Contacts.Photo.CONTENT_DIRECTORY);
                Cursor photoCursor = contentResolver.query(photoUri, new String[]{ContactsContract.Contacts.Photo.PHOTO}, null, null, null);
                if (photoCursor != null && photoCursor.moveToFirst()) {
                    int photoIndex = photoCursor.getColumnIndex(ContactsContract.Contacts.Photo.PHOTO);
                    if (photoIndex == -1) {
                        continue;
                    }
                    byte[] photoData = photoCursor.getBlob(photoIndex);
                    image = BitmapFactory.decodeByteArray(photoData, 0, photoData.length);
                    photoCursor.close();
                }
                if (image == null) {
                    Drawable vectorDrawable = VectorDrawableCompat.create(getResources(), R.drawable.baseline_person_24, null);
                    image = Bitmap.createBitmap(vectorDrawable.getIntrinsicWidth(), vectorDrawable.getIntrinsicHeight(), Bitmap.Config.ARGB_8888);
                    Canvas canvas = new Canvas(image);
                    vectorDrawable.setBounds(0, 0, canvas.getWidth(), canvas.getHeight());
                    vectorDrawable.draw(canvas);
                }
                contacts.add(new Contact(name, image));
            }
            cursor.close();
        }

        ContactAdapter adapter = new ContactAdapter(getActivity(), contacts);
        contactsListView.setAdapter(adapter);
    }
}