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
		
	sensorMin=4*EEPROM.read(3*id);
	sensorMax=4*EEPROM.read(3*id+1);

	lowPowerPos=0;
	lowPowerNeg=0;
	highPower=255;
	maxSpeed=254;
}

bool TransDriver::calibrate(){
	
	int pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	int firstPos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	
	int length = 0;
	int delayTime = 0;
	
	// Find lowPowerPos
	length = 3;
	delayTime = 400;
	
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
		
	// Max		
	sensorMax=1;
	int newVal=10;
	setSpeed(lowPowerPos);//driver motorn ?t ena h?llet
		
	while(newVal>sensorMax){//v?ntar till sensorv?rdet inte l?ngre ?kar, d?refter definerar det som maximum position
		sensorMax=newVal;
		
		delay(300);
			
		newVal=analogRead(sensorPin);
	}
		
	setSpeed(0);
	delay(500);
		
	// Find lowPowerNeg	
	firstPos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	
	length = 3;
	delayTime = 50;
	
	for(int j=0;j<255;j=j+2){
			
			pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
			setSpeed(-j);
			delay(delayTime);
			
			if(abs(pos-firstPos)>length){
				lowPowerNeg=j;						
				break;
			}
	}
	
	setSpeed(0);
	delay(500);
	
	// Min	
	newVal=sensorMax;	
	sensorMin=newVal+1;
			
	setSpeed(-lowPowerNeg);
			
	while(newVal<sensorMin){
		sensorMin=newVal;
							
		delay(300);
				
		newVal=analogRead(sensorPin);
	}
	
	setSpeed(0);
	delay(500);
		
	return true;
}

// Activate motor with chosen duty cycle
void TransDriver::setSpeed(int vel){
	
	int lowPower = 0;
	
	// Choose direction
	if(vel>0){
		digitalWrite(forwardPin,HIGH);
		digitalWrite(backwardPin,LOW);		
		lowPower = lowPowerPos;	
	}else if(vel<0){
		digitalWrite(forwardPin,LOW);
		digitalWrite(backwardPin,HIGH);		
		lowPower = lowPowerNeg;		
	}
	 
	// Set speed
	if(vel==0){	
		digitalWrite(forwardPin,LOW);
		digitalWrite(backwardPin,LOW);
		analogWrite(speedPin,0);
	}else{
		int finalPower=map(abs(vel),0,maxSpeed,lowPower,highPower);
		analogWrite(speedPin,finalPower);		
	}

}

int TransDriver::readConstrained() {
  return map(constrain(analogRead(sensorPin), sensorMin, sensorMax), sensorMin, sensorMax, 0, 255);
}
