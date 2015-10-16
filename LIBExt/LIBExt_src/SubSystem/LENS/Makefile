
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = Lens

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
	LensAPI.c \
	LensCmd.c \
	LensCtrlTsk.c \
	LensDrv/LensDrv_FF.c \
	LensDrv/LensDrv_DUMMY.c \
	LensDrv/LensDrv_DZ5311.c \
	LensMotor/LensMotor_PSEUDO.c \
	LensMotor/LensMotor_R2A30423.c \
	LensMotor/LensMotor_DUMMY.c

ASM =

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
	../../../../Include \

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = ../../..

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D = -Otime
ASMFLAGS_D =

CFLAGS_R = -Otime
ASMFLAGS_R =

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
