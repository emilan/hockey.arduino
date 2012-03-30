#include "EEPROM.h"
#include "WProgram.h"

class TransDriver{

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

	TransDriver();
	TransDriver(int _id);
	
	bool calibrate();
	
	void setSpeed(int vel);
	int readConstrained(); 
	
	bool reset();
};
