#include "TransController.h"

TransController::TransController(int _Kp,int _Ki,int _Kd,int _interval){
	interval=_interval;
	Ki=_Ki;
	Kp=_Kp;
	Kd=_Kd;
}

int TransController::update(float speed,float delta){
	
	float absSpeedfrac=abs(speed)/interval;
	
	time=millis();
	
	float deltaTime=(time-lastTime)*0.001;
	
	float P=delta;
	
	I+=delta*deltaTime;
	I=constrain(I,-255/absSpeedfrac,255/absSpeedfrac);
	//I*=pow(0.99,1/deltaTime);
		
	float D=(delta-lastDelta)/deltaTime;

	float spe=-absSpeedfrac*(Ki*I+Kp*P+Kd*D);
	
	if(abs(P)<5){
		spe=0;
	}

	lastTime=time;
	lastDelta=delta;
	return constrain(spe,-interval,interval);
}

void TransController::reset(){
	I=0;
	lastDelta=0;
}
