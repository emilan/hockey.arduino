#include "Feedback.h"
Feedback::Feedback(int _Kp,int _Ki,int _Kd,int _interval){
	interval=_interval;
	Ki=_Ki;
	Kp=_Kp;
	Kd=_Kd;
}
int Feedback::update(float rotSpeed,float delta){
	float absSpeedfrac=abs(rotSpeed)/interval;
	float a=-5;
	float b=-5;
	float c=0;
	time=millis();
	float deltaTime=(time-lastTime)*0.001;
	I+=delta*deltaTime;
	I=constrain(I,-255/absSpeedfrac,255/absSpeedfrac);
	//I*=pow(0.99,1/deltaTime);
	float P=delta;
	float D=(delta-lastDelta)/deltaTime;

	float spe=-absSpeedfrac*(Ki*I+Kp*P+Kd*D);
	if(abs(P)<5){
		spe=0;
	}
	/*Serial.print(I);
	Serial.print("\t");
	Serial.print(P);
	Serial.print("\t");
	Serial.print(D);
	Serial.print("\t");
	Serial.println(constrain(spe,-255,255));*/
		

	lastTime=time;
	lastDelta=delta;
	return constrain(spe,-interval,interval);
}
void Feedback::reset(){
	I=0;
	lastDelta=0;
}
