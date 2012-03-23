#include "WProgram.h"

class RotController{
	
	float Ki,Kp;
	int powerMax = 127;
	int powerMin = -127;
	
	int lastTime;	
	int lastError;
	int lastPower;
	
public:	
	RotController(float _Kp, float _Ki);
	int update(int rotSpeed, int cwError);
	void reset();
};


	