//*****************************************************************************
//     Program        :    Digitaql Zoom for MI1030
//     Author        :
//    Goal        :    Setup various digital zoom
//     Functions    :
//*****************************************************************************
#include "sensor_model.h"

#if A1040_630
static UINT16 MAX_Dzoom_Step = 0;
static UINT16 MaxDzTblCnt;

//VGA
UINT16 VDOZOOM_INFOR_TABLE[55][8] =
{
//sie_in            pre_out         ipe_out         crop size
{1272,  476,    656,    476,    640,    476,    2560,   1920,},//10
{1248,  468,    624,    468,    608,    468,    2496,   1872,},//11
{1212,  456,    608,    456,    592,    456,    2432,   1824,},//12
{1172,  444,    592,    444,    576,    444,    2368,   1776,},//13
{1144,  432,    576,    432,    560,    432,    2320,   1740,},//14
{1120,  420,    560,    420,    544,    420,    2272,   1704,},//15
{1088,  408,    544,    408,    528,    408,    2224,   1668,},//16
{1056,  396,    544,    396,    528,    396,    2176,   1632,},//17
{1032,  388,    528,    388,    512,    388,    2128,   1596,},//18
{1008,  380,    512,    380,    496,    380,    2080,   1560,},//19
{984,   372,    496,    372,    480,    372,    2032,   1524,},//20
{972,   364,    496,    364,    480,    364,    1984,   1488,},//21
{952,   356,    480,    356,    464,    356,    1936,   1452,},//22
{932,   348,    464,    348,    448,    348,    1888,   1416,},//23
{908,   340,    448,    340,    432,    340,    1840,   1380,},//24
{888,   332,    448,    332,    432,    332,    1792,   1344,},//25
{864,   324,    432,    324,    416,    324,    1744,   1308,},//26
{844,   316,    416,    316,    400,    316,    1696,   1272,},//27
{824,   308,    400,    308,    384,    308,    1648,   1236,},//28
{804,   300,    400,    300,    384,    300,    1600,   1200,},//29
{784,   296,    384,    296,    368,    296,    1568,   1176,},//30
{768,   288,    384,    288,    368,    288,    1536,   1152,},//31
{752,   280,    368,    280,    352,    280,    1504,   1128,},//32
{736,   276,    368,    276,    352,    276,    1472,   1104,},//33
{720,   272,    352,    272,    336,    272,    1440,   1080,},//34
{704,   264,    352,    264,    336,    264,    1408,   1056,},//35
{688,   256,    336,    256,    320,    256,    1376,   1032,},//36
{672,   252,    336,    252,    320,    252,    1344,   1008,},//37
{656,   244,    320,    244,    304,    244,    1312,   984, },//38
{640,   240,    320,    240,    304,    240,    1280,   960, },//39
{624,   232,    304,    232,    288,    232,    1248,   936, },//40
{608,   228,    304,    228,    288,    228,    1216,   912, },//41
{592,   224,    288,    224,    272,    224,    1184,   888, },//42
{580,   216,    288,    216,    272,    216,    1152,   864, },//43
{556,   208,    272,    208,    256,    208,    1120,   840, },//44
{544,   200,    272,    200,    256,    200,    1088,   816, },//45
{532,   196,    256,    196,    240,    196,    1056,   792, },//46
{512,   192,    256,    192,    240,    192,    1024,   768, },//47
{500,   188,    240,    188,    224,    188,    992,    744, },//48
{488,   180,    240,    180,    224,    180,    960,    720, },//49
{476,   176,    224,    176,    208,    176,    928,    696, },//50
{452,   168,    224,    168,    208,    168,    896,    672, },//51
{440,   164,    208,    164,    192,    164,    864,    648, },//52
{428,   160,    208,    160,    192,    160,    832,    624, },//53
{408,   152,    192,    152,    176,    152,    800,    600, },//54
{396,   148,    192,    148,    176,    148,    784,    588, },//55
{388,   144,    192,    144,    176,    144,    768,    576, },//56
{380,   140,    176,    140,    160,    140,    752,    564, },//57
{364,   136,    176,    136,    160,    136,    736,    552, },//58
{356,   132,    176,    132,    160,    132,    720,    540, },//59
{348,   132,    176,    132,    160,    132,    704,    528, },//60
{340,   128,    160,    128,    144,    128,    688,    516, },//61
{332,   124,    160,    124,    144,    124,    672,    504, },//62
{332,   124,    160,    124,    144,    124,    656,    492, },//63
{320,   120,    160,    120,    144,    120,    640,    480, } //64
};
//720p
UINT16 VDOZOOM_INFOR_MODE_2_TABLE[61][8] =
{
//sie_in      pre_out     ipe_out    crop size
//{1272,  956,    1264,   956,    1248,   956,    1272,    956,},//10
{1272,  944,    1264,   944,    1248,   944,    1272,    944,},//10
{1244,  936,    1232,   936,    1216,   936,    1244,    936,},//11
{1216,  916,    1216,   916,    1200,   916,    1216,    916,},//12
{1188,  896,    1184,   896,    1168,   896,    1188,    896,},//13
{1164,  876,    1152,   876,    1136,   876,    1164,    876,},//14
{1140,  856,    1136,   856,    1120,   856,    1140,    856,},//15
{1116,  836,    1104,   836,    1088,   836,    1116,    836,},//16
{1092,  816,    1088,   816,    1072,   816,    1092,    816,},//17
{1068,  796,    1056,   796,    1040,   796,    1068,    796,},//18
{1044,  780,    1040,   780,    1024,   780,    1044,    780,},//19
{1020,  764,    1008,   764,    992,    764,    1020,    764,},//20
{996,   748,    992,    748,    976,    748,     996,    748,},//21
{976,   732,    976,    732,    960,    732,     976,    732,},//22
{956,   716,    944,    716,    928,    716,     956,    716,},//23
{936,   700,    928,    700,    912,    700,     936,    700,},//24
{916,   684,    912,    684,    896,    684,     916,    684,},//25
{896,   668,    896,    668,    880,    668,     896,    668,},//26
{876,   652,    864,    652,    848,    652,     876,    652,},//27
{856,   644,    848,    644,    832,    644,     856,    644,},//28
{836,   628,    832,    628,    816,    628,     836,    628,},//29
{816,   612,    816,    612,    800,    612,     816,    612,},//30
{796,   596,    784,    596,    768,    596,     796,    596,},//31
{780,   584,    768,    584,    752,    584,     780,    584,},//32
{764,   572,    752,    572,    736,    572,     764,    572,},//33
{748,   560,    736,    560,    720,    560,     748,    560,},//34
{732,   548,    720,    548,    704,    548,     732,    548,},//35
{716,   536,    704,    536,    688,    536,     716,    536,},//36
{700,   524,    688,    524,    672,    524,     700,    524,},//37
{684,   512,    672,    512,    656,    512,     684,    512,},//38
{668,   500,    656,    500,    640,    500,     668,    500,},//39
{652,   488,    640,    488,    624,    488,     652,    488,},//40
{636,   476,    624,    476,    608,    476,     636,    476,},//41
{620,   464,    608,    464,    592,    464,     620,    464,},//42
{604,   452,    592,    452,    576,    452,     604,    452,},//43
{588,   448,    576,    448,    560,    448,     588,    448,},//44
{576,   436,    576,    436,    560,    436,     576,    436,},//45
{564,   424,    560,    424,    544,    424,     564,    424,},//46
{552,   416,    544,    416,    528,    416,     552,    416,},//47
{540,   404,    528,    404,    512,    404,     540,    404,},//48
{528,   396,    528,    396,    512,    396,     528,    396,},//49
{516,   388,    512,    388,    496,    388,     516,    388,},//50
{504,   380,    496,    380,    480,    380,     504,    380,},//51
{496,   372,    496,    372,    480,    372,     496,    372,},//52
{484,   364,    480,    364,    464,    364,     484,    364,},//53
{472,   356,    464,    356,    448,    356,     472,    356,},//54
{460,   348,    448,    348,    432,    348,     460,    348,},//55
{448,   336,    448,    336,    432,    336,     448,    336,},//56
{436,   328,    432,    328,    416,    328,     436,    328,},//57
{424,   320,    416,    320,    400,    320,     424,    320,},//58
{416,   312,    416,    312,    400,    312,     416,    312,},//59
{404,   304,    400,    304,    384,    304,     404,   304, },//60
{392,   296,    384,    296,    368,    296,     392,   296, },//61
{384,   288,    384,    288,    368,    288,     384,   288, },//62
{376,   284,    368,    284,    352,    284,     376,   284, },//63
{368,   276,    368,    276,    352,    276,     368,   276, },//64
{360,   272,    352,    272,    336,    272,     360,   272, },//65
{352,   264,    352,    264,    336,    264,     352,   264, },//66
{344,   260,    336,    260,    320,    260,     344,   260, },//67
{336,   252,    336,    252,    320,    252,     336,   252, },//68
{328,   244,    320,    244,    304,    244,     328,   244, },//69
{320,   240,    320,    240,    304,    240,     320,   240, } //70
};

//1280*960
UINT16 VDOZOOM_INFOR_MODE_3_TABLE[61][8] =
{
//sie_in      pre_out     ipe_out    crop size
{1272,  956,    1264,   956,    1248,   956,    2560,   1920,},//10
{1244,  936,    1232,   936,    1216,   936,    2496,   1872,},//11
{1216,  916,    1216,   916,    1200,   916,    2448,   1836,},//12
{1188,  896,    1184,   896,    1168,   896,    2384,   1788,},//13
{1164,  876,    1152,   876,    1136,   876,    2336,   1752,},//14
{1140,  856,    1136,   856,    1120,   856,    2288,   1716,},//15
{1116,  836,    1104,   836,    1088,   836,    2240,   1680,},//16
{1092,  816,    1088,   816,    1072,   816,    2192,   1644,},//17
{1068,  796,    1056,   796,    1040,   796,    2144,   1608,},//18
{1044,  780,    1040,   780,    1024,   780,    2096,   1572,},//19
{1020,  764,    1008,   764,    992,    764,    2048,   1536,},//20
{996,   748,    992,    748,    976,    748,    2000,   1500,},//21
{976,   732,    976,    732,    960,    732,    1952,   1464,},//22
{956,   716,    944,    716,    928,    716,    1904,   1428,},//23
{936,   700,    928,    700,    912,    700,    1872,   1404,},//24
{916,   684,    912,    684,    896,    684,    1824,   1368,},//25
{896,   668,    896,    668,    880,    668,    1776,   1332,},//26
{876,   652,    864,    652,    848,    652,    1744,   1308,},//27
{856,   644,    848,    644,    832,    644,    1728,   1296,},//28
{836,   628,    832,    628,    816,    628,    1680,   1260,},//29
{816,   612,    816,    612,    800,    612,    1632,   1224,},//30
{796,   596,    784,    596,    768,    596,    1600,   1200,},//31
{780,   584,    768,    584,    752,    584,    1568,   1176,},//32
{764,   572,    752,    572,    736,    572,    1536,   1152,},//33
{748,   560,    736,    560,    720,    560,    1504,   1128,},//34
{732,   548,    720,    548,    704,    548,    1472,   1104,},//35
{716,   536,    704,    536,    688,    536,    1440,   1080,},//36
{700,   524,    688,    524,    672,    524,    1408,   1056,},//37
{684,   512,    672,    512,    656,    512,    1376,   1032,},//38
{668,   500,    656,    500,    640,    500,    1344,   1008,},//39
{652,   488,    640,    488,    624,    488,    1312,   984, },//40
{636,   476,    624,    476,    608,    476,    1280,   960, },//41
{620,   464,    608,    464,    592,    464,    1248,   936, },//42
{604,   452,    592,    452,    576,    452,    1216,   912, },//43
{588,   448,    576,    448,    560,    448,    1200,   900, },//44
{576,   436,    576,    436,    560,    436,    1168,   876, },//45
{564,   424,    560,    424,    544,    424,    1136,   852, },//46
{552,   416,    544,    416,    528,    416,    1120,   840, },//47
{540,   404,    528,    404,    512,    404,    1088,   816, },//48
{528,   396,    528,    396,    512,    396,    1056,   792, },//49
{516,   388,    512,    388,    496,    388,    1040,   780, },//50
{504,   380,    496,    380,    480,    380,    1024,   768, },//51
{496,   372,    496,    372,    480,    372,    992,    744, },//52
{484,   364,    480,    364,    464,    364,    976,    732, },//53
{472,   356,    464,    356,    448,    356,    960,    720, },//54
{460,   348,    448,    348,    432,    348,    928,    696, },//55
{448,   336,    448,    336,    432,    336,    896,    672, },//56
{436,   328,    432,    328,    416,    328,    880,    660, },//57
{424,   320,    416,    320,    400,    320,    848,    636, },//58
{416,   312,    416,    312,    400,    312,    832,    624, },//59
{404,   304,    400,    304,    384,    304,    816,    612, },//60
{392,   296,    384,    296,    368,    296,    800,    600, },//61
{384,   288,    384,    288,    368,    288,    784,    588, },//62
{376,   284,    368,    284,    352,    284,    768,    576, },//63
{368,   276,    368,    276,    352,    276,    736,    552, },//64
{360,   272,    352,    272,    336,    272,    720,    540, },//65
{352,   264,    352,    264,    336,    264,    704,    528, },//66
{344,   260,    336,    260,    320,    260,    688,    516, },//67
{336,   252,    336,    252,    320,    252,    672,    504, },//68
{328,   244,    320,    244,    304,    244,    656,    492, },//69
{320,   240,    320,    240,    304,    240,    640,    480, } //70
};
//848*480
UINT16 VDOZOOM_INFOR_MODE_4_TABLE[56][8] =
{
//sie_in      pre_out     ipe_out    crop size
{1272,  476,    864,    476,    848,    476,    2560,   1920,},//10
{1244,  468,    844,    468,    816,    468,    2496,   1872,},//11
{1216,  456,    824,    456,    800,    456,    2432,   1824,},//12
{1188,  444,    804,    444,    784,    444,    2368,   1776,},//13
{1164,  432,    784,    432,    768,    432,    2320,   1740,},//14
{1140,  424,    768,    424,    752,    424,    2272,   1704,},//15
{1116,  412,    752,    412,    736,    412,    2224,   1668,},//16
{1092,  400,    736,    400,    720,    400,    2144,   1608,},//17
{1068,  392,    720,    392,    704,    392,    2096,   1572,},//18
{1044,  384,    704,    384,    688,    384,    2048,   1536,},//19
{1020,  376,    688,    376,    672,    376,    2016,   1512,},//20
{996,   368,    672,    368,    656,    368,    1968,   1476,},//21
{976,   360,    656,    360,    640,    360,    1936,   1452,},//22
{956,   352,    640,    352,    624,    352,    1888,   1416,},//23
{936,   344,    624,    344,    608,    344,    1840,   1380,},//24
{916,   336,    608,    336,    592,    336,    1792,   1344,},//25
{896,   328,    592,    328,    576,    328,    1760,   1320,},//26
{876,   320,    580,    320,    560,    320,    1728,   1296,},//27
{856,   312,    568,    312,    544,    312,    1680,   1260,},//28
{836,   304,    556,    304,    528,    304,    1632,   1224,},//29
{816,   296,    544,    296,    528,    296,    1600,   1200,},//30
{796,   292,    532,    292,    512,    292,    1568,   1176,},//31
{780,   284,    520,    284,    496,    284,    1536,   1152,},//32
{764,   280,    508,    280,    480,    280,    1504,   1128,},//33
{748,   272,    496,    272,    480,    272,    1456,   1092,},//34
{732,   264,    484,    264,    464,    264,    1424,   1068,},//35
{716,   260,    472,    260,    448,    260,    1392,   1044,},//36
{700,   252,    460,    252,    432,    252,    1360,   1020,},//37
{684,   248,    448,    248,    432,    248,    1328,   996,}, //38
{668,   240,    436,    240,    416,    240,    1296,   972,}, //39
{652,   232,    424,    232,    400,    232,    1248,   936,}, //40
{636,   228,    412,    228,    384,    228,    1216,   912,}, //41
{620,   220,    400,    220,    384,    220,    1184,   888,}, //42
{604,   216,    392,    216,    368,    216,    1152,   864,}, //43
{588,   212,    384,    212,    368,    212,    1136,   852,}, //44
{576,   208,    376,    208,    352,    208,    1104,   828,}, //45
{564,   204,    368,    204,    352,    204,    1088,   816,}, //46
{552,   196,    360,    196,    336,    196,    1056,   792,}, //47
{540,   192,    352,    192,    336,    192,    1040,   780,}, //48
{528,   188,    344,    188,    320,    188,    1008,   756,}, //49
{516,   184,    336,    184,    320,    184,    992,    744,}, //50
{504,   180,    328,    180,    304,    180,    960,    720,}, //51
{492,   176,    320,    176,    304,    176,    944,    708,}, //52
{480,   172,    312,    172,    288,    172,    928,    696,}, //53
{468,   168,    304,    168,    288,    168,    896,    672,}, //54
{456,   164,    296,    164,    272,    164,    880,    660,}, //55
{444,   160,    288,    160,    272,    160,    864,    648,}, //56
{432,   156,    280,    156,    256,    156,    832,    624,}, //57
{420,   152,    272,    152,    256,    152,    816,    612,}, //58
{408,   144,    264,    144,    240,    144,    768,    576,}, //59
{396,   140,    256,    140,    240,    140,    752,    564,}, //60
{388,   136,    248,    136,    224,    136,    736,    552,}, //61
{380,   132,    240,    132,    224,    132,    704,    528,}, //62
{372,   128,    232,    128,    208,    128,    688,    516,}, //63
{364,   124,    224,    124,    208,    124,    672,    504,}, //64
{356,   120,    216,    120,    192,    120,    640,    480,}  //65

};
//2560*960
UINT16 VDOZOOM_INFOR_MODE_5_TABLE[61][8] =
{
//sie_in            pre_out     ipe_out    crop size
{2556,  956,    1456,   956,    1440,   956,    2560,   1920,},//10
{2504,  936,    1424,   936,    1408,   936,    2496,   1872,},//11
{2452,  916,    1392,   916,    1376,   916,    2448,   1836,},//12
{2400,  896,    1360,   896,    1344,   896,    2400,   1800,},//13
{2352,  880,    1328,   880,    1312,   880,    2352,   1764,},//14
{2304,  864,    1312,   864,    1296,   864,    2304,   1728,},//15
{2256,  844,    1280,   844,    1264,   844,    2256,   1692,},//16
{2200,  824,    1248,   824,    1232,   824,    2208,   1656,},//17
{2156,  804,    1216,   804,    1200,   804,    2160,   1620,},//18
{2112,  784,    1200,   784,    1184,   784,    2112,   1584,},//19
{2068,  768,    1168,   768,    1152,   768,    2064,   1548,},//20
{2024,  752,    1152,   752,    1136,   752,    2016,   1512,},//21
{1980,  740,    1120,   740,    1104,   740,    1968,   1476,},//22
{1940,  724,    1104,   724,    1088,   724,    1920,   1440,},//23
{1900,  708,    1072,   708,    1056,   708,    1888,   1416,},//24
{1860,  692,    1056,   692,    1040,   692,    1856,   1392,},//25
{1820,  676,    1024,   676,    1008,   676,    1808,   1356,},//26
{1780,  664,    1008,   664,    992,    664,    1776,   1332,},//27
{1736,  648,    976,    648,    960,    648,    1728,   1296,},//28
{1692,  632,    960,    632,    944,    632,    1696,   1272,},//29
{1656,  616,    944,    616,    928,    616,    1648,   1236,},//30
{1620,  608,    912,    608,    896,    608,    1632,   1224,},//31
{1584,  592,    896,    592,    880,    592,    1584,   1188,},//32
{1552,  580,    880,    580,    864,    580,    1552,   1164,},//33
{1520,  568,    864,    568,    848,    568,    1520,   1140,},//34
{1488,  556,    832,    556,    816,    556,    1488,   1116,},//35
{1456,  544,    816,    544,    800,    544,    1456,   1092,},//36
{1424,  532,    800,    532,    784,    532,    1424,   1068,},//37
{1392,  520,    784,    520,    768,    520,    1392,   1044,},//38
{1364,  508,    768,    508,    752,    508,    1360,   1020,},//39
{1336,  500,    752,    500,    736,    500,    1328,   996, },//40
{1308,  488,    736,    488,    720,    488,    1312,   984, },//41
{1276,  476,    720,    476,    704,    476,    1280,   960, },//42
{1248,  468,    704,    468,    688,    468,    1248,   936, },//43
{1220,  456,    688,    456,    672,    456,    1216,   912, },//44
{1192,  448,    672,    448,    656,    448,    1200,   900, },//45
{1164,  436,    656,    436,    640,    436,    1168,   876, },//46
{1140,  424,    640,    424,    624,    424,    1136,   852, },//47
{1116,  416,    624,    416,    608,    416,    1120,   840, },//48
{1092,  404,    608,    404,    592,    404,    1088,   816, },//49
{1048,  392,    592,    392,    576,    392,    1056,   792, },//50
{1028,  384,    576,    384,    560,    384,    1024,   768, },//51
{1016,  380,    576,    380,    560,    380,    1008,   756, },//52
{992,   372,    560,    372,    544,    372,    992,    744, },//53
{972,   364,    544,    364,    528,    364,    976,    732, },//54
{952,   356,    544,    356,    528,    356,    960,    720, },//55
{932,   348,    528,    348,    512,    348,    928,    696, },//56
{912,   340,    512,    340,    496,    340,    912,    684, },//57
{892,   332,    496,    332,    480,    332,    896,    672, },//58
{864,   324,    480,    324,    464,    324,    864,    648, },//59
{844,   316,    480,    316,    464,    316,    848,    636, },//60
{824,   308,    464,    308,    448,    308,    816,    612, },//61
{800,   300,    448,    300,    432,    300,    800,    600, },//62
{780,   292,    432,    292,    416,    292,    784,    588, },//63
{760,   284,    432,    284,    416,    284,    768,    576, },//64
{744,   276,    416,    276,    400,    276,    736,    552, },//65
{716,   268,    400,    268,    384,    268,    720,    540, },//66
{696,   260,    384,    260,    368,    260,    704,    528, },//67
{680,   256,    384,    256,    368,    256,    688,    516, },//68
{664,   248,    368,    248,    352,    248,    656,    492, },//69
{640,   240,    352,    240,    336,    240,    640,    480, } //70
};

