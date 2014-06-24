################################################################################
#
################################################################################

LOCAL_PATH := $(call my-dir)

################################################################################
#
################################################################################
include $(CLEAR_VARS)

#-----------------------------------------------------------
#$(call config-custom-folder,hal:hal)

#-----------------------------------------------------------
LOCAL_SRC_FILES += $(call all-c-cpp-files-under, ../hal/cam_cal)

#-----------------------------------------------------------
#LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_CUSTOM)/kernel/cam_cal/inc

#-----------------------------------------------------------
LOCAL_STATIC_LIBRARIES += 
#
LOCAL_WHOLE_STATIC_LIBRARIES += 

#-----------------------------------------------------------
LOCAL_MODULE := libcameracustom.cam_cal

#-----------------------------------------------------------
include $(BUILD_STATIC_LIBRARY) 

################################################################################
#
################################################################################
include $(CLEAR_VARS)
include $(call all-makefiles-under,$(LOCAL_PATH))

