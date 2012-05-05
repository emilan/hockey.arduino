#include "TransDriver.h"

TransDriver::TransDriver(){
	;
}

TransDriver::TransDriver(int _id){
	
	id=_id;
	
	forwardPin=22+4*id;
	backwardPin=23+4*id;	
	pinMode(forwardPin,OUTPUT);
	pinMode(backwardPin,OUTPUT);
	
	speedPin=2+2*id;
	sensorPin=2*id;

	highPower=255;
	maxSpeed=254;
	
	lowPowerPos=EEPROM.read(7*id);
	sensorMax=4*EEPROM.read(7*id+1);
	lowPowerNeg=EEPROM.read(7*id+2);
	sensorMin=4*EEPROM.read(7*id+3);
	
}

bool TransDriver::calibrate(){

	int length = 5;
	int delayTime = 1000;
	int lastValue = 0; 
	
	// Find lowPowerPos	
	lastValue = analogRead(sensorPin);
	
	digitalWrite(forwardPin,1);
	digitalWrite(backwardPin,0);
		
	for(int i=50;i<255;i++){
	
		if(abs(analogRead(sensorPin)-lastValue)>length){
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
		
	// Find sensorMax	
	sensorMax=0;
	int newVal=10;
		
	analogWrite(speedPin,constrain(lowPowerPos+100,0,255));
		
	while(newVal>sensorMax) {		
		sensorMax=newVal;		
		delay(delayTime);			
		newVal=analogRead(sensorPin);
	}
		
	analogWrite(speedPin,0);
	delay(500);
		
	// Find lowPowerNeg
	lastValue = analogRead(sensorPin);
	
	digitalWrite(forwardPin,0);
	digitalWrite(backwardPin,1);
		
	for(int i=30;i<255;i++){
	
		if(abs(analogRead(sensorPin)-lastValue)>length){
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
	
	// Find sensorMin
	
	newVal=sensorMax;	
	sensorMin=1025;
	
	analogWrite(speedPin,constrain(lowPowerNeg+100,0,255));
			
	while(newVal<sensorMin){
		sensorMin=newVal;							
		delay(delayTime);
		
		newVal=analogRead(sensorPin);
	}
	
	analogWrite(speedPin,0);
	delay(500);
		
	return true;
}

// Activate motor with chosen duty cycle
void TransDriver::setSpeed(int vel){
	
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
		
		/*if(finalPower == 255) {
			Serial.print("FULL POWER player ");
			Serial.println(id);
		}*/

		analogWrite(speedPin,finalPower);	
		
	}

}

int TransDriver::readConstrained() {
  return map(constrain(analogRead(sensorPin), sensorMin, sensorMax), sensorMin, sensorMax, 0, 255);
}

bool TransDriver::reset() {
	return true;
}
