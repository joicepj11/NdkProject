package com.android.joice.ndkproject.nativecode;


import com.android.joice.ndkproject.exception.InvalidTypeException;
import com.android.joice.ndkproject.exception.NotExistingKeyException;

public class Store  implements StoreListener {

    StoreListener mListener;

    public Store(StoreListener pListener) {
        mListener = pListener;
    }
    public native int getCount();
    public native String getString(String pKey) throws NotExistingKeyException, InvalidTypeException;
    public native void setString(String pKey, String pString);
    public native int getInteger(String pKey) throws InvalidTypeException, NotExistingKeyException;
    public native void setInteger(String pKey, int pInt);
    public native Color getColor(String pKey) throws NotExistingKeyException, InvalidTypeException;
    public native void setColor(String pKey, Color pColor);
    public native long startWatcher();
    public native void stopWatcher(long pPointer);

    @Override
    public void onSuccess(int pValue) {
        mListener.onSuccess(pValue);
    }

    @Override
    public void onSuccess(String pValue) {
        mListener.onSuccess(pValue);
    }

    @Override
    public void onSuccess(Color pValue) {
        mListener.onSuccess(pValue);
    }

    static {
        System.loadLibrary("store");

    }
}
