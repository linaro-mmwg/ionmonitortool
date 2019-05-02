LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := ion-monitor-tool

LOCAL_SRC_FILES := ion-monitor-tool.c parse.c exoplayer.c utils.c
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_MODULE_TAGS := optional

LOCAL_PROPRIETARY_MODULE := true
LOCAL_LDFLAGS := -Wl,--no-wchar-size-warning
LOCAL_CFLAGS :=  "-mfloat-abi=hard"
LOCAL_CPPFLAGS :=  "-mfloat-abi=hard"
LOCAL_LDFLAGS += -Wl,--no-warn-mismatch

LOCAL_MODULE_TARGET_ARCH = $(TARGET_ARCH)

include $(BUILD_EXECUTABLE)
