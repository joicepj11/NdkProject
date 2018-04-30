package com.android.joice.ndkproject;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.android.joice.ndkproject.exception.NotExistingKeyException;
import com.android.joice.ndkproject.nativecode.Color;
import com.android.joice.ndkproject.exception.InvalidTypeException;
import com.android.joice.ndkproject.nativecode.Store;
import com.android.joice.ndkproject.nativecode.StoreListener;
import com.android.joice.ndkproject.nativecode.StoreThreadSafe;
import com.android.joice.ndkproject.nativecode.StoreType;

public class MainActivity extends AppCompatActivity implements View.OnClickListener , StoreListener {
    StoreType type;
    //Store store;
    EditText mType;
    EditText mKey;
    EditText mValue;
    Button get , set;
    String TAG = MainActivity.this.getClass().getSimpleName();
    private StoreThreadSafe store = new StoreThreadSafe(this);
    private long mWatcher;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mWatcher = store.startWatcher();
        initializeViews();
    }

    private void initializeViews() {
        mType = findViewById(R.id.type);
        mKey = findViewById(R.id.key);
        mValue = findViewById(R.id.value);
        findViewById(R.id.set).setOnClickListener(this);
        findViewById(R.id.get).setOnClickListener(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
      //  mType.setText(String.valueOf(store.getCount()));
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        store.stopWatcher(mWatcher);
    }

    private void onGetValue(String key) throws NotExistingKeyException, InvalidTypeException {
        switch (type) {
            case Integer:
                mKey.setText(Integer.toString(store.getInteger(key)));
                break;
            case String:
                mKey.setText(store.getString(key));
                break;
            case Color:
                mKey.setText(store.getColor(key).toString());
                break;
        }
    }

    private void onSetValue(String key , String value) {
        try {
            switch (type) {
                case Integer:
                    store.setInteger(key, Integer.parseInt(value));
                    break;
                case String:
                    store.setString(key, value);
                    break;
                case Color:
                    store.setColor(key, new Color(value));
                    break;
            }
        } catch (Exception eException) {
            displayMessage(eException.getMessage());
            Log.d(TAG, "onSetValue: "+eException);
        }
    }

    @Override
    public void onClick(View v) {

        switch (v.getId()) {
            case R.id.get:
                storeType();
                try {
                    onGetValue(mKey.getText().toString());
                } catch (NotExistingKeyException e) {
                    e.printStackTrace();
                } catch (InvalidTypeException e) {
                    e.printStackTrace();
                }
                break;

            case R.id.set:
                storeType();
                onSetValue(mKey.getText().toString() , mValue.getText().toString());
                break;
        }
    }

    private void storeType() {

        if(mType.getText().toString().contains("c")){
            type = StoreType.Color;
        }else if(mType.getText().toString().contains("s")){
            type =StoreType.String;
        }else if(mType.getText().toString().contains("i")){
            type =StoreType.Integer;
        }
    }

    private void displayMessage(String message) {
        Toast.makeText(getApplicationContext(),message,Toast.LENGTH_LONG).show();
    }

    @Override
    public void onSuccess(int pValue) {
        displayMessage(String.format("Integer '%1$d' successfuly saved!", pValue));
    }

    @Override
    public void onSuccess(String pValue) {
        displayMessage(String.format("String '%1$s' successfuly saved!", pValue));
    }

    @Override
    public void onSuccess(Color pValue) {
        displayMessage(String.format("Color '%1$d' successfuly saved!", pValue));
    }

}
