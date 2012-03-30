#include "WProgram.h"

class RotController{
	
	float Ki,Kp;
	int powerMax;
	int powerMin;
	
	int lastTime;	
	//int lastError;
	int lastPower;
	
public:
	int lastError;

	RotController();
	RotController(float _Kp, float _Ki);
	int update(int rotSpeed, int cwError);
	
	void reset();
};


	