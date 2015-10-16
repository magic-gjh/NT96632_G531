
enum _PLB_TASK_STATE {
    PLB_ST_IDLE = 0,        // idle mode
    PLB_ST_MENU,            // menu mode
    PLB_ST_FULL,            // full image mode
    PLB_ST_THUMB,           // thumbnail mode
    PLB_ST_PLAY_MOV,        // play movie
    PLB_ST_PAUSE_MOV,       // pause movie
    PLB_ST_FWD_MOV,         // forward movie
    PLB_ST_BWD_MOV,         // backward movie
    PLB_ST_SLOW_MOV,        // play movie with slow motion
    PLB_ST_PLAY_WAV,        // play wav
    PLB_ST_PAUSE_WAV,       // pause wav
    PLB_ST_FWD_WAV,         // forward wav
    PLB_ST_BWD_WAV,         // backward wav
    PLB_ST_ZOOM,            // zoom mode
    PLB_ST_ZOOM_MOVE,            // zoom mode
    PLB_ST_ERASE,           // erase menu mode
    PLB_ST_SLIDE_SHOW,       // slide show mode
    PLB_ST_QDMENU, //quick delete mode
    PLB_ST_MENU_AVI,        //AVI menu mode
};

typedef struct _PLB_TASK_DATA {
    UINT8   State;          // current state
    UINT32  err;            // error status
    UINT16  TimerCount;     // timer count
    UINT16  VolCount;       // volume bar display count
    BOOL    bVolDraw;       // volume bar draw flag
} PLB_TASK_DATA;

extern PLB_TASK_DATA g_PlbData;

extern void FlowPB_SliderCBFunc(void);

extern void PlayFlowMoviePlayCBFunc(UINT32 event_id);

