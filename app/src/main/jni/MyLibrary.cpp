//
// Created by joice on 31/3/18.
//

#include"com_android_joice_ndkproject_MyNdk.h"

JNIEXPORT jstring JNICALL
Java_com_android_joice_ndkproject_MyNdk_getMyString(JNIEnv *env, jobject jobj){
  return env->NewStringUTF("From MyLibrary ");
}