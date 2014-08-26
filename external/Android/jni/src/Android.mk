LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/$(SDL_PATH)/include

# Add your application source files here...
LOCAL_SRC_FILES := \
op_bgload.cpp \
op_choice.cpp \
op_cleartext.cpp \
op_delay.cpp \
op_fi.cpp \
op_goto.cpp \
op_gsetvar.cpp \
op_if.cpp \
op_jump.cpp \
op_music.cpp \
op_random.cpp \
op_setimg.cpp \
op_setvar.cpp \
op_sound.cpp \
op_text.cpp \
Data.cpp \
Loop.cpp \
Parse.cpp \
VNDC.cpp \
AudioManager.cpp \
ContextManager.cpp \
TextManager.cpp \
UDisplayable.cpp




LOCAL_SHARED_LIBRARIES := SDL2 SDL2_mixer SDL2_ttf SDL2_image

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
