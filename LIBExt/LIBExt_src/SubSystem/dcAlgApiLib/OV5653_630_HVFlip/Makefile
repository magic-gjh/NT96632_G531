
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = Sensor_OV5653_630_HVFlip

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
	../OV5653_630/ae_OV5653_630.c \
	../OV5653_630/awb_OV5653_630.c \
	../OV5653_630/flashlight_OV5653_630.c \
	../OV5653_630/dzoom_OV5653_630.c \
	../OV5653_630/IQSetting_OV5653_630.c \
	../OV5653_630/SensorSys_OV5653_630.c \
	../OV5653_630/isr_OV5653_630.c \
	../OV5653_630/OV5653.c \
	../OV5653_630/SensorFlowFunc_OV5653_630.c \
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
CFLAGS_D = -Otime -DPanasonic_10M_630=0 -DMI1030_630=0 -DMI5100_630=0 -DOV5653_630=1 -DOV5653_630_HVFlip=1
ASMFLAGS_D =

CFLAGS_R = -Otime -DPanasonic_10M_630=0 -DMI1030_630=0 -DMI5100_630=0 -DOV5653_630=1 -DOV5653_630_HVFlip=1
ASMFLAGS_R =

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt


