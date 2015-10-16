#ifndef CAL_HEADER_H
#define CAL_HEADER_H
#if _SENSORLIB_ == _SENSORLIB_OV5653_630_
#include "SensorCal_OV5653_630.h"
#elif _SENSORLIB_ == _SENSORLIB_OV9710_630_
#include "SensorCal_OV9710_630.h"
#elif _SENSORLIB_ == _SENSORLIB_A1040_630_
#include "SensorCal_A1040_630.h"
#elif _SENSORLIB_ == _SENSORLIB_MI5100_630_
#include "SensorCal_MI5100_630.h"
#elif _SENSORLIB_ == _SENSORLIB_OV9710_630_
#include "SensorCal_OV9710_630.h"
#elif _SENSORLIB_ == _SENSORLIB_OV9710_DUAL_630_
#include "SensorCal_OV9710_630.h"
#elif _SENSORLIB_ == _SENSORLIB_OV5633_630_
#include "SensorCal_OV5633_630.h"
#elif _SENSORLIB_ == _SENSORLIB_NT99140_632_
#include "SensorCal_NT99140_632.h"
#elif _SENSORLIB_ == _SENSORLIB_NT99141_632_
#include "SensorCal_NT99141_632.h"
#elif _SENSORLIB_ == _SENSORLIB_MI5100_TVP5150_630_
#include "SensorCal_MI5100_630.h"
#elif _SENSORLIB_ == _SENSORLIB_ZT3150_630_ || _SENSORLIB_ == _SENSORLIB_ZT3120_630_
#include "SensorCal_Dummy_630.h"
#else
#error "You must create a SensorCal_XXX.h for current sensor"
#endif

#endif// CAL_HEADER_H


