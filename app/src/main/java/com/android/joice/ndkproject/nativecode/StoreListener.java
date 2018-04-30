package com.android.joice.ndkproject.nativecode;

import com.android.joice.ndkproject.nativecode.Color;

public interface StoreListener {
    void onSuccess(int pValue);
    void onSuccess(String pValue);
    void onSuccess(Color pValue);
}
