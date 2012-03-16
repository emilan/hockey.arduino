#include "TransDriver.h"
#include "RotDriver.h"
#include "Feedback.h"

class Player{
	int id;
	TransDriver trans;
	RotDriver rotation;
	int pos,rot;
	int deltaRot;
	int lastDeltaRot;
	bool correctHalfTurn;
	int lastPos,lastRot;
	float proximity;
	Feedback rotFeedback;
	Feedback transFeedback;
	
public:
	int transDestination,rotDestination;
	float transSpeed,rotSpeed;
	Player();
	Player(int _id);
	void update();
	void updateRotation();
	void updateTranslation();
	void setState(byte transSpeed,byte transDestination,char rotSpeed,byte rotDestination);
	byte getPos();
	byte getRot();
	bool calibrate();
	void diagnostics();
	void reset();
	void saveCalibration();
};
