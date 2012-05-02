#include "Player.h"

// Control parameters
float parameters[6][6] = 
	{ {2,0,0.2,0},
	  {20,0,1,0},
	  {20,0,1,0},
	  {50,0,1,0},
	  {100,0,1,0},
	  {20,0,1,0} };

Player::Player(){

}

Player::Player(int _id){

	id=_id;
		
	// For rot
	overshot = false;
	passedZero = false;
	rotLastDiff = 0;
	
	startTime = 0;
	
	int rot = rotDriver.readConstrained() - rotDriver.zeroAngle; 
		
	if (rot < 0) {
		rotCurrent = rot + 255;
	} else {
		rotCurrent = rot;
	}
	
	rotLast = rotCurrent;  
	
	// Drivers
	transDriver=TransDriver(id);
	rotDriver=RotDriver(id);
	
	//Controllers
	transController=TransController(id,parameters[id][0],parameters[id][1]);
	rotController=RotController(id,parameters[id][2],parameters[id][3]);
	
}

void Player::update(){
	updateTrans();
	updateRot();
}

void Player::updateTrans(){

	int error;

	transCurrent = transDriver.readConstrained();
	
	if(abs(transSpeed) > 0) {
		error = transDestination - transCurrent;
	} else {
		error = 0;
	}
		
	int spe = transController.update(transSpeed, error);
	transDriver.setSpeed(spe);
	
}

void Player::updateRot() {
	int rot = rotDriver.readConstrained() - rotDriver.zeroAngle; 

	if (rot < 0)
		rotCurrent = rot + 255;
	else rotCurrent = rot;
	
	if(abs(rotSpeed) == 127) {

		int time = millis();
	
		if(!startTime) {
			startTime = time;
			rotDriver.setSpeed(rotSpeed);
		}		
	
		if(time - startTime > 500) {
			rotDriver.setSpeed(0);
			startTime = 0;
			rotSpeed = 0;
		}		
	
	} else {
		
		int posError = 0;
		int negError =  0;
		long error = 0;
		
		// Calculate posError and negError
		
		if(rotDestination < rotCurrent) {
		
			posError = rotCurrent - rotDestination;
			negError = -(rotDestination - rotCurrent + 255);
			
		} else if (rotDestination > rotCurrent) {
		
			posError = rotCurrent - rotDestination + 255;
			negError = -(rotDestination - rotCurrent);
		}
		
		if(id == 0) {
		
				Serial.print("CURRENT:");
				Serial.println(rotCurrent);
		
				Serial.print("POS_ERROR:");
				Serial.println(posError);
				
				Serial.print("NEG_ERROR:");
				Serial.println(negError);
				
			}
			
		// Check for overshot (and passedZero)
			
		if((rotLast > 205 && rotCurrent < 50) || (rotLast < 50 && rotCurrent > 205)) { 
			passedZero = true;
			
			if(id == 0) {
				Serial.println("passedZero!");
			}
		}
		
		long diff = rotDestination - rotCurrent;
		
		if(rotLastDiff * diff < 0 && !passedZero) {
			overshot = !overshot;
			
			if(id == 0) {
				Serial.println("overshot1!");
			}
			
		} else if (rotLastDiff * diff > 0 && passedZero) {
			overshot = !overshot;
			
			if(id == 0) {
				Serial.println("overshot2!");
			}
		}

		rotLast = rotCurrent;
		
		if (diff != 0) {
			rotLastDiff = diff;	
		}
		
		// Choose correct error		
		
		if(!overshot) {
		
			if(rotSpeed > 0) {
				error = posError;
			} else if (rotSpeed < 0) {
				error = negError;
			} else {
				error = 0;
			}
		
		} else {
		
			if(abs(posError) < abs(negError)) {
				error = posError;
			} else {
				error = negError;
			}
			
		}
		
		/*
		
		// Error correction for rotation direction and overshoot.
		if(rotSpeed > 0) {

			if(error < 0 && !overshot) {
				error = error + 255;
			} else if(passedZero && overshot) {
				
				if(rotCurrent < rotDestination) {
					error = error - 255;
				} //annars, om rotCur > rotDest, error = error
			}
		
		} else if(rotSpeed < 0) {
		
			if(error > 0 && !overshot) {
				error = error - 255;
			} else if(passedZero && overshot) {
			
				if(rotCurrent > rotDestination) {
					error = error + 255;
				} //annars, om rotCur < rotDest, error = error
			}

		} else {	
			error = 0;		
		}
		*/

		int speed = abs(rotSpeed);
		
		int spe = rotController.update(speed, error);
		rotDriver.setSpeed(spe);
		
		passedZero = false;
		
		if(id == 0) {
		
			Serial.print("ERROR:");
			Serial.println(error);
		
		}
		
	}
		
}

void Player::setState(byte _transSpeed, byte _transDestination, char _rotSpeed, byte _rotDestination){
	
	// Reset for new movement
	reset();
	
	// Save new state to variables	
	transSpeed = _transSpeed;
	transDestination = _transDestination;
	
	rotSpeed =_rotSpeed;
	rotDestination = _rotDestination;
		
}

bool Player::calibrate(){
	
	transDriver.calibrate();
	rotDriver.calibrate();
	
	return true;
	
}

bool Player::saveCalibration(){

	//Store calibration
	EEPROM.write(7*id,transDriver.lowPowerPos);		
	EEPROM.write(7*id+1,transDriver.sensorMax/4); // Change 10-bit to 8-bit value
	EEPROM.write(7*id+2,transDriver.lowPowerNeg);
	EEPROM.write(7*id+3,transDriver.sensorMin/4); // Change 10-bit to 8-bit value
	EEPROM.write(7*id+4,rotDriver.lowPowerPos);
	EEPROM.write(7*id+5,rotDriver.lowPowerNeg);
	EEPROM.write(7*id+6,rotDriver.zeroAngle);

	return true;
	
}

bool Player::reset(){

	transDriver.reset();
	rotDriver.reset();
	
	transController.reset();
	rotController.reset();
	
	overshot = false;
	passedZero = false;
	rotLastDiff = 0;
	
	return true;
	
}

byte Player::getTrans(){
	return transCurrent;
}

byte Player::getRot(){
	return rotCurrent;
}