DZOOM_INFOR VDOZOOM_INFOR = {_Ratio_4_3, VDOZOOM_INFOR_TABLE,sizeof(VDOZOOM_INFOR_TABLE)/(8*2)};
DZOOM_INFOR VDOZOOM_INFOR_MODE_2 = {_Ratio_4_3/*_Ratio_16_9*/, VDOZOOM_INFOR_MODE_2_TABLE,sizeof(VDOZOOM_INFOR_MODE_2_TABLE)/(8*2)};
DZOOM_INFOR VDOZOOM_INFOR_MODE_3 = {_Ratio_4_3, VDOZOOM_INFOR_MODE_3_TABLE,sizeof(VDOZOOM_INFOR_MODE_3_TABLE)/(8*2)};
DZOOM_INFOR VDOZOOM_INFOR_MODE_4 = {_Ratio_4_3,VDOZOOM_INFOR_MODE_4_TABLE,sizeof(VDOZOOM_INFOR_MODE_4_TABLE)/(8*2)};
DZOOM_INFOR VDOZOOM_INFOR_MODE_5 = {_Ratio_4_3,VDOZOOM_INFOR_MODE_5_TABLE,sizeof(VDOZOOM_INFOR_MODE_5_TABLE)/(8*2)};

INT zoom_index = 10;
SIZE_INFOR    LCD_Size, JPEG_Pre_Size,JPEG_Cap_Size;
SIZE_INFOR    IPE_Out_Prv_Size, IPE_Out_Cap_Size;
SIZE_INFOR    PRE_Hsub_Prv_Size, PRE_Hsub_Cap_Size;
SIZE_INFOR    IME_Out_Prv_Size, IME_Out_Cap_Size;
SIZE_INFOR    PRE_Crop_Prv_Size, PRE_Crop_Cap_Size;

