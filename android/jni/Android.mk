#
#  Android.mk
#  Android project makefile.
#
#  Copyright (c) 2016 Waync Cheng.
#  All Rights Reserved.
#
#  2016/5/21 Waync created
#

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libcgo101
LOCAL_CFLAGS    := -Werror -fexceptions
LOCAL_SRC_FILES := cgo101.cpp

include $(BUILD_SHARED_LIBRARY)
