package com.android.joice.ndkproject.nativecode;

public class MyNdk {
    static {
        System.loadLibrary("MyLibrary");
    }
    public native String getMyString();
}