void Set_DzoomIndex(INT idx)
{
    zoom_index = idx;
}
INT Get_DzoomIndex(void)
{
    return zoom_index;
}
void Set_DzoomMaxIndex(UINT16 index)
{
    if(index >  MaxDzTblCnt)
    {
        MAX_Dzoom_Step = MaxDzTblCnt;
        debug_err(("Out of Dzoom table range, force max to:%d\r\n",MaxDzTblCnt));
    }
    else
        MAX_Dzoom_Step = index;
}

UINT16 Get_DzoomMaxIndex(void)
{
    if(MAX_Dzoom_Step!=0)
        return MAX_Dzoom_Step;
    else
        return MaxDzTblCnt;
}


//#NT#20090912#ethanlau -begin
//add mode
DZOOM_INFOR* Get_DZoomInfor(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    DZOOM_INFOR *pTableInfor;

    pTableInfor = &VDOZOOM_INFOR;
    switch(SIE_PRE_PARAM->VGAType)
    {
        case SENSOR_OUT_TWO_LINE:
        case SENSOR_OUT_THREE_LINE:
        default:
            switch(SIE_PRE_PARAM->ModeSel)
            {
                case MONITOR_MODE_4:
                    if(gImageAlgInfo.ImageRatio == _Ratio_4_3)
                        pTableInfor = &VDOZOOM_INFOR;
                    else if(gImageAlgInfo.ImageRatio == _Ratio_3_2)
                        pTableInfor = &VDOZOOM_INFOR;
                    else
                        pTableInfor = &VDOZOOM_INFOR_MODE_4;
                break;
                case MONITOR_MODE_2:
                    pTableInfor = &VDOZOOM_INFOR_MODE_2;
                break;
                case MONITOR_MODE_3:
                    pTableInfor = &VDOZOOM_INFOR_MODE_3;
                break;
                case MONITOR_MODE_5:
                    pTableInfor = &VDOZOOM_INFOR_MODE_5;
                break;
                case MONITOR_MODE:
                default:
                    if(gImageAlgInfo.ImageRatio == _Ratio_4_3)
                        pTableInfor = &VDOZOOM_INFOR;
                    else
                        pTableInfor = &VDOZOOM_INFOR_MODE_4;
                break;
            }
            break;
    }
    return pTableInfor;
}
//#NT#20090912#ethanlau -end
UINT16 Get_HRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx)
{
    return usImgRatioTable[ImgRatioIdx][DefaultRatioIdx];
}

