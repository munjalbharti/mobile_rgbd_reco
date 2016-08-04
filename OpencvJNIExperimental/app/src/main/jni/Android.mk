LOCAL_PATH := $(call my-dir)
#Put ur prebuilt librarie here
PREBUILT_LIB_PATH := $(LOCAL_PATH)/../myLibraries/

#Directory containing Android.mk

$(info LOCAL_PATH is $(LOCAL_PATH) )

include $(CLEAR_VARS)
LOCAL_MODULE    := sift_prebuilt
LOCAL_SRC_FILES := $(PREBUILT_LIB_PATH)/$(TARGET_ARCH_ABI)/libnonfree.so
#No special includes .the same as opencv will be used..have already copied xfeature2d there
#LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)

OPENCVROOT := ~/OpenCV-android-sdk

# OpenCV
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on

OPENCV_LIB_TYPE:=static
#non free includes will also be included by this line
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk


#for eigen
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/


LOCAL_SHARED_LIBRARIES := sift_prebuilt
$(info $(LOCAL_C_INCLUDES))
LOCAL_LDLIBS += -llog

$(info  Pre builtLibraries  are $(LOCAL_STATIC_LIBRARIES) ###)
$(info LOCAL_LDLIBS are $(LOCAL_LDLIBS) ###)


LOCAL_MODULE := wow
LOCAL_SRC_FILES := ProcessImage.cpp
include $(BUILD_SHARED_LIBRARY)


