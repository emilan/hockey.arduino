#include "TransDriver.h"
#include "RotDriver.h"
#include "RotController.h"
#include "TransController.h"

class Player{
	int id;
	TransDriver trans;
	RotDriver rotation;
	int pos,rot;
	
	int lastDeltaRot;
	bool correctHalfTurn;
	int lastPos,lastRot;
	float proximity;
	RotController rotController;
	TransController transController;
	
public:
	int deltaRot;
	int transDestination,rotDestination;
	float transSpeed,rotSpeed;
	Player();
	Player(int _id);
	void update();
	void updateRotation();
	void updateTranslation();
	void setState(byte transSpeed,byte transDestination,char rotSpeed,byte rotDestination);
	bool calibrate();
	void saveCalibration();
	byte getPos();
	byte getRot();
	void reset();
};
