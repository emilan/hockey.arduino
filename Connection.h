#include "EEPROM.h"
#include "WProgram.h"

class Connection{
	int lowPowerPos;
	int lowPowerNeg;
	int highPower;
	int maxSpeed;
	bool isRot;
	int id;
	
public:
	int forwardPin;
	int backwardPin;
	int speedPin;
	int sensorPin;
	int sensorMin;
	int sensorMax;
	int zeroAngle;

	Connection();
	Connection(int _id, bool _isRot);
	bool calibrate();
	void setSpeed(int vel);
};