UINT16 Get_VRatio(UINT32 ImgRatioIdx, UINT32 DefaultRatioIdx)
{
    return usImgRatioTable[DefaultRatioIdx][ImgRatioIdx];
}

UINT32 Adj_VSize(UINT32 InSize, UINT16 VRatio, UINT16 Align)
{
    UINT32 rtValue;

    rtValue = InSize * VRatio / IMG_RATIO_UINT;
    rtValue = (rtValue + (Align - 1)) / Align * Align;
    return rtValue;
}

UINT32 Adj_HSize(UINT32 InSize, UINT16 HRatio, UINT16 Align)
{
    UINT32 rtValue;

    rtValue = InSize;
    rtValue = InSize * HRatio / IMG_RATIO_UINT;
    rtValue = (rtValue + (Align - 1)) / Align * Align;
    return rtValue;
}

UINT32 Adj_PreLineOffset(UINT32 LineOffset, PRE_VGAFormatEnum VGAType, UINT8 PreBitDepth)
{
    UINT32 lofs;

    lofs = LineOffset * PreBitDepth / 8;
    if (VGAType == SENSOR_OUT_THREE_LINE)
    {
        lofs = (lofs + (PRE_BLOCK_BUF_SIZE/3 - 1)) / (PRE_BLOCK_BUF_SIZE/3) * (PRE_BLOCK_BUF_SIZE/3);
    }
    else if (VGAType == SENSOR_OUT_TWO_LINE)
    {
        lofs = (lofs + (PRE_BLOCK_BUF_SIZE/2 - 1)) / (PRE_BLOCK_BUF_SIZE/2) * (PRE_BLOCK_BUF_SIZE/2);
    }
    return lofs;
}
//#NT#20090921#ethanlau -begin
void Set_digitalzoom(UINT DzoomIdx, struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 lofs, i, idx, AdjSize;
    DZOOM_INFOR *pTableInfor;
    UINT16 (*DzTable)[8], HRatio, VRatio, AdjTable[1][8];

    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    DzTable = (UINT16(*)[8])pTableInfor->DzTable;
    //#NT#2010/09/09#ethanlau -begin
    //avoid dzoom max index error causeed by dzoom talbe max size different between different tbl,index is start from 10 not 0
    MaxDzTblCnt = pTableInfor->TblSize + 10-1;
    if(DzoomIdx < 10)
    {
        DzoomIdx = 10;
    }
    else if(DzoomIdx > Get_DzoomMaxIndex())
    {
        DzoomIdx = Get_DzoomMaxIndex();
    }
    //#NT#2010/09/09#ethanlau -end
    SetIPPSizeInfor(_SensorRatioIdx, pTableInfor->SensorRatio);
    //for image pipeline
    if (GetIPPSizeInfor(_SensorRatioIdx) != gImageAlgInfo.ImageRatio)
    {
        //get adjust ratio
        HRatio = Get_HRatio(gImageAlgInfo.ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
        VRatio = Get_VRatio(gImageAlgInfo.ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));

        for (i = 0; i < 8; i ++) AdjTable[0][i] = DzTable[DzoomIdx - 10][i];

        if ((HRatio != IMG_RATIO_UINT) && (VRatio == IMG_RATIO_UINT))
        {
            AdjTable[0][6] = Adj_HSize(AdjTable[0][6], HRatio, 16);
            if (SIE_PRE_PARAM->VGAType == SENSOR_OUT_TWO_LINE)
            {
                AdjTable[0][0] = Adj_HSize(AdjTable[0][0], HRatio, 16);
                AdjSize = AdjTable[0][0] / 2;
            }
            else if (SIE_PRE_PARAM->VGAType == SENSOR_OUT_THREE_LINE)
            {
                AdjTable[0][0] = Adj_HSize(AdjTable[0][0], HRatio, 12);
                AdjSize = ((AdjTable[0][0] - THREE_LINE_DUMMY_PIX) / 3);
            }
            else
            {
                AdjTable[0][0] = Adj_HSize(AdjTable[0][0], HRatio, 16);
                AdjSize = AdjTable[0][0];
            }

            if (AdjSize  > DzTable[DzoomIdx - 10][2])
            {
                AdjTable[0][2] = DzTable[DzoomIdx - 10][2];
            }
            else
            {
                AdjTable[0][2] = AdjSize / 16 * 16;
            }
            AdjTable[0][4] = AdjTable[0][2] - IPE_HSIZE_IODIFF;
        }
        else if ((HRatio == IMG_RATIO_UINT) && (VRatio != IMG_RATIO_UINT))
        {
            AdjTable[0][1] = Adj_VSize(AdjTable[0][1], VRatio, 4);
            AdjTable[0][7] = Adj_VSize(AdjTable[0][7], VRatio, 4);
            if (SIE_PRE_PARAM->VGAType == SENSOR_OUT_TWO_LINE)
            {
                AdjTable[0][5] = AdjTable[0][3] = AdjTable[0][1] * 2;
            }
            else if (SIE_PRE_PARAM->VGAType == SENSOR_OUT_THREE_LINE)
            {
                AdjTable[0][5] = AdjTable[0][3] = AdjTable[0][1] * 3;
            }
            else
            {
                AdjTable[0][5] = AdjTable[0][3] = AdjTable[0][1];
            }
        }
        else
        {
            debug_err(("dzoom..not support\r\n"));
        }

        idx = 0;
        DzTable = (UINT16(*)[8])AdjTable;
    }
    else
    {
        idx = DzoomIdx - 10;
    }

    if (gImageAlgInfo.ImageRatio != gImageAlgInfo.DispRatio)
    {
        //get adjust ratio
        HRatio = Get_HRatio(gImageAlgInfo.ImageRatio, gImageAlgInfo.DispRatio);
        VRatio = Get_VRatio(gImageAlgInfo.ImageRatio, gImageAlgInfo.DispRatio);

        SetIPPSizeInfor(_ImeOut1Hsize, Adj_HSize(DispSizeInfo.uiDisplayFBWidth, HRatio, 16));
        SetIPPSizeInfor(_ImeOut1Vsize, Adj_VSize(DispSizeInfo.uiDisplayFBHeight, VRatio, 4));
        SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    }
    else
    {
        SetIPPSizeInfor(_ImeOut1Hsize, DispSizeInfo.uiDisplayFBWidth);
        SetIPPSizeInfor(_ImeOut1Vsize, DispSizeInfo.uiDisplayFBHeight);
        SetIPPSizeInfor(_ImeOut1LineOffset, GetIPPSizeInfor(_ImeOut1Hsize));
    }


    lofs = Adj_PreLineOffset(DzTable[idx][2], SIE_PRE_PARAM->VGAType, gImageAlgInfo.PrePrvBitDepth);
    SetIPPSizeInfor(_PreOutLineOffset, lofs);
    SetIPPSizeInfor(_IpeOutHsize,DzTable[idx][4]);
    SetIPPSizeInfor(_IpeOutVsize,DzTable[idx][5]);
    SetIPPSizeInfor(_PreOutHsize,DzTable[idx][2]);
    SetIPPSizeInfor(_PreOutVsize,DzTable[idx][3]);
    SetIPPSizeInfor(_PreCropHsize,DzTable[idx][0]);
    SetIPPSizeInfor(_PreCropVsize,DzTable[idx][1]);
    SetIPPSizeInfor(_IpeCapOutHsize,DzTable[idx][6] - IPE_HSIZE_IODIFF);
    SetIPPSizeInfor(_IpeCapOutVsize,DzTable[idx][7]);
    SetIPPSizeInfor(_ImeCapOutHsize,DzTable[idx][6] - IPE_HSIZE_IODIFF);
    SetIPPSizeInfor(_ImeCapOutVsize,DzTable[idx][7]);
    SetIPPSizeInfor(_PreCropCapHsize,DzTable[idx][6]);
    SetIPPSizeInfor(_PreCropCapVsize,DzTable[idx][7]);
    //debug_msg("#~~~~~~~~~~~~~~~~~#\r\n");
    //for(i=0;i<8;i++)
    //    debug_msg(" %d ",DzTable[idx][i]);
    //debug_msg("\r\n");

    //debug_msg("pre out size: %d %d %d\r\n",GetIPPSizeInfor(_PreOutHsize),GetIPPSizeInfor(_PreOutVsize),GetIPPSizeInfor(_PreOutLineOffset));
    //debug_msg("ime path1 out size: %d %d %d\r\n",GetIPPSizeInfor(_ImeOut1Hsize),GetIPPSizeInfor(_ImeOut1Vsize),GetIPPSizeInfor(_ImeOut1LineOffset));
}
//#NT#20090921#ethanlau -end
void SetImageOutSize(UINT16 Hsize, UINT16 Vsize)
{
    SetIPPSizeInfor(_JpegCapHsize,Hsize);
    SetIPPSizeInfor(_JpegCapVsize,Vsize);
}

UINT32 Get_DzoomPrvMaxBuffer(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    DZOOM_INFOR *pTableInfor;
    UINT16 (*DzTable)[8];
    UINT32 lofs;
//#NT#20090922#ethanlau -begin
//dynamic change mode, so assign max buffer size
    //pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    pTableInfor = &VDOZOOM_INFOR_MODE_5;//
//#NT#20090922#ethanlau -end
    DzTable = (UINT16(*)[8])pTableInfor->DzTable;
    lofs = Adj_PreLineOffset(DzTable[0][2], SIE_PRE_PARAM->VGAType, gImageAlgInfo.PrePrvBitDepth);
    return (lofs * (DzTable[0][5]));
}

//#NT#20090912#ethanlau -begin
UINT32 Get_DzoomPrvMaxBuffer_SIE(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 Hsize, Vsize, BufferSize;

    Hsize = Vsize = 0;
    switch(SIE_PRE_PARAM->ModeSel )
    {
        case MONITOR_MODE:
        case MONITOR_MODE_2:
        case MONITOR_MODE_3:
        case MONITOR_MODE_4:
        case MONITOR_MODE_5:
            Hsize = HD_Sync_Prv_5;
            Vsize = VD_Total_Prv_5;
        break;
    }
#if 0
    if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_2)
    {
        Hsize = ((HD_Total_Prv_2)-(ACT_STRX_Prv_2));
        Vsize = ((VD_Total_Prv_2)-(ACT_STRY_Prv_2));
    }
    else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_3)
    {
        Hsize = ((HD_Total_Prv_3)-(ACT_STRX_Prv_3));
        Vsize = ((VD_Total_Prv_3)-(ACT_STRY_Prv_3));
    }
    else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE)
    {
        Hsize = ((HD_Total_Prv)-(ACT_STRX_Prv));
        Vsize = ((VD_Total_Prv)-(ACT_STRY_Prv));
    }
    else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_4)
    {
        Hsize = ((HD_Total_Prv_4)-(ACT_STRX_Prv_4));
        Vsize = ((VD_Total_Prv_4)-(ACT_STRY_Prv_4));
    }
    else if (SIE_PRE_PARAM->ModeSel==MONITOR_MODE_5)
    {
        Hsize = ((HD_Total_Prv_5)-(ACT_STRX_Prv_5));
        Vsize = ((VD_Total_Prv_5)-(ACT_STRY_Prv_5));
    }
#endif
    Hsize = Hsize&0xfffffffc;

    if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_8BIT)
    {
        BufferSize = Hsize*Vsize;
    }
    else if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_12BIT)
    {
        BufferSize = Hsize*Vsize*3/2;
    }
    else if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_16BIT)
    {
        BufferSize = Hsize*Vsize*2;
    }
    else
    {
        BufferSize = Hsize*Vsize*2;//give me more if you're not sure
        debug_err(("ERROR\r\n"));
    }

    BufferSize = (BufferSize+3)&0xfffffffc;

    return BufferSize;
}
//#NT#20090912#ethanlau -end
UINT32 Get_DzoomCapMaxFieldBuffer_SIE(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 Hsize, Vsize, BufferSize;

    Hsize = ((HD_Total_Cap)-(ACT_STRX_Cap));
    Vsize = ((VD_Total_Cap)-(ACT_STRY_Cap));

    Hsize = Hsize&0xfffffffc;

    if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_8BIT)
    {
        BufferSize = Hsize*Vsize;
    }
    else if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_12BIT)
    {
        BufferSize = Hsize*Vsize*3/2;
    }
    else if (SIE_PRE_PARAM->SieBitDepth == SIE_OUT_PACK_16BIT)
    {
        BufferSize = Hsize*Vsize*2;
    }
    else
    {
        BufferSize = Hsize*Vsize*2;//give me more if you're not sure
        debug_err(("ERROR\r\n"));
    }

    BufferSize = (BufferSize+3)&0xfffffffc;

    return BufferSize;
}

