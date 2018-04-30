//
// Created by joice on 2/4/18.
//
#include"com_android_joice_ndkproject_nativecode_Store.h"
#include "Store.h"
#include <cstdlib>
#include <cstring>
#include<cstdio>
#include<unistd.h>

void releaseEntryValue(JNIEnv* pEnv, StoreEntry* pEntry);
static Store gStore;
static jobject gLock;

static jmethodID MethodOnSuccessInt;
static jmethodID MethodOnSuccessString;
static jmethodID MethodOnSuccessColor;

JNIEXPORT jint JNI_OnLoad(JavaVM* pVM, void* reserved) {
// Store initialization.'
    JNIEnv *env;
    if (pVM->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK) {
        abort();
    }
    // Caches methods.
    jclass StoreClass = env->FindClass("com/android/joice/ndkproject/nativecode/Store");

    if (StoreClass == NULL){
        abort();
    }

    if ((MethodOnSuccessInt = env->GetMethodID(StoreClass, "onSuccess","(I)V")) == NULL){
        abort();
    }

    if (( MethodOnSuccessString = env->GetMethodID(StoreClass, "onSuccess","(Ljava/lang/String;)V")) == NULL){
        abort();
    }

    if ((MethodOnSuccessColor = env->GetMethodID(StoreClass, "onSuccess","(Lcom/android/joice/ndkproject/nativecode/Color;)V")) == NULL){
        abort();
    }

    env->DeleteLocalRef(StoreClass);
    gStore.mLength = 0;

    jclass ObjectClass = env->FindClass("java/lang/Object");
    if (ObjectClass == NULL) {
        abort();
    }

    jmethodID ObjectConstructor = env->GetMethodID(ObjectClass,"<init>", "()V");

    if (ObjectConstructor == NULL){
        abort();
    }

    jobject lockTmp = env->NewObject(ObjectClass, ObjectConstructor);

    env->DeleteLocalRef(ObjectClass);
    gLock = env->NewGlobalRef(lockTmp);
    env->DeleteLocalRef(lockTmp);

    jclass StoreThreadSafeClass = env->FindClass("com/android/joice/ndkproject/nativecode/StoreThreadSafe");
    if (StoreThreadSafeClass == NULL) {
        abort();
    }

    jfieldID lockField = env->GetStaticFieldID(StoreThreadSafeClass,"LOCK", "Ljava/lang/Object;");
    if (lockField == NULL){
        abort();
    }

    env->SetStaticObjectField(StoreThreadSafeClass, lockField, gLock);
    env->DeleteLocalRef(StoreThreadSafeClass);

    return JNI_VERSION_1_6;
}

JNIEXPORT jint JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_getCount(JNIEnv *pEnv, jobject jobj){
     return gStore.mLength;
}

bool isEntryValid(JNIEnv* pEnv, StoreEntry* pEntry, StoreType pType) {
   if (pEntry == NULL) {
   throwNotExistingKeyException(pEnv);
   } else if (pEntry->mType != pType) {
   throwInvalidTypeException(pEnv);
   }
   return !pEnv->ExceptionCheck();
}

StoreEntry* findEntry(JNIEnv* pEnv, Store* pStore, jstring pKey) {
    StoreEntry* entry = pStore->mEntries;
    StoreEntry* entryEnd = entry + pStore->mLength;//adding length so that pointer points last entry
    // Compare requested key with every entry key currently stored
    // until we find a matching one.
    const char* tmpKey = pEnv->GetStringUTFChars(pKey, NULL);

    while ((entry < entryEnd) && (strcmp(entry->mKey, tmpKey) != 0)) {
    ++entry; //incrementing pointer to next address
    }
    pEnv->ReleaseStringUTFChars(pKey, tmpKey);
    return (entry == entryEnd) ? NULL : entry;
}

