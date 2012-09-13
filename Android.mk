LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := atlas
LOCAL_SRC_FILES := libatlas.so
LOCAL_EXPORT_C_INCLUDES := \
   $(LOCAL_PATH)/communication \
   $(LOCAL_PATH)/container \
   $(LOCAL_PATH)/foundation \
   $(LOCAL_PATH)/math \
   $(LOCAL_PATH)/os \
   $(LOCAL_PATH)/util \
   $(LOCAL_PATH)/xml \

LOCAL_EXPORT_CFLAGS := -DATLAS_SYM=IMPORT
LOCAL_EXPORT_CPPFLAGS := -DATLAS_SYM=IMPORT

LOCAL_SHARED_LIBRARIES := glob-shared
LOCAL_STATIC_LIBRARIES := libxml2-static

include $(PREBUILT_SHARED_LIBRARY)

$(call import-module, libxml2-2.7.8)
$(call import-module, glob)

