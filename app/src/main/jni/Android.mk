LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := MyLibrary

LOCAL_SRC_FILES := MyLibrary.cpp

include $(CLEAR_VARS)

LOCAL_MODULE := store

LOCAL_SRC_FILES := store.cpp

include $(BUILD_SHARED_LIBRARY)