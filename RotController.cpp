#include "RotController.h"

RotController::RotController(float _Kp, float _Ki) {

	Kp=_Kp;
	Ki=_Ki;
	
	reset();
	
}

int RotController::update(int speed, int cwError){
	
	int time = millis();	
	int deltaTime = 0;
	
	if(lastTime) {
		deltaTime = (time-lastTime)*0.001;
	}
	
	if(speed < 0) {
		error = 255 - cwError;
	} else {
		error = cwError;
	}
	
	float spw = 1.0f*speed/powerMax; // spw = shall be speed dependant
		
	int powerDelta = spw*Kp*(error-lastError)+Ki*deltaTime*error;
	
	int power = constrain(lastPower+powerDelta,powerMin,powerMax);
	
	if(abs(error)<2){
		power = 0;
	}

	lastTime = time;
	lastError = error;
	lastPower = power;
	
	return power;
	
}

void RotController::reset() {
	
	lastTime = 0;
	lastError = 0;
	lastPower = 0;
	
}
