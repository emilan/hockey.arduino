#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif"

class RotController{
	
	int id;
	float Ki,Kp;
	int powerMax;
	int powerMin;
	
	int lastTime;	
	int lastError;
	float lastPower;
	float ITerm;
	
public:

	RotController();
	RotController(int _id, float _Kp, float _Ki);
	
	int update(int speed, int error);	
	void reset();
};


	