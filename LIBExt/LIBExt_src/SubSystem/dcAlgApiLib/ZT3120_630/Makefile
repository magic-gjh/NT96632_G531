
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = Sensor_ZT3120_630

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
	ae_ZT3120_630.c \
	awb_ZT3120_630.c \
	flashlight_ZT3120_630.c \
	dzoom_ZT3120_630.c \
	IQSetting_ZT3120_630.c \
	SensorSys_ZT3120_630.c \
	isr_ZT3120_630.c \
	ZT3120.c \
	SensorFlowFunc_ZT3120_630.c \
	../IPPManager.c \
	../sen_cmd.c \
	../ImageAlgInfor.c \
	../sen_common.c \
	../Cal_Api.c \
	../IPPBuffer.c \
	../DataStamp.c \
	../AlbumShot.c \
	../cap_common.c \
	../CoupleShot.c \
	../PreShot.c \
	../prv_common.c \
	../Alg_IPLMode.c\
	../dis_common.c\
	../sen_snr.c\

ASM =

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = .. \
	../../../../../Include \

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = ../../../..

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D = -Otime -DZT3120_630=1
ASMFLAGS_D =

CFLAGS_R = -Otime -DZT3120_630=1
ASMFLAGS_R =

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt


