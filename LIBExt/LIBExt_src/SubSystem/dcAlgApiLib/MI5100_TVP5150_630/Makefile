
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = Sensor_MI5100_TVP5150_630

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
	ae_MI5100_TVP5150_630.c \
	awb_MI5100_TVP5150_630.c \
	flashlight_MI5100_TVP5150_630.c \
	dzoom_MI5100_TVP5150_630.c \
	IQSetting_MI5100_TVP5150_630.c \
	SensorSys_MI5100_TVP5150_630.c \
	isr_MI5100_TVP5150_630.c \
	MI5100.c \
	TVP5150.c \
	SensorFlowFunc_MI5100_TVP5150_630.c \
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
CFLAGS_D = -Otime -DMI5100_TVP5150_630=1
ASMFLAGS_D =

CFLAGS_R = -Otime -DMI5100_TVP5150_630=1
ASMFLAGS_R =

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt


