/**
    Header file for CEC Task

    Header file for CEC Task.

    @file       cec.h
    @ingroup    mIDISPHDMITx
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef  _CEC_H
#define  _CEC_H


/**
    CEC Remote Control Pass Through Feature Command Code

    CEC User Control Codes Define
*/
//@{
typedef enum
{
    CECKEY_SELECT       =0x00,  ///< CEC Key 0x00: Select
    CECKEY_UP,                  ///< CEC Key 0x01: Up
    CECKEY_DOWN,                ///< CEC Key 0x02: Down
    CECKEY_LEFT,                ///< CEC Key 0x03: Left
    CECKEY_RIGHT,               ///< CEC Key 0x04: Right
    CECKEY_RIGHTUP,             ///< CEC Key 0x05: Right-Up
    CECKEY_RIGHTDOWN,           ///< CEC Key 0x06: Right-Down
    CECKEY_LEFTUP,              ///< CEC Key 0x07: Left-Up
    CECKEY_LEFTDOWN,            ///< CEC Key 0x08: Left-Down
    CECKEY_ROOT_MENU,           ///< CEC Key 0x09: Root Menu
    CECKEY_SETUP_MENU,          ///< CEC Key 0x0A: Setup Menu
    CECKEY_CONTENT_MENU,        ///< CEC Key 0x0B: Content Menu
    CECKEY_FAVORITE_MENU,       ///< CEC Key 0x0C: Favorite Menu
    CECKEY_EXIT,                ///< CEC Key 0x0D: Exit
    CECKEY_MEDIA_TOPMENU=0x10,  ///< CEC Key 0x10: Media Top Menu
    CECKEY_MEDIA_CONTEXT,       ///< CEC Key 0x11: Media Context - Sensitive Menu
    CECKEY_NUMBERENTRYMODE=0x1D,///< CEC Key 0x1D: Number Entry Mode
    CECKEY_NUMBER_11,           ///< CEC Key 0x1E: Number 11
    CECKEY_NUMBER_12,           ///< CEC Key 0x1F: Number 12
    CECKEY_NUMBER_0,            ///< CEC Key 0x20: Number 0 or Number 10
    CECKEY_NUMBER_1,            ///< CEC Key 0x21: Number 1
    CECKEY_NUMBER_2,            ///< CEC Key 0x22: Number 2
    CECKEY_NUMBER_3,            ///< CEC Key 0x23: Number 3
    CECKEY_NUMBER_4,            ///< CEC Key 0x24: Number 4
    CECKEY_NUMBER_5,            ///< CEC Key 0x25: Number 5
    CECKEY_NUMBER_6,            ///< CEC Key 0x26: Number 6
    CECKEY_NUMBER_7,            ///< CEC Key 0x27: Number 7
    CECKEY_NUMBER_8,            ///< CEC Key 0x28: Number 8
    CECKEY_NUMBER_9,            ///< CEC Key 0x29: Number 9
    CECKEY_DOT,                 ///< CEC Key 0x2A: Dot
    CECKEY_ENTER,               ///< CEC Key 0x2B: Enter
    CECKEY_CLEAR,               ///< CEC Key 0x2C: Clear
    CECKEY_NEXT_FAVORITE=0x2F,  ///< CEC Key 0x2F: Next Favorite
    CECKEY_CHANNEL_UP,          ///< CEC Key 0x30: Channel Up
    CECKEY_CHANNEL_DOWN,        ///< CEC Key 0x31: Channel Down
    CECKEY_PREVIOUS_CHANNEL,    ///< CEC Key 0x32: Previous Channel
    CECKEY_SOUND_SELECT,        ///< CEC Key 0x33: Sound Select
    CECKEY_INPUT_SELECT,        ///< CEC Key 0x34: Input Select
    CECKEY_DISPLAY_INFO,        ///< CEC Key 0x35: Display Information
    CECKEY_HELP,                ///< CEC Key 0x36: Help
    CECKEY_PAGE_UP,             ///< CEC Key 0x37: Page Up
    CECKEY_PAGE_DOWN,           ///< CEC Key 0x38: Page Down
    CECKEY_POWER=0x40,          ///< CEC Key 0x40: Power
    CECKEY_VOLUME_UP,           ///< CEC Key 0x41: Volume Up
    CECKEY_VOLUME_DOWN,         ///< CEC Key 0x42: Volume Down
    CECKEY_MUTE,                ///< CEC Key 0x43: Mute
    CECKEY_PLAY,                ///< CEC Key 0x44: Play
    CECKEY_STOP,                ///< CEC Key 0x45: Stop
    CECKEY_PAUSE,               ///< CEC Key 0x46: Pause
    CECKEY_RECORD,              ///< CEC Key 0x47: Record
    CECKEY_REWIND,              ///< CEC Key 0x48: Rewind
    CECKEY_FAST_FORWARD,        ///< CEC Key 0x49: Fast Forward
    CECKEY_EJECT,               ///< CEC Key 0x4A: Eject
    CECKEY_FORWARD,             ///< CEC Key 0x4B: Forward
    CECKEY_BACKWARD,            ///< CEC Key 0x4C: Backward
    CECKEY_STOP_RECORD,         ///< CEC Key 0x4D: Stop Record
    CECKEY_PAUSE_RECORD,        ///< CEC Key 0x4E: Pause Record
    CECKEY_ANGLE=0x50,          ///< CEC Key 0x50: Angle
    CECKEY_SUB_PICTURE,         ///< CEC Key 0x51: SUB-Picture
    CECKEY_VIDEO_ON_DEMAND,     ///< CEC Key 0x52: Video On Demand
    CECKEY_ELECTRO_PROG_GUIDE,  ///< CEC Key 0x53: Electronic Program Guide
    CECKEY_TIMER_PROGRAMMING,   ///< CEC Key 0x54: Timer Programming
    CECKEY_INITIAL_CONFIG,      ///< CEC Key 0x55: Initial Configuration
    CECKEY_SEL_BROADCAST_TYPE,  ///< CEC Key 0x56: Select Broadcast Type
    CECKEY_SEL_SOUND_PRESENT,   ///< CEC Key 0x57: Select Sound Presentation
    CECKEY_PLAY_FUNC=0x60,      ///< CEC Key 0x60: Play Function
    CECKEY_PAUSE_PLAY_FUNC,     ///< CEC Key 0x61: Pause Play Function
    CECKEY_RECORD_FUNC,         ///< CEC Key 0x62: Record Function
    CECKEY_PAUSE_RECORD_FUNC,   ///< CEC Key 0x63: Pause-Record Function
    CECKEY_STOP_FUNC,           ///< CEC Key 0x64: Stop Function
    CECKEY_MUTE_FUNC,           ///< CEC Key 0x65: Mute Function
    CECKEY_RESTORE_VOLUME_FUNC, ///< CEC Key 0x66: Restore Volume Function
    CECKEY_TUNE_FUNC,           ///< CEC Key 0x67: Tune Function
    CECKEY_SEL_MEDIA_FUNC,      ///< CEC Key 0x68: Select Media Function
    CECKEY_SEL_AV_IN_FUNC,      ///< CEC Key 0x69: Select AV Input Function
    CECKEY_SEL_AUDIO_IN_FUNC,   ///< CEC Key 0x6A: Select Audio Input Function
    CECKEY_POWER_TOGGLE_FUNC,   ///< CEC Key 0x6B: Power Toggle Function
    CECKEY_POWER_OFF_FUNC,      ///< CEC Key 0x6C: Power OFF Function
    CECKEY_POWER_ON_FUNC,       ///< CEC Key 0x6D: Power ON Function
    CECKEY_F1=0x71,             ///< CEC Key 0x71: F1(Blue)
    CECKEY_F2,                  ///< CEC Key 0x72: F2(Red)
    CECKEY_F3,                  ///< CEC Key 0x73: F3(Green)
    CECKEY_F4,                  ///< CEC Key 0x74: F4(Yellow)
    CECKEY_F5,                  ///< CEC Key 0x75: F5
    CECKEY_DATA,                ///< CEC Key 0x76: Data

    ENUM_DUMMY4WORD(CECKEY)
}CECKEY;
//@}

/**
    CEC Remote callback function prototype
*/
typedef ER (*CecRemoteKeyCB)(CECKEY Key, BOOL Press);

/**
    CEC Vendor callback function prototype
*/
typedef ER (*CecVendorKeyCB)(UINT32 Key, BOOL Press);




/**
    CEC Transmit command types

    CEC Transmit command types
*/
//@{
typedef enum
{
    //CECFEATURE_TASKOPEN_INIT,     ///< CEC Task Open Initial Flow, Internal Usage
    CECFEATURE_ALLOC_LOGADDR=0x01,  ///< Allocate Logical Address
    CECFEATURE_ONE_TOUCH_PLAY,      ///< One Touch Play
    CECFEATURE_GET_VENDORID,        ///< Get the Device/TV's Vendor ID
    CECFEATURE_INACTIVE_SOURCE,     ///< Notify the TV that the active source inactivate.

    //CECFEATURE_,       ///< CEC Feature:
    ENUM_DUMMY4WORD(CECFEATURE)
}CECFEATURE;
//@}



/**
    CEC Vendor ID

    CEC Vendor ID
*/
//@{
typedef enum
{
    CECVENDOR_NONE       = 0x000000,
    CECVENDOR_SONYBRAVIA = 0x080046,

    ENUM_DUMMY4WORD(CECVENDOR)
}CECVENDOR;
//@}


extern ER       cec_open(void);
extern ER       cec_close(void);
extern void     cec_setPhysicalAddr(UINT16 uiAddr);
extern UINT32   cec_getCecStatus(void);
extern ER       cec_applyFeature(CECFEATURE Feature, UINT32 uiParam);
extern void     cec_setDeviceVendorID(CECVENDOR uiVendorID);
extern void     cec_hookRemoteCallBack(CecRemoteKeyCB BasicCallBack, CecVendorKeyCB VendorCallBack);




#endif

