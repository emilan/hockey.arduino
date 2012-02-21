
#include "Player.h"
#include "stdafx.h"


Connection::Connection( int _id,boolean _isRot)
{
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
	lowPower=0;
	highPower=255;
	maxSpeed=127*(1+!isRot);
	zeroAngle=EEPROM.read(3*id+2);;
}
boolean Connection::calibrate(){
	int pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
	int lastPos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);;
	//om rotation, hittar l�gsta kraft som beh�vs f�r att driva motorerna. spelare 0 och 4 saknar vinkelgivare
	//m�jligen skall detta g�ras f�r translation ocks� 
	if(isRot){
		zeroAngle=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
		/*Serial.println(id,DEC);
		for(int i=0;i<255;i++){
			
			pos=map(analogRead(sensorPin),sensorMin,sensorMax,0,255);
			setSpeed(i);
			delay(100);
			if(abs(pos-lastPos)>10){
				lowPower=i;
				
				
				break;
			}
			lastPos=pos;
			
		}
		setSpeed(0);*/
		
	}
	if(!isRot){
		int newVal=analogRead(sensorPin);//sensorv�rde mellan 0 och 1023
		if(newVal>sensorMax){
			sensorMax=newVal;
			//EEPROM.write(2*id,sensorMax);
		}
		if(newVal<sensorMin){
			sensorMin=newVal;
		}
		//sensorMax=1;
		//int newVal=10;
		//setSpeed(-200);//driver motorn �t ena h�llet
		//
		//delay(400);
		//while(newVal>sensorMax){//v�ntar till sensorv�rdet inte l�ngre �kar, d�refter definerar det som maximum position
		//	sensorMax=newVal;
		//	newVal=analogRead(sensorPin);
		//	
		//	delay(100);
		//}

		//newVal=sensorMax;
		//sensorMin=newVal+1;
		//setSpeed(200);
		//delay(400);
		//while(newVal<sensorMin){
		//	sensorMin=newVal;
		//	newVal=analogRead(sensorPin);
		//	delay(100);
		//}
	}
	//setSpeed(0);
	return true;
}
void Connection::setSpeed(int vel){//driver motor angiven med dutycycle
	
	if(vel>0){//avg�r k�rriktningen
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
		int power=map(abs(vel),0,maxSpeed,lowPower,highPower);
		
		analogWrite(speedPin,power);
	}

}

Player::Player(){
	;
}
Player::Player(int _id){

	id=_id;
	trans=Connection(id,false);
	rotation=Connection(id,true);
	//rotation=Connection(24+4*id,25+4*id,2+2*id,3+2*id,0,1023);
	proximity=100;
	lastDeltaRot=0;
	correctHalfTurn=true;
	rotFeedback=Feedback(2,2,0,127);
	transFeedback=Feedback(2,2,0,254);
}
void Player::update(){
	updateTranslation();
	updateRotation();
	
	//sendState();,,
	//Serial.println(pos,DEC);//DEBUGGING
	//setVel(-transSpeed*deltaPos/proximity);
}
void Player::updateRotation(){
	
	rot=map(analogRead(rotation.sensorPin),rotation.sensorMin,rotation.sensorMax,0,255)-rotation.zeroAngle;//mellan -255 och 255
	float deltaRot=rotDestination-rot;//mellan -255 och 510
	deltaRot+=255*((deltaRot<0)-(deltaRot>255));//mellan 0 och 255
	
	if((deltaRot-127)*(lastDeltaRot-127)<0){//change to correct halfturn if crossed the 180 degree mark
		correctHalfTurn=true;
	}
	lastDeltaRot=deltaRot;//updaterar f�rra finkeln
	
	deltaRot-=255*((correctHalfTurn&&deltaRot>127)||((!correctHalfTurn)&&deltaRot<127));//placerar felet p� r�tt varv beroende p� rotationsriktning 
	
	
	float spe=rotFeedback.update(rotSpeed,deltaRot);
	rotation.setSpeed(spe);
	
}
void Player::updateTranslation(){
	lastPos=pos;
	pos=map(analogRead(trans.sensorPin),trans.sensorMin,trans.sensorMax,0,255);
	
	float deltaPos=pos-transDestination;
	float spe=transFeedback.update(transSpeed,deltaPos);//-abs(transSpeed)* constrain(deltaPos/proximity,-1,1);//skall �terkopplas
	trans.setSpeed(spe);
	
}
void Player::setState(byte _transSpeed,byte _transDestination,char _rotSpeed,byte _rotDestination){
	transDestination=_transDestination;
	rotDestination=_rotDestination;
	transSpeed=_transSpeed;
	rotSpeed=_rotSpeed;
	//avg�r om felet �r den stora eller lilla vinkeln
	rot=map(analogRead(rotation.sensorPin),rotation.sensorMin,rotation.sensorMax,0,255);
	float deltaRot=rotDestination-rot;
	deltaRot+=255*(deltaRot<0);
	lastDeltaRot=deltaRot;
	deltaRot-=255*(deltaRot>127);
	
	
	
	correctHalfTurn=deltaRot*rotSpeed>0;
	rotFeedback.reset();
	
}
byte Player::getPos(){
	return pos;
}
byte Player::getRot(){
	return rot;
}
boolean Player::calibrate(){
	rotation.calibrate();
	return trans.calibrate();//
	
}
void Player::reset(){
	trans.sensorMax=0;
	trans.sensorMin=1023;
	rotation.zeroAngle=0;
}
void Player::saveCalibration(){
	EEPROM.write(3*id,trans.sensorMin/4);//g�r om v�rderna till 8bit precision
	EEPROM.write(3*id+1,trans.sensorMax/4);
	EEPROM.write(3*id+2,rotation.zeroAngle);
}
void Player::diagnostics(){
	int pos=map(analogRead(trans.sensorPin),trans.sensorMin,trans.sensorMax,0,255);
	Serial.println(pos);
	//float spe=-abs(rotSpeed)* constrain(deltaPos/proximity,-1,1);
	
	
}
