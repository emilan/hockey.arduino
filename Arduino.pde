#include <EEPROM.h>
#include "Player.h"

#define TEAMID "t0"

Player players[6];					// Player array

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

	return res;
}

// System setup
void setup(){

	Serial.begin(115200);				// Begin serial, 115kBaud
	
	for(int i=0;i<6;i++){				// Player init
		
		players[i]=Player(i);
		
		players[i].transDestination=0;
		players[i].transSpeed=0;
		players[i].rotDestination=0;
		players[i].rotSpeed=0;
		
	}
	
}

char mode = '\0';

void update() {
	for(int i=0;i<6;i++)	
		players[i].update();					// Uppdaterar styrsignal
}

// Main loop
void loop() {

	if(Serial.available() > 0) {		// Wait for message
		
		delay(1);						// Delay 1 ms
		mode=Serial.read();
		
		if(mode=='i'){					// Indetification Mode, In use ?!?!?
			
			Serial.println(TEAMID);
		
		}else if(mode=='c'){			// Command Mode
			
			while(Serial.available() >=5 ) {		// Complete command?
				
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
			update();

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
			
			if (pNumber >=0 && pNumber < 6) {	
				Serial.println("Setting state...");
				players[pNumber].setState(transSpeed, transDestination, rotSpeed, rotDestination);
			}
			
		} else if(mode=='a') {
			
			if(Serial.available() >= 1) {
			
				byte pNumber = getByte();
				
				if (pNumber >=0 && pNumber < 6) {	
				
					players[pNumber].reset();
					
					Serial.print("Calibrating player ");
					Serial.print(pNumber);
					Serial.println("...");
					
					players[pNumber].calibrate();
					players[pNumber].saveCalibration();
				
					Serial.println("Done!");
					
				}				
			
			} else {
						
				for(int i=0;i<6;i++){
				
					players[i].reset();
					
					Serial.print("Calibrating player ");
					Serial.print(i);
					Serial.println("...");
					
					players[i].calibrate();
					players[i].saveCalibration();
					
					Serial.println("Done!");
					
				}
				
			}

		}
		
		Serial.flush();
		
	}
	if (mode == 'd')
		update();
}

void sendState(){								// Konstuerar ett meddelande och skickar det
	
	int index = 0;
	byte toSend[12];
	
	for(int i=0;i<6;i++){
		toSend[index++]=players[i].getTrans();
		toSend[index++]=players[i].getRot();
	}
	
	Serial.write(toSend,12);
}