StoreEntry* allocateEntry(JNIEnv* pEnv, Store* pStore, jstring pKey) {
    // If entry already exists in the store, releases its content
    // and keep its key.
    StoreEntry* entry = findEntry(pEnv, pStore, pKey);
    if (entry != NULL) {
        releaseEntryValue(pEnv, entry);
    }
    // If entry does not exist, create a new entry
    // right after the entries already stored.
    else {

        if (pStore->mLength >= STORE_MAX_CAPACITY) {
            throwStoreFullException(pEnv);
            return NULL;
        }
        entry = pStore->mEntries + pStore->mLength; //adding base address + length of Store
        // Copies the new key into its final C string buffer.
        const char* tmpKey = pEnv->GetStringUTFChars(pKey, NULL);
        entry->mKey = new char[strlen(tmpKey) + 1];
        strcpy(entry->mKey, tmpKey);
        pEnv->ReleaseStringUTFChars(pKey, tmpKey);
        ++pStore->mLength;//incrementing the length of Store struct
    }
    return entry;
}

void releaseEntryValue(JNIEnv* pEnv, StoreEntry* pEntry) {
    switch (pEntry->mType) {
        case StoreType_String:
            delete pEntry->mValue.mString;
        break;
        case StoreType_Color:
        // Unreferences the object for garbage collection.
        pEnv->DeleteGlobalRef(pEntry->mValue.mColor);
        break;
        case StoreType_Integer:
        printf("nothing to release to integer type");
        break;
    }
}

JNIEXPORT jstring JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_getString(JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_String)) {
        // Converts a C string into a Java String.
        return pEnv->NewStringUTF(entry->mValue.mString);
    } else {
        return NULL;
    }
}

JNIEXPORT void JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_setString(JNIEnv* pEnv, jobject pThis, jstring pKey, jstring pString) {
// Turns the Java string into a temporary C string.
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL) {
        entry->mType = StoreType_String;
        // Copy the temporary C string into its dynamically allocated
        // final location. Then releases the temporary string.
        jsize stringLength = pEnv->GetStringUTFLength(pString);
        entry->mValue.mString = new char[stringLength + 1];
        // Directly copies the Java String into our new C buffer.
        pEnv->GetStringUTFRegion(pString, 0, stringLength,entry->mValue.mString);
        // Append the null character for string termination.
        entry->mValue.mString[stringLength] = '\0';
        pEnv->CallVoidMethod(pThis, MethodOnSuccessString, (jstring) pEnv->NewStringUTF(entry->mValue.mString));
    }
}

JNIEXPORT jint JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_getInteger(JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_Integer)) {
        return entry->mValue.mInteger;
    } else {
        return 0;
    }
}

JNIEXPORT void JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_setInteger(JNIEnv* pEnv, jobject pThis, jstring pKey, jint pInteger) {
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL) {
        entry->mType = StoreType_Integer;
        entry->mValue.mInteger = pInteger;
        //callback for store listener
        pEnv->CallVoidMethod(pThis, MethodOnSuccessInt, (jint) entry->mValue.mInteger);
    }
}

JNIEXPORT jobject JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_getColor(JNIEnv* pEnv, jobject pThis, jstring pKey) {
    StoreEntry* entry = findEntry(pEnv, &gStore, pKey);
    if (isEntryValid(pEnv, entry, StoreType_Color)) {
        return entry->mValue.mColor;
    } else {
        return NULL;
    }
}


JNIEXPORT void JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_setColor(JNIEnv* pEnv, jobject pThis, jstring pKey, jobject pColor) {
// Save the Color reference in the store.
    StoreEntry* entry = allocateEntry(pEnv, &gStore, pKey);
    if (entry != NULL) {
        entry->mType = StoreType_Color;
        // The Java Color is going to be stored on the native side.
        // Need to keep a global reference to avoid a potential
        // garbage collection after method returns.
        entry->mValue.mColor = pEnv->NewGlobalRef(pColor);

        pEnv->CallVoidMethod(pThis, MethodOnSuccessColor, (jstring) entry->mValue.mColor);
    }
}

