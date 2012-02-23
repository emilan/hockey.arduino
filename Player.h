#ifdef DEBUG
#include "VSPDE.h"
#else
#include "WProgram.h"
#endif
#include "Feedback.h"

struct Connection{//struct som läser och skriver i hårdvaran
	int forwardPin;
	int backwardPin;
	int speedPin;
	int sensorPin;
	int sensorMin;
	int sensorMax;
	Connection( int id=0,boolean isRot=false);
	boolean calibrate();
	void setSpeed(int vel);
	int lowPowerPos;
	int lowPowerNeg;
	int highPower;
	int maxSpeed;
	boolean isRot;
	int id;
	int zeroAngle;
};
class Player{
	int id;
	Connection trans,rotation;
	int pos,rot;
	int deltaRot;
	int lastDeltaRot;
	boolean correctHalfTurn;
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
	boolean calibrate();
	void diagnostics();
	void reset();
	void saveCalibration();
};
