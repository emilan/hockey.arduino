#if defined(ARDUINO) && ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <EEPROM.h>

class TransDriver{

	int id;
	
	int highPower;
	int maxSpeed;
	
public:
	int forwardPin;
	int backwardPin;
	int speedPin;
	int sensorPin;
	
	int lowPowerPos;
	int sensorMax;
	
	int lowPowerNeg;	
	int sensorMin;
	
	TransDriver();
	TransDriver(int _id);
	
	bool calibrate();
	
	void setSpeed(int vel);
	int readConstrained(); 
	
	bool reset();
};
