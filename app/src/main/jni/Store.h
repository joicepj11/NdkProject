//
// Created by joice on 2/4/18.
//

#ifndef _STORE_H_
#define _STORE_H_

#include <cstdint>
#include <pthread.h>
#define STORE_MAX_CAPACITY 16

typedef enum {
    StoreType_Integer,
    StoreType_String,
    StoreType_Color
} StoreType;

typedef union {
    int32_t mInteger;
    char*   mString;
    jobject mColor;
} StoreValue;

typedef struct {
    char* mKey;
    StoreType mType;
    StoreValue mValue;
} StoreEntry;

typedef struct {
    StoreEntry mEntries[STORE_MAX_CAPACITY];
    int32_t mLength;
} Store;

typedef struct {
Store* mStore;
JavaVM* mJavaVM;
jobject mLock;
pthread_t mThread;
int32_t mRunning;
} StoreWatcher;

void throwInvalidTypeException(JNIEnv* pEnv);
void throwNotExistingKeyException(JNIEnv* pEnv);
void throwStoreFullException(JNIEnv* pEnv);

StoreWatcher* startWatcher(JavaVM* pJavaVM, Store* pStore, jobject pLock);
void stopWatcher(StoreWatcher* pWatcher);
void* runWatcher(void* pArgs);
void processEntry(StoreEntry* pEntry);

#endif //_STORE_H_
