#ifdef DEBUG
#include "VSPDE.h"
#else
#include "WProgram.h"
#endif
class Feedback{
	int Ki,Kp,Kd;
	float proximity;
	float I;
	int time;
	int lastTime;
	int lastDelta;
	int interval;
public:	
	Feedback(int _Kp=5,int _Ki=5,int _Kd=0,int interval=255);//
	int update(float rotSpeed,float delta);
	void reset();
};