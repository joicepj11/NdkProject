/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_android_joice_ndkproject_nativecode_Store */

#ifndef _Included_com_android_joice_ndkproject_nativecode_Store
#define _Included_com_android_joice_ndkproject_nativecode_Store
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    getCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_android_joice_ndkproject_nativecode_Store_getCount
  (JNIEnv *, jobject);

/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    getString
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_android_joice_ndkproject_nativecode_Store_getString
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    setString
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_android_joice_ndkproject_nativecode_Store_setString
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    getInteger
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_android_joice_ndkproject_nativecode_Store_getInteger
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    setInteger
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_android_joice_ndkproject_nativecode_Store_setInteger
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    getColor
 * Signature: (Ljava/lang/String;)Lcom/android/joice/ndkproject/nativecode/Color;
 */
JNIEXPORT jobject JNICALL Java_com_android_joice_ndkproject_nativecode_Store_getColor
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    setColor
 * Signature: (Ljava/lang/String;Lcom/android/joice/ndkproject/nativecode/Color;)V
 */
JNIEXPORT void JNICALL Java_com_android_joice_ndkproject_nativecode_Store_setColor
  (JNIEnv *, jobject, jstring, jobject);

/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    startWatcher
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_android_joice_ndkproject_nativecode_Store_startWatcher
  (JNIEnv *, jobject);

/*
 * Class:     com_android_joice_ndkproject_nativecode_Store
 * Method:    stopWatcher
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_android_joice_ndkproject_nativecode_Store_stopWatcher
  (JNIEnv *, jobject, jlong);

#ifdef __cplusplus
}
#endif
#endif
