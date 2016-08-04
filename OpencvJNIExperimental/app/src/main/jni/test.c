//
// Created by munjalbharti on 07.03.16.
//

#include "test.h"

#include <string.h>
#include <jni.h>



jstring Java_exp_com_tum_opencvjniexperimental_MainActivity_invokeNativeFunction(JNIEnv* env, jobject javaThis) {
    return (*env)->NewStringUTF(env, "Hello from native code!");
}