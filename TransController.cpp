#include "TransController.h"

TransController::TransController() {

}

TransController::TransController(int _id, float _Kp, float _Ki) {

	id = _id;
	Kp=_Kp;
	Ki=_Ki;
	
	powerMax = 255;
	powerMin = -255;
	
	reset();
	
}

int TransController::update(int speed, int error){
	
	int time = millis();	
	float deltaTime = 0;
	
	if(lastTime) {
		deltaTime = (time-lastTime)*0.001;
	}
	
	float spw = 1.0f*speed/powerMax; // spw = shall be speed dependant
	float siw = 1.0f*(1/(speed+1)); // siw = shall be speed dependant
	
	/*
	float powerDelta = 1.0f*spw*(Kp*(error-lastError)+Ki*deltaTime*error);
			
	float power = constrain(lastPower+powerDelta,powerMin,powerMax);
	*/
	  
	ITerm += error;  
	
	float power = spw * Kp * error + siw * Ki * ITerm;
	power = constrain(power,powerMin,powerMax);
	
	if(abs(error) <= 2) {
		power = 0;
		ITerm = 0;
	}
	
	lastTime = time;
	lastError = error;
	lastPower = power;
	
	return power;
	
}

void TransController::reset() {
	
	lastTime = 0;
	lastError = 0;
	lastPower = 0;
	
	ITerm = 0;
	
}
