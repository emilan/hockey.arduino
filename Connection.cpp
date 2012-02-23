#include "Connection.h"

Connection::Connection(){
	;
}

Connection::Connection(int _id, bool _isRot){
	
	id=_id;
	isRot=_isRot;
	
	forwardPin=22+4*id+2*isRot;
	backwardPin=23+4*id+2*isRot;
	speedPin=2+2*id+isRot;
	sensorPin=2*id+isRot;
	
	if(!isRot){
		sensorMin=4*EEPROM.read(3*id);
		sensorMax=4*EEPROM.read(3*id+1);
	}else{
		sensorMin=0;
		sensorMax=1023;
	}
	
	pinMode(forwardPin,OUTPUT);
	pinMode(backwardPin,OUTPUT);
	lowPowerPos=0;
	lowPowerNeg=0;
	highPower=255;
	maxSpeed=127*(1+!isRot);
	zeroAngle=EEPROM.read(3*id+2);
}
bool Connection::calibrate(){
	
	int pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	int firstPos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	int v=0;
	int delayTime = 0;
	
	// Find lowPowerPos
	
	if(isRot){
		v = 5;
		delayTime = 100;
	} else {
		v = 3;
		delayTime = 400;
	}
	
	for(int i=0;i<255;i=i+2){
			
			pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
			setSpeed(i);
			delay(delayTime);
			
			if(abs(pos-firstPos)>v){
				lowPowerPos=i;						
				break;
			}
	}
	
	setSpeed(0);
	delay(500);
	
	if(isRot){ // Rotation
		//zeroAngle=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	
	} else { // Translation
		
		// Maxläge		
		sensorMax=1;
		int newVal=10;
		setSpeed(lowPowerPos);//driver motorn ?t ena h?llet
		
		while(newVal>sensorMax){//v?ntar till sensorv?rdet inte l?ngre ?kar, d?refter definerar det som maximum position
			sensorMax=newVal;
			
			delay(300);
			
			newVal=analogRead(sensorPin);
		}
		
	}
		
	setSpeed(0);
	delay(500);
		
	// Find lowPowerNeg
	
	firstPos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	
	if(isRot){
		v = 5;
		delayTime = 100;
	} else {
		v = 3;
		delayTime = 50;
	}
	
	for(int j=0;j<255;j=j+2){
			
			pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
			setSpeed(-j);
			delay(delayTime);
			
			if(abs(pos-firstPos)>v){
				lowPowerNeg=j;						
				break;
			}
	}
	
	setSpeed(0);
	delay(500);
	
	if(isRot){ // Rotation
		//zeroAngle=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	
	} else { // Translation
		
		// Minläge		
		int newVal=sensorMax;
		sensorMin=newVal+1;
			
		setSpeed(-lowPowerNeg);
			
		while(newVal<sensorMin){
			sensorMin=newVal;
							
			delay(300);
				
			newVal=analogRead(sensorPin);
		}
		
	}
	
	/*int newVal=analogRead(sensorPin);//sensorv?rde mellan 0 och 1023
		if(newVal>sensorMax){
			sensorMax=newVal;
			//EEPROM.write(2*id,sensorMax);
		}
		if(newVal<sensorMin){
			sensorMin=newVal;
		}
		*/
		
	setSpeed(0);
	delay(500);
	
	return true;
}
void Connection::setSpeed(int vel){//driver motor angiven med dutycycle
	
	if(vel>0){//avg?r k?rriktningen
		digitalWrite(forwardPin,HIGH);
		digitalWrite(backwardPin,LOW);
	}else if(vel<0){
		digitalWrite(forwardPin,LOW);
		digitalWrite(backwardPin,HIGH);
	}
	if(vel==0){
		digitalWrite(forwardPin,LOW);
		digitalWrite(backwardPin,LOW);
		analogWrite(speedPin,0);
	}else{
		int power=map(abs(vel),0,maxSpeed,lowPowerPos,highPower);
		
		analogWrite(speedPin,power);
	}

}