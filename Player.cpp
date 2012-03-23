#include "Player.h"

// Control parameters
int feedbackParameters[6][6] = 
	{ {2,2,0,2,2,0},
	  {2,2,0,2,2,0},
	  {2,2,0,2,2,0},
	  {2,2,0,2,2,0},
	  {2,2,0,2,2,0},
	  {2,2,0,2,2,0} };

Player::Player(){
	;
}

Player::Player(int _id){

	id=_id;
	trans=TransDriver(id);
	rotation=RotDriver(id);

	correctHalfTurn=true;
	rotController=RotController(feedbackParameters[id][0],feedbackParameters[id][1],feedbackParameters[id][2],127);
	transController=TransController(feedbackParameters[id][3],feedbackParameters[id][4],feedbackParameters[id][5],254);
}

void Player::update(){
	updateTranslation();
	updateRotation();
}

void Player::updateRotation(){
	
	rot = rotation.readConstrained() - rotation.zeroAngle;
	
	int cwError = rotDestination - rot;				//mellan -255 och 510   '+???'
	if (cwError < 0)
		cwError += 255;
	
	/*
	deltaRot+=255*((deltaRot<0)-(deltaRot>255));	//mellan 0 och 255
	
	if((deltaRot-127)*(lastDeltaRot-127)<0){		//change to correct halfturn if crossed the 180 degree mark
		correctHalfTurn=true;
	}
	
	lastDeltaRot=deltaRot;							//updaterar f�rra finkeln
	
	deltaRot-=255*((correctHalfTurn&&deltaRot>127)||((!correctHalfTurn)&&deltaRot<127));		//placerar felet p� r�tt varv beroende p� rotationsriktning 
	*/
	
	float spe = rotController.update(rotSpeed, cwError);
	rotation.setSpeed(spe);
}

void Player::updateTranslation(){
	lastPos=pos;
	pos = trans.readConstrained();
	
	float deltaPos=pos-transDestination;
	float spe=transController.update(transSpeed,deltaPos);//-abs(transSpeed)* constrain(deltaPos/proximity,-1,1);//skall �terkopplas
	trans.setSpeed(spe);
}

void Player::setState(byte _transSpeed, byte _transDestination, char _rotSpeed, byte _rotDestination){
	
	// Reset for new movement
	transController.reset();			//?!?!?!
	rotController.reset();
	
	transSpeed = _transSpeed;
	transDestination = _transDestination;
	
	rotSpeed =_rotSpeed;
	rotDestination = _rotDestination;
	
	//avg�r om felet �r den stora eller lilla vinkeln
	rot = rotation.readConstrained(); // - rotation.zeroAngle;
	float deltaRot=rotDestination-rot;
	deltaRot+=255*(deltaRot<0);
	lastDeltaRot=deltaRot;
	deltaRot-=255*(deltaRot>127);
	
	correctHalfTurn=deltaRot*rotSpeed>0;
}

bool Player::calibrate(){
	
	rotation.calibrate();
	trans.calibrate();
	
	return true;
	
}

void Player::saveCalibration(){					// Store calibration
	EEPROM.write(3*id,trans.sensorMin/4);		//g�r om v�rderna till 8bit precision
	EEPROM.write(3*id+1,trans.sensorMax/4);
	EEPROM.write(3*id+2,rotation.zeroAngle);
}

void Player::reset(){
	trans.sensorMax=0;
	trans.sensorMin=1023;
	rotation.zeroAngle=0;
}

byte Player::getPos(){
	return pos;
}

byte Player::getRot(){
	return rot;
}