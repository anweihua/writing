APP_STL := gnustl_static
LOCAL_CFLAGS :=-D__GXX_EXPERIMENTAL_CXX0X__  <--important
LOCAL_CPPFLAGS  := -std=c++11
APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -DCOCOS2D_DEBUG=1
APP_CFLAGS += -Wno-error=format-security
APP_PLATFORM := android-19