#include "ThermoCouple.h"
#include "SCMPacket.h"

using namespace RPL;

ThermoCouple::ThermoCouple():ThermoCouple(0){

}

ThermoCouple::ThermoCouple(int id) : ThermoCouple(id, Settings::TC_CS_MAP[id]){

}

ThermoCouple::ThermoCouple(int id, int csPin) : ThermoCouple(id,
  Settings::TC_CLK_PIN, csPin, Settings::TC_DO_PIN){
}

ThermoCouple::ThermoCouple(int id, int clkPin,int csPin, int doPin) :
  id(id),
  thermoCouple(clkPin, csPin, doPin),
  accumulator(0),
  accumulatorCount(0) {

}

float ThermoCouple::writeValueToBuffer(char buffer[]){
  char packetId[2];
  char data[5];

  packetId[0] = 'T';
  if(this->id < 10){
    packetId[1] = '0' + this->id;
  } else {
    packetId[1] = 'A' + (this->id-10);
  }

  float temperature = 0;
  if(this->accumulatorCount != 0){
    temperature = this->accumulator / this->accumulatorCount;
  }
  this->writeFloatToBuffer(temperature, data);

  SCMPacket packet(packetId, data);
  packet.write(buffer);
  return temperature;
}

void ThermoCouple::writeFloatToBuffer(float toWrite, char buffer[]){
  int decimalLeftShifts = 0;
  int maxValue = 10000;
  for(decimalLeftShifts = 0; decimalLeftShifts<5; decimalLeftShifts++){
    if(toWrite >= (maxValue - 0.5 / (10000 / maxValue))){
      break;
    }
    maxValue /= 10;
  }

  int scaleFactor = 1;
  for(int i = 0; i<decimalLeftShifts-1; i++){
    scaleFactor *= 10;
  }

  float fixedPointFloat = toWrite * scaleFactor;
  int fixedPointInt = fixedPointFloat;
  if(fixedPointFloat - fixedPointInt > 0.5){
    fixedPointInt ++;
  }

  for(int i = 0; i<5; i++){
    if(i == (decimalLeftShifts-1)){
      buffer[4-i] = '.';
      continue;
    }
    buffer[4-i] = '0' + fixedPointInt%10;
    fixedPointInt /= 10;
  }
}

void ThermoCouple::aquire(){
  this->accumulator += this->thermoCouple.readInternal();
  this->accumulatorCount++;
}

void ThermoCouple::resetAccumulator(){
  this->accumulator = 0;
  this->accumulatorCount = 0;
}