UINT32 Get_DzoomCapMaxBuffer(struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 Width, Height;
    UINT16 (*DzTable)[8], HRatio, VRatio;
    DZOOM_INFOR *pTableInfor;

    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    DzTable = (UINT16(*)[8])pTableInfor->DzTable;

    HRatio = Get_HRatio(gImageAlgInfo.ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
    VRatio = Get_VRatio(gImageAlgInfo.ImageRatio, GetIPPSizeInfor(_SensorRatioIdx));
    //#NT#20091012#ethanlau -begin
    //Width = (Adj_HSize(DzTable[0][6], HRatio, 16) * gImageAlgInfo.PrePrvBitDepth / 8);
    Width = (Adj_HSize(DzTable[0][6], HRatio, 16) * gImageAlgInfo.PreCapBitDepth / 8);
    //#NT#20091012#ethanlau -end
    Height = Adj_VSize(DzTable[0][7], VRatio, 4);
    return (Width * Height);
}
//#NT#2010/09/11#ethanlau -begin
//#NT#return Dzoom ratio for UI
UINT32 GetRealDZoomRatio(UINT32 idx,struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    UINT32 ratio;
    DZOOM_INFOR *pTableInfor;
    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    ratio = (((pTableInfor->DzTable[0][1]+4) * 100)+pTableInfor->DzTable[idx - 10][1]-1) / pTableInfor->DzTable[idx - 10][1];

    if ((ratio % 10) > 4)
        ratio = ratio / 10 + 1;
    else
        ratio = ratio / 10;

    return (ratio);
}
//#NT#2010/09/11#ethanlau -end
//#NT#2010/09/11#ethanlau -begin
//#NT#Use Dzoom ratio to estimate new dzoom table index.(to maintain ratio between different sensor mode)
UINT GetDZoomIdxFromRatio(UINT Ratio,struct SIE_PRE_PARAM_STRUCT *SIE_PRE_PARAM)
{
    DZOOM_INFOR *pTableInfor;
    UINT i,_ratio;
    pTableInfor = Get_DZoomInfor(SIE_PRE_PARAM);
    for(i=0;i<pTableInfor->TblSize;i++)
    {
        _ratio = (((pTableInfor->DzTable[0][1]+4) * 100)+pTableInfor->DzTable[i][1]-1) / pTableInfor->DzTable[i][1];
        if ((_ratio % 10) > 4)
            _ratio = _ratio / 10 + 1;
        else
            _ratio = _ratio / 10;
        if(_ratio >= Ratio)
        {
            Set_DzoomIndex((i+10));
            return (i+10);
        }
    }
    Set_DzoomIndex((i+10));
    return (i+10);
}
//#NT#2010/09/11#ethanlau -end
#endif//----------------------------------------------------------------------------------------------------
