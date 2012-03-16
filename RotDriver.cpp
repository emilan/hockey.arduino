#include "RotDriver.h"

RotDriver::RotDriver() {
	;
}

RotDriver::RotDriver(int _id){
	
	id=_id;
	
	forwardPin=24+4*id;
	backwardPin=25+4*id;	
	pinMode(forwardPin,OUTPUT);
	pinMode(backwardPin,OUTPUT);
	
	speedPin=3+2*id;	// Analog out
	sensorPin=4*id;		// Analog in
	
	sensorMin=0;
	sensorMax=1023;
	
	lowPowerPos=0;
	lowPowerNeg=0;
	highPower=255;
	
	maxSpeed=127; 					/// ?!?!?!?
	zeroAngle=EEPROM.read(3*id+2);  /// ?!?!?!?
}

bool RotDriver::calibrate(){
	
	int pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
		
	// Read zeroAngle (Player must face forward before calibration!)	
	zeroAngle=pos;
			
	int length = 5;
	int delayTime = 100;
	
	// Find lowPowerPos
	int firstPos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	
	for(int i=0;i<255;i=i+2){
			
		pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
		setSpeed(i);
		delay(delayTime);
			
		if(abs(pos-firstPos)>length){
			lowPowerPos=i;						
			break;
		}
	}
	
	setSpeed(0);
	delay(500);
	
	// Find lowPowerNeg
	
	length = 5;
	delayTime = 100;
	
	firstPos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	
	for(int i=0;i<255;i=i+2){
			
		pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
		setSpeed(-i);
		delay(delayTime);
			
		if(abs(pos-firstPos)>length){
			lowPowerNeg=i;						
			break;
		}
	}
	
	setSpeed(0);
	delay(500);
		
	return true;
}

// Activate motor with chosen duty cycle
void RotDriver::setSpeed(int vel){
	
	int power = 0;
	
	// Choose direction
	if(vel>0){
		digitalWrite(forwardPin,HIGH);
		digitalWrite(backwardPin,LOW);		
		power = lowPowerPos;	
	}else if(vel<0){
		digitalWrite(forwardPin,LOW);
		digitalWrite(backwardPin,HIGH);		
		power = lowPowerNeg;		
	}
	
	// Set speed
	if(vel==0){	
		digitalWrite(forwardPin,LOW);
		digitalWrite(backwardPin,LOW);
		analogWrite(speedPin,0);
	}else{
		int powerFinal=map(abs(vel),0,maxSpeed,lowPowerPos,highPower);
		analogWrite(speedPin,powerFinal);		
	}

}

// Finns annat sätt att koda, behövs detta?!?!?
int RotDriver::readConstrained() {
  return map(constrain(analogRead(sensorPin), sensorMin, sensorMax), sensorMin, sensorMax, 0, 255);
}