void throwNotExistingKeyException(JNIEnv* pEnv) {
    jclass clazz = pEnv->FindClass("com/android/joice/ndkproject/exception/NotExistingKeyException");
    if (clazz != NULL) {
        pEnv->ThrowNew(clazz, "Key does not exist.");
    }
    pEnv->DeleteLocalRef(clazz);
}

void throwInvalidTypeException(JNIEnv* pEnv){
    jclass clazz = pEnv->FindClass("com/android/joice/ndkproject/exception/InvalidTypeException");
    if (clazz != NULL) {
        pEnv->ThrowNew(clazz, "Invalid type.");
    }
    pEnv->DeleteLocalRef(clazz);
}

void throwStoreFullException(JNIEnv* pEnv){
jclass clazz = pEnv->FindClass("com/android/joice/ndkproject/exception/StoreFullException");
    if (clazz != NULL) {
        pEnv->ThrowNew(clazz, "Invalid type.");
    }
    pEnv->DeleteLocalRef(clazz);
}

JNIEXPORT jlong JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_startWatcher(JNIEnv *pEnv, jobject jobj){
    JavaVM* javaVM;
// Caches the VM.
    if (pEnv->GetJavaVM(&javaVM) != JNI_OK){
        abort();
    }
// Launches the background thread.
    StoreWatcher* watcher = startWatcher(javaVM, &gStore, gLock);

    return (jlong) watcher;
 }

JNIEXPORT void JNICALL
Java_com_android_joice_ndkproject_nativecode_Store_stopWatcher(JNIEnv *pEnv, jobject pThis, jlong pWatcher){
    stopWatcher((StoreWatcher*) pWatcher);
}

StoreWatcher* startWatcher(JavaVM* pJavaVM, Store* pStore, jobject pLock) {
    StoreWatcher* watcher = new StoreWatcher();
    watcher->mJavaVM = pJavaVM;
    watcher->mStore = pStore;
    watcher->mLock = pLock;
    watcher->mRunning = true;
    pthread_attr_t lAttributes;
    if (pthread_attr_init(&lAttributes)) {
        abort();
    }
                       //thread-id        //attr       //fun ptr //parameters
    if (pthread_create(&watcher->mThread, &lAttributes,runWatcher, watcher)) {
        abort();
    }
    return watcher;
}

void stopWatcher(StoreWatcher* pWatcher) {
    pWatcher->mRunning = false;
}

void* runWatcher(void* pArgs) {
    StoreWatcher* watcher = (StoreWatcher*) pArgs;
    Store* store = watcher->mStore;
    JavaVM* javaVM = watcher->mJavaVM;
    JavaVMAttachArgs javaVMAttachArgs;
    javaVMAttachArgs.version = JNI_VERSION_1_6;
    javaVMAttachArgs.name = "NativeThread";
    javaVMAttachArgs.group = NULL;
    JNIEnv* env;

    if (javaVM->AttachCurrentThreadAsDaemon(&env, &javaVMAttachArgs) != JNI_OK){
        abort();
    }
    // Runs the thread loop.
    while (true) {
    sleep(3); // In seconds.
    env->MonitorEnter(watcher->mLock);
    if (!watcher->mRunning) break;
    StoreEntry* entry = watcher->mStore->mEntries;
    StoreEntry* entryEnd = entry + watcher->mStore->mLength;
    while (entry < entryEnd) {
    processEntry(entry);
    ++entry;
    }
    // Critical section end.
    env->MonitorExit(watcher->mLock);
    }
    javaVM->DetachCurrentThread();
    delete watcher;
    pthread_exit(NULL);
}

void processEntry(StoreEntry* pEntry) {
    switch (pEntry->mType) {
        case StoreType_Integer:
            if (pEntry->mValue.mInteger > 100000) {
                pEntry->mValue.mInteger = 100000;
            } else if (pEntry->mValue.mInteger < -100000) {
                pEntry->mValue.mInteger = -100000;
            }
        break;
        default:

    }
}