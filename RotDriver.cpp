#include "RotDriver.h"

RotDriver::RotDriver() {

}

RotDriver::RotDriver(int _id){
	
	id=_id;
	
	forwardPin=24+4*id;
	backwardPin=25+4*id;	
	pinMode(forwardPin,OUTPUT);
	pinMode(backwardPin,OUTPUT);
	
	speedPin=3+2*id;		// Analog out
	sensorPin=2*id+1;		// Analog in
	
	sensorMin=0;
	sensorMax=1023;
		
	highPower=255;
	maxSpeed=127;
	
	lowPowerPos=EEPROM.read(7*id+4);
	lowPowerNeg=EEPROM.read(7*id+5);
	zeroAngle=EEPROM.read(7*id+6);
}

bool RotDriver::calibrate(){

	int length = 30;
	int delayTime = 1000;
	int lastValue = 0;
	
	// Read zeroAngle (Player must face forward before calibration!)	
	zeroAngle=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	
	// Find lowPowerPos	
	lastValue = analogRead(sensorPin);
	
	digitalWrite(forwardPin,1);
	digitalWrite(backwardPin,0);
		
	for(int i=30;i<255;i++){
	
		if(abs(analogRead(sensorPin)-lastValue)>length) {
			lowPowerPos=i;		
			break;
		}
		
		lastValue = analogRead(sensorPin);
		
		analogWrite(speedPin,i);
		delay(delayTime);
		
		analogWrite(speedPin,0);
		delay(100);
		
	}		
	
	analogWrite(speedPin,0);
	delay(500);
	
	// Find lowPowerNeg	
	lastValue = analogRead(sensorPin);
	
	digitalWrite(forwardPin,0);
	digitalWrite(backwardPin,1);
		
	for(int i=30;i<255;i++){
	
		if(abs(analogRead(sensorPin)-lastValue)>length) {
			lowPowerNeg=i;	
			break;
		}
		
		lastValue = analogRead(sensorPin);
		
		analogWrite(speedPin,i);
		delay(delayTime);
		
		analogWrite(speedPin,0);
		delay(100);
		
	}		
	
	analogWrite(speedPin,0);
	delay(500);
		
	return true;
}

// Activate motor with chosen duty cycle
void RotDriver::setSpeed(int vel){
		
	// Set speed
	if(vel == 0) {
	
		digitalWrite(forwardPin,0);
		digitalWrite(backwardPin,0);
		analogWrite(speedPin,0);
		
	} else {	
	
		int lowPower = 0;
		
		// Choose direction
		if(vel > 0){		
			digitalWrite(forwardPin,1);
			digitalWrite(backwardPin,0);		
			lowPower = lowPowerPos;			
		}else if(vel < 0){		
			digitalWrite(forwardPin,0);
			digitalWrite(backwardPin,1);		
			lowPower = lowPowerNeg;			
		}
		
		int finalPower=map(abs(vel),0,maxSpeed,lowPower,highPower);
		analogWrite(speedPin,finalPower);	
		
	}

}

int RotDriver::readConstrained() {
  return map(constrain(analogRead(sensorPin), sensorMin, sensorMax), sensorMin, sensorMax, 0, 255);
}

bool RotDriver::reset() {
	return true;
}
