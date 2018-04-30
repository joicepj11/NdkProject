package com.android.joice.ndkproject.nativecode;

import com.android.joice.ndkproject.exception.InvalidTypeException;
import com.android.joice.ndkproject.exception.NotExistingKeyException;

public class StoreThreadSafe extends Store {

    private static  Object LOCK = new Object();
    public StoreThreadSafe(StoreListener pListener) {
        super(pListener);
    }
    @Override
    public int getCount() {
        synchronized (LOCK) {
            return super.getCount();
        }
    }

    @Override
    public int getInteger(String pKey) throws NotExistingKeyException,InvalidTypeException
    {
        synchronized (LOCK) {
            return super.getInteger(pKey);
        }
    }
    @Override
    public void setInteger(String pKey, int pInt) {
        synchronized (LOCK) {
            super.setInteger(pKey, pInt);
        }
    }
    @Override
    public String getString(String pKey) throws NotExistingKeyException,InvalidTypeException
    {
        synchronized (LOCK) {
            return super.getString(pKey);
        }
    }
    @Override
    public void setString(String pKey, String pString) {
        synchronized (LOCK) {
            super.setString(pKey, pString);
        }
    }

    @Override
    public Color getColor(String pKey) throws NotExistingKeyException,InvalidTypeException
    {
        synchronized (LOCK) {
            return super.getColor(pKey);
        }
    }
    @Override
    public void setColor(String pKey, Color pColor) {
        synchronized (LOCK) {
            super.setColor(pKey, pColor);
        }
    }

    @Override
    public void stopWatcher(long pPointer) {
        synchronized (LOCK) {
            super.stopWatcher(pPointer);
        }
    }

}
