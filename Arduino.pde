#include "Player.h"
#include "TimerThree.h"

#define TEAMID "t0"

Player players[6];
bool calibrating=false;

void setup(){
	
	TimerThree t=TimerThree();			// Initialize Timer
	t.start();
	t.setPeriod(100);					// PWM dutycycle 100 ms

	Serial.begin(115200);				// Begin serial, 115kBaud

	for(int i=0;i<6;i++){				// Player init
		players[i]=Player(i);
		players[i].transDestination=0;
		players[i].transSpeed=0;
		players[i].rotDestination=120; 	//?!?!?!
		players[i].rotSpeed=0;
	}
	
}

char getChar() {
	char res = 0;
	char sign = 1;

	while (Serial.available() > 0) {
		char c = Serial.read();
		if (c == '-')
			sign = -1;
		else {
			if (c == ' ')
				break;
			res = res * 10 + c - '0';
		}
	}
	res = sign * res;
	//Serial.println(res, DEC);
	return res;
}

byte getByte() {
	byte res = 0;
	while (Serial.available() > 0) {
		char c = Serial.read();
		if (c == ' ')
			break;
		res = res * 10 + c - '0';
	}
	//Serial.println(res, DEC);
	return res;
}

void loop(){
	if(Serial.available() > 0){			// Wait for message
		
		delay(1);						// Delay 1 ms
		char mode=Serial.read();
		
		if(mode=='i'){					// Indetification Mode, In use ?!?!?
			
			Serial.println(TEAMID);
		
		}else if(mode=='c'){			// Command Mode
			
			if(calibrating){
			
				calibrating=false;
				
				for(int i=0;i<6;i++){
					players[i].saveCalibration();
				}
				
			}
			
			while(Serial.available()>=5){		// Complete command?
				
				byte pNumber=Serial.read();
				byte transSpeed=Serial.read();
				byte transDestination=Serial.read();
				char rotSpeed=Serial.read();
				byte rotDestination=Serial.read();

				if(pNumber>=0&&pNumber<6){
					players[pNumber].setState(transSpeed, transDestination, rotSpeed, rotDestination);
				}
			}
			
			sendState();
			
		}else if(mode=='d'){
		
			byte pNumber = getByte();
			byte transSpeed = getByte();
			byte transDestination = getByte();
			char rotSpeed = getChar();
			byte rotDestination = getByte();
			
			Serial.print("Received d: ");
			Serial.print(pNumber, DEC);
			Serial.print('.');
			Serial.print(transSpeed, DEC);
			Serial.print('.');
			Serial.print(transDestination, DEC);
			Serial.print('.');
			Serial.print(rotSpeed, DEC);
			Serial.print('.');
			Serial.println(rotDestination, DEC);
			
			if (pNumber >=0 && pNumber<6) {	
				Serial.println("Setting state...");
				players[pNumber].setState(transSpeed, transDestination, rotSpeed, rotDestination);
			}
			
			// Fulhack f�r errors
			
			mode = Serial.read();
			
			while(mode!='x') {
		
				Serial.println(players[pNumber].rotController.lastError);
				
				for(int i=0;i<6;i++)	
				players[i].update();					// Uppdaterar styrsignal
				
				mode=Serial.read();
			
			}
			
		} else if(mode=='a'){							// Kalibrerar spelet
			
			if(!calibrating){
			
				for(int i=0;i<6;i++){
					players[i].reset();
					players[i].calibrate();
				}
				
			}
			
			calibrating=true;

		}
		
		Serial.flush();
		
	}
	
	for(int i=0;i<6;i++)	
		players[i].update();					// Uppdaterar styrsignal
		
}

void sendState(){								// Konstuerar ett meddelande och skickar det
	
	byte toSend[12];
	int index=0;
	
	for(int i=0;i<6;i++){
		toSend[index++]=players[i].getTrans();
		toSend[index++]=players[i].getRot();
	}
	
	Serial.write(toSend,12);
}



