#include "stdafx.h"
#include "TimerThree.h"
#include "Player.h"

#define TEAMID "t1"
Player players[6];
boolean calibrating=false;
void setup(){
	TimerThree t=TimerThree();         // initialize timer1, and set a 1/2 second period
	t.start();
	t.setPeriod(100);// sätter pmw-period till 100 mikrosekunder


	Serial.begin(115200);//startar seriell anslutning med 115kBaud
	for(int i=0;i<6;i++){//sätter startvärden för spelarna
		players[i]=Player(i);
		players[i].transDestination=0;
		players[i].transSpeed=0;
		players[i].rotDestination=120;
		players[i].rotSpeed=0;
	}
	//uncomment when sensors are installed for autocalibrating game at startup
	
		//for(int i=0;i<6;i++){
		//	players[i].calibrate();
		//}
	
}
void loop(){
	
	
	if(Serial.available()>0){//kollar om meddelande har börjat komma
		
		delay(1);//väntar en millisekund för att mota hela medelandet
		
		char mode=Serial.read();
		if(mode=='i'){//identification
			Serial.println(TEAMID);
			
		}else if(mode=='c'){//command
			if(calibrating){
				calibrating=false;
				for(int i=0;i<6;i++){//updaterar styrsignal
					players[i].saveCalibration();
				}
			}
			while(Serial.available()>=5){//om meddelandet är <5 är det inget kommando
				
				byte pNumber=Serial.read();
				byte transSpeed=Serial.read();
				byte transDestination=Serial.read();
				char rotSpeed=Serial.read();
				byte rotDestination=Serial.read();
				/*Serial.print(transSpeed,DEC);
				Serial.print(" ");
				Serial.print(transDestination,DEC);
				Serial.print(" ");
				Serial.print(rotSpeed,DEC);
				Serial.print(" ");
				Serial.println(rotDestination,DEC);*/

				if(pNumber>=0&&pNumber<6){	
					players[pNumber].setState(transSpeed,transDestination,rotSpeed,rotDestination);
				}
			}
			
			sendState();
			for(int i=0;i<6;i++){//updaterar styrsignal
				players[i].update();
			}
			//players[0].update();
		}else if(mode=='d'){
			players[0].diagnostics();
			
		}else if(mode=='a'){//calibrerar spelet
			if(!calibrating){
				for(int i=0;i<6;i++){
					players[i].reset();
					//players[i].setState(200,127,0,0);
				}
			}
			calibrating=true;
			for(int i=0;i<6;i++){
				players[i].calibrate();
				//players[i].setState(200,127,0,0);
			}
		}
		Serial.flush();//slänger bufferten som inte lästs
		
	}
	
}
void sendState(){//konstuerar ett meddelande och skickar det
	byte toSend[12];
	int index=0;
	for(int i=0;i<6;i++){
		toSend[index++]=players[i].getPos();
		toSend[index++]=players[i].getRot();
	}
	Serial.write(toSend,12);

}



