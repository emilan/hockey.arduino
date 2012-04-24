#include <EEPROM.h>
#include "TransDriver.h"
#include "RotDriver.h"
#include "TransController.h"
#include "RotController.h"

class Player{
	
	int id;
	int startTime;
	
	bool overshot;
	bool passedZero;
	long rotLastError;
	
	// Drivers
	TransDriver transDriver;
	RotDriver rotDriver;
	
	// Controllers
	TransController transController;
	RotController rotController;

	int transCurrent, rotCurrent, rotLast;
	
public:

	int transDestination, rotDestination;
	float transSpeed, rotSpeed;
	
	Player();
	Player(int _id);
	
	byte getTrans();
	byte getRot();
	
	void update();
	void updateRot();
	void updateTrans();
	
	void setState(byte transSpeed, byte transDestination, char rotSpeed, byte rotDestination);
	
	bool calibrate();
	bool saveCalibration();
	
	bool reset();
};
