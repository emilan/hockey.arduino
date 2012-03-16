#include "EEPROM.h"
#include "WProgram.h"

class RotDriver{

	int id;
	int lowPowerPos;
	int lowPowerNeg;
	int highPower;
	int maxSpeed;
	
public:
	int forwardPin;
	int backwardPin;
	int speedPin;
	int sensorPin;
	int sensorMin;
	int sensorMax;
	int zeroAngle;

	RotDriver();
	RotDriver(int _id);
	bool calibrate();
	void setSpeed(int vel);
	int readConstrained();  
};
