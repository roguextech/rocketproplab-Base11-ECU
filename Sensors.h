#ifndef SENSORS_H
#define SENSORS_H
#include "PressureTransducer.h"
#include "ThermoCouple.h"
#include "Settings.h"
#include "Sampler.h"

namespace RPL{
class Sensors{
public:
  Sensors();
  void tick();

private:
  Sampler<PressureTransducer, Settings::PT_PIN_MAP_LEN> ptSampler;
  Sampler<ThermoCouple, Settings::TC_PIN_MAP_LEN> tcSampler;

};
}

#endif
