package com.example.hw5.contacts;

import android.graphics.Bitmap;

public class Contact {
    private String name;
    private Bitmap image;

    public Contact(String name, Bitmap image) {
        this.name = name;
        this.image = image;
    }

    public String getName() {
        return name;
    }

    public Bitmap getImage() {
        return image;
    }
}
