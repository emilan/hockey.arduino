#include "Player.h"

Player::Player(){
	;
}

Player::Player(int _id){

	id=_id;
	trans=Connection(id,false);
	rotation=Connection(id,true);
	//proximity=100;
	//lastDeltaRot=0;
	correctHalfTurn=true;
	rotFeedback=Feedback(2,2,0,127);
	transFeedback=Feedback(2,2,0,254);
}

void Player::update(){
	updateTranslation();
	updateRotation();
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

bool Player::calibrate(){
	rotation.calibrate();
	return trans.calibrate();//
	
}

void Player::reset(){
	trans.sensorMax=0;
	trans.sensorMin=1023;
	rotation.zeroAngle=0;
}

void Player::saveCalibration(){					// Store calibration
	EEPROM.write(3*id,trans.sensorMin/4);		//g�r om v�rderna till 8bit precision
	EEPROM.write(3*id+1,trans.sensorMax/4);
	EEPROM.write(3*id+2,rotation.zeroAngle);
}

void Player::diagnostics(){
	int pos=map(analogRead(trans.sensorPin),trans.sensorMin,trans.sensorMax,0,255);
	Serial.println(pos);
	//float spe=-abs(rotSpeed)* constrain(deltaPos/proximity,-1,1);
}