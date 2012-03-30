#include "Player.h"

// Control parameters
int feedbackParameters[6][6] = 
	{ {2,2,2,2},
	  {2,2,2,2},
	  {2,2,2,2},
	  {2,2,2,2},
	  {2,2,2,2},
	  {2,2,2,2} };

Player::Player(){

}

Player::Player(int _id){

	id=_id;
	
	// Drivers
	transDriver=TransDriver(id);
	rotDriver=RotDriver(id);
	
	//Controllers
	transController=TransController(feedbackParameters[id][0],feedbackParameters[id][1]);
	rotController=RotController(feedbackParameters[id][2],feedbackParameters[id][3]);
	
}

void Player::update(){
	updateTrans();
	updateRot();
}

void Player::updateRot(){
	
	rotCurrent = rotDriver.readConstrained() - rotDriver.zeroAngle;
	
	int cwError = rotDestination - rotCurrent;				//mellan -255 och 510   '+???'
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
	rotDriver.setSpeed(spe);
}

void Player::updateTrans(){

	transCurrent = transDriver.readConstrained();
	int error = transCurrent - transDestination;
	
	float spe = transController.update(transSpeed, error);
	transDriver.setSpeed(spe);
}

void Player::setState(byte _transSpeed, byte _transDestination, char _rotSpeed, byte _rotDestination){
	
	// Reset for new movement
	transController.reset();
	rotController.reset();
	
	// Save new state to variables	
	transSpeed = _transSpeed;
	transDestination = _transDestination;
	
	rotSpeed =_rotSpeed;
	rotDestination = _rotDestination;
		
}

bool Player::calibrate(){
	
	rotDriver.calibrate();
	transDriver.calibrate();
	
	return true;
	
}

bool Player::saveCalibration(){					// Store calibration
	EEPROM.write(3*id,transDriver.sensorMin/4);		//g�r om v�rderna till 8bit precision
	EEPROM.write(3*id+1,transDriver.sensorMax/4);
	EEPROM.write(3*id+2,rotDriver.zeroAngle);
	
	// FIXA DETTA!
	
	return true;
	
}

bool Player::reset(){

	transDriver.reset();
	rotDriver.reset();
	
	transController.reset();
	rotController.reset();
	
	// Ska detta vara med i reset???
	
	transCurrent = 0;
	rotCurrent = 0;
	
	/*
	transDriver.sensorMax=0;
	transDriver.sensorMin=1023;
	rotDriver.zeroAngle=0;
	*/
	
	return true;
	
}

byte Player::getTrans(){
	return transCurrent;
}

byte Player::getRot(){
	return rotCurrent;
}