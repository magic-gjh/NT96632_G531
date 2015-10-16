/* ZT3010 initial script for OV9712 sensor */
   {0x00, 0x02},
   {0x00, 0x00},
   {0xA4, 0x00},
   {0xA2, 0x00},
   {0xA3, 0x19},
   {0xA8, 0x3F},
   {0x05, 0x0B},
   {0x04, 0x01},
   {0x06, 0x11},

   {0x00, 0x02},  //delay for detect

   {0x0C, 0x10},
   {0x0D, 0x00},
   {0x0E, 0x20},
   {0x0F, 0x00},

   {0x10, 0x30},
   {0x11, 0x00},
   {0x12, 0x40},
   {0x13, 0x08},
#if 0
//crop left
   // Sensor 0 Horizontal window
   {0x14, 0x97},  //start 0x197
   {0x15, 0x01},
   {0x16, 0x56},
   {0x17, 0x05},

// Sensor 0 Vertical window
   {0x18, 0x01},
   {0x19, 0x00},
   {0x1A, 0x20}, //0x320 =800
   {0x1B, 0x03},

   // Sensor 1 Horizontal window
   {0x1C, 0x97},  // start 0x197
   {0x1D, 0x01},
   {0x1E, 0x56},  //0x500 = 1280 ,0x3C0 = 960
   {0x1F, 0x05},

// Sensor 1 Vertical window
   {0x20, 0x01},
   {0x21, 0x00},
   {0x22, 0x20},//0x320 =800
   {0x23, 0x03},
#else
//crop center
	  // Sensor 0 Horizontal window
   {0x14, 0x37}, //start 0x197, add 0x140/2 = 0xA0 for  center
   {0x15, 0x02},
   {0x16, 0xF6}, //end 0x556, add 0x140 /2 = 0xA0 for  center
   {0x17, 0x05},

// Sensor 0 Vertical window
   {0x18, 0x01},
   {0x19, 0x00},
   {0x1A, 0x20}, //0x320 =800
   {0x1B, 0x03},

 // Sensor 1 Horizontal window
   {0x1C, 0x37},	//start 0x197, add 0x140 /2 = 0xA0 for  center
   {0x1D, 0x02},
   {0x1E, 0xF6},	 //end 0x556, add 0x140 /2 = 0xA0  for  center
   {0x1F, 0x05},

// Sensor 1 Vertical window
   {0x20, 0x01},
   {0x21, 0x00},
   {0x22, 0x20},//0x320 =800
   {0x23, 0x03},
#endif

// CSI SBU output window Horizontal
   {0x24, 0x01}, //1920=0x780 ,add 0x140 for crop center
   {0x25, 0x00},
   {0x26, 0x80},
   {0x27, 0x07},

// CSI SBU output window Vertical
   {0x28, 0x00},
   {0x29, 0x00},
   {0x2A, 0x1F},//0x320 =800
   {0x2B, 0x03},

// CSI IO sync H/V
   {0x2C, 0x00},	/// should be set WD
   {0x2D, 0x0A},
   {0x2E, 0xFF},     /// should be set 0xFFFF
   {0x2F, 0xFF},

   {0x30, 0x00},     /// half of WD
   {0x31, 0x05},
   {0x32, 0x00},

   {0x00, 0x02},

   {0x00, 0x01}

