/* 

FIRMWARE TO CONFIG HC 06 FOR COMPATIBLE ZOWI APP
After upload code you have 10 seconds to connect the module
HC06---- Arduino
TX ----> RX (D6)
RX <---- TX (D7)
Turn on  Serial MOnitor (Baud: 9600) to view Process, if configuration unsuccess, please try another baurate
Once the LED off configuration will start and at the end LED will blink
After this you can pair your module////////////////////////////////////////////////////////////
///////////////////////////////////////
////////////////////////////////////////
CHƯƠNG TRÌNH NÀY ĐƯỢC VIẾT RA ĐỂ CONFIG CHO MODULE BLUETOOTH HC-06 GỐC ĐỂ ROBOT ZOWI CÓ THỂ TƯƠNG THÍCH VỚI ANDROID ZOWI APP 
LUU Ý: VỚI MODULE ĐI KÈM THEO COMBO DO STEMBOT.VN CUNG CẤP, MODULE HC-06 ĐÃ ĐƯỢC CONFIG SẴN, KHÔNG CẦN PHẢI LOAD CHƯƠNG TRÌNH NÀY
SỬ DỤNG:
UPLOAD CHƯƠNG TRÌNH VÀO ROBOT
BẬT SERIAL MONITOR LÊN ĐỂ THEO DÕI QUÁ TRÌNH. 

Created by Hien Phan, Aug 2017
*/
// Options:
//-- Otto Library
#include <Otto.h>
#include <SoftwareSerial.h>
#define BUFFER_SIZE 64
// --- For BufferSize for messages ---
char inData[BUFFER_SIZE];
char inChar=-1;
int count=0;
int i=0;
// ---- Settings for Arduino ----
int ArduLED=13;				// Internal Arduino´s LED
char ssid[10]		= "Zowi";	// Name for Bluetooth.
char baudios		 = '4';		   // 1=>1200 baudios, 2=>2400, 3=>4800, 4=>9600 (default), 5=>19200, 6=>38400, 7=>57600, 8=>115200
char password[10]	= "1234";		// Password for pairing

//-- Movement parameters
int T=1000;              //Initial duration of movement -> show the speed of movement
int moveId=0;            //type of movement
int moveSize=30;         //Size of movement (amplitude of movement)

int speakerPin = 13;
int length = 28; // the number of notes
char notes[] = "GGAGcB GGAGdc GGxecBA yyecdc";
int beats[] = { 2, 2, 8, 8, 8, 16, 1, 2, 2, 8, 8,8, 16, 1, 2,2,8,8,8,8,16, 1,2,2,8,8,8,16 };
int tempo = 150;

Otto Ottobot;  //my name is Ottobot! Hello World!
SoftwareSerial BT(6,7); //RX,TX

#define HIP_L   2        // connect Servo Hip left to D2
#define FOOT_L  4        // Connect Servo Foot Left to D4
#define HIP_R   3        // Connect Servo Hip right to D3
#define FOOT_R  5        // COnnect Servo Foot Right to D5

void setup()
{
	BT.begin(9600);		//9600bauds is the deafult baudrate for these modules.
					//if it´s not working try changing this baudrate to match your HC-06 initial setup
	Serial.begin(9600);
	    // Waiting time (10 seconds) onboard LED is ON:
  Serial.println("CONFIG HC06 BLUETOOTH MODULE");
  Ottobot.setTrims(0,0,0,0);
  Ottobot.saveTrimsOnEEPROM(); // NOTE: comment out this line if you are using calibration function from Zowi App
  Ottobot.init(HIP_L, HIP_R, FOOT_L, FOOT_R);
  Ottobot.home();
  delay(50);
  Serial.println("Zowi Started");
  Ottobot.walk(1,T,1);
//	pinMode(ArduLED,OUTPUT);
//	digitalWrite(ArduLED,HIGH);
//	delay(1000);
//	digitalWrite(ArduLED,LOW);
//	
//  BT.print("AT"); 
//  waitResponse();
//  //delay(1000); // Now configuration start
//  Serial.println("SET NAME: Zowi");
//  BT.print("AT+NAME"); BT.print(ssid); delay(1000);   // Change Name of BT
//  waitResponse();
//  Serial.println("SET BAURATE: 9600");
//  
//  BT.print("AT+BAUD"); BT.print(baudios); delay(1000);    // Change Baud
//  waitResponse();
//  Serial.println("SET PASS: 1234");
//  
//  BT.print("AT+PIN"); BT.print(password); delay(1000);	    // Change Password
//  waitResponse();
}

void loop()
{
	// After programing bluetooth, onboard LED will Blink.
  // digitalWrite(ArduLED, !digitalRead(ArduLED));

  // This will prevent bufferoverrun errors
  byte byte_count=BT.available();
  if(byte_count)
  {
    Serial.println("Incoming Data");
    int first_bytes=byte_count;
    int remaining_bytes=0;
    if(first_bytes>=BUFFER_SIZE-2)
    {
      remaining_bytes=byte_count-(BUFFER_SIZE-2);
    }
    for (i=0;i<first_bytes;i++)
    {
      inChar=BT.read();
      inData[i]=inChar;
    }
    inData[i]='\0';
    for(i=0;i<remaining_bytes;i++) //This burns off (discarded) any remaining bytes that the buffer cannot handle (for some reason)
    {
      inChar=BT.read();
    }
//    if (Ottobot.getRestState()==false) {
//      move(inData);
//    }
    move(inData[0]); // The string will be like "Command: D"
    Serial.println(inData[0]);
    BT.println("I am performing command: "+String(char(inData[0])));
    count++;
  }
	delay(100);
}

void playTone(int tone, int duration) {

  for (long i = 0; i < duration * 1000L; i += tone * 2) {
     digitalWrite(speakerPin, HIGH);
     delayMicroseconds(tone);
     digitalWrite(speakerPin, LOW);
     delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {

  char names[] = {'C', 'D', 'E', 'F', 'G', 'A', 'B',           
                   'c', 'd', 'e', 'f', 'g', 'a', 'b',
                   'x', 'y' };
  
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014,
                   956,  834,  765,  593,  468,  346,  224,
                   655,  715 };
  
  int SPEE = 5; // play the tone corresponding to the note name
  
  for (int i = 0; i < 17; i++) {
     if (names[i] == note) {
      int newduration = duration/SPEE;
       playTone(tones[i], newduration);
     }
  }
}

//-- Function to execute the right movement according the movement command received.
void move(int moveId){

  bool manualMode = false;

  switch (moveId) {
    case 0:
      Ottobot.home();
      break;
    case 1: //M 1 1000 
      Ottobot.walk(1,T,1);
      break;
    case 2: //M 2 1000 
      Ottobot.walk(1,T,-1);
      break;
    case 3: //M 3 1000 
      Ottobot.turn(1,T,1);
      break;
    case 4: //M 4 1000 
      Ottobot.turn(1,T,-1);
      break;
    case 5: //M 5 1000 30 
      Ottobot.updown(1,T,30);
      break;
    case 6: //M 6 1000 30
      moveSize =30;
      Ottobot.moonwalker(1,T,moveSize,1);
      break;
    case 7: //M 7 1000 30
      moveSize =30;
      Ottobot.moonwalker(1,T,moveSize,-1);
      break;
    case 8: //M 8 1000 30
      moveSize =30;
      Ottobot.swing(1,T,moveSize);
      break;
    case 9: //M 9 1000 30 
    moveSize =30;
      Ottobot.crusaito(1,T,moveSize,1);
      break;
    case 10: //M 10 1000 30
      moveSize =30; 
      Ottobot.crusaito(1,T,moveSize,-1);
      break;
    case 11: //M 11 1000 
      Ottobot.jump(1,T);
      break;
    case 12: //M 12 1000 30
    moveSize =30;
      Ottobot.flapping(1,T,moveSize,1);
      break;
    case 13: //M 13 1000 30
    moveSize =30;
      Ottobot.flapping(1,T,moveSize,-1);
      break;
    case 14: //M 14 1000 20
    moveSize = 20;
      Ottobot.tiptoeSwing(1,T,moveSize);
      break;
    case 15: //M 15 500 
      Ottobot.bend(1,T,1);
      break;
    case 16: //M 16 500 
      Ottobot.bend(1,T,-1);
      break;
    case 17: //M 17 500 
      Ottobot.shakeLeg(1,T,1);
      break;
    case 18: //M 18 500 
      Ottobot.shakeLeg(1,T,-1);
      break;
    case 19: //M 19 500 20
      moveSize =30;
      Ottobot.jitter(1,T,moveSize);
      break;
    case 20: //M 20 500 15
      moveSize =30;
      Ottobot.ascendingTurn(1,T,moveSize);
      break;
    default:
        manualMode = true;
      break;
  }
  Ottobot.home();
//
//  if(!manualMode){
//    sendFinalAck();
//  }
       
}

void happy_dance()
{
  // to dance repeatedly:
  Ottobot.home();
  Ottobot.jitter(2,750,20);
  Ottobot.crusaito(1,800,30,1);
  Ottobot.crusaito(1,800,30,-1);
  Ottobot.crusaito(1,800,30,1);
  delay(300);
  Ottobot.walk(1,1500,-1);
  Ottobot.walk(2,1000,1);
  Ottobot.moonwalker(1,600,30,1);
  Ottobot.moonwalker(1,600,30,-1);
  Ottobot.moonwalker(1,600,30,1);
  Ottobot.moonwalker(1,600,30,-1);
  Ottobot.walk(1,1500,-1);
  Ottobot.walk(2,1000,1);
  Ottobot.moonwalker(1,600,30,1);
  Ottobot.moonwalker(1,600,30,-1);
  Ottobot.moonwalker(1,600,30,1);
  Ottobot.moonwalker(1,600,30,-1);
  Ottobot.walk(1,1500,-1);
  Ottobot.walk(2,1000,1);
  Ottobot.shakeLeg(1,700,1);
  Ottobot.shakeLeg(1,700,-1);
  delay(1000);
  Ottobot.home();
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.moonwalker(1,3000,50,-1);
  delay(100);
  Ottobot.moonwalker(1,3000,50,-1);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  delay(500);
  Ottobot.crusaito(1,800,30,1);
  Ottobot.crusaito(1,800,30,-1);
  Ottobot.crusaito(1,800,30,1);
  delay(300);
  Ottobot.walk(1,1500,-1);
  Ottobot.walk(2,1000,1);
  Ottobot.moonwalker(1,600,30,1);
  Ottobot.moonwalker(1,600,30,-1);
  Ottobot.moonwalker(1,600,30,1);
  Ottobot.moonwalker(1,600,30,-1);
  Ottobot.walk(1,1500,-1);
  Ottobot.walk(2,1000,1);
  Ottobot.moonwalker(1,600,30,1);
  Ottobot.moonwalker(1,600,30,-1);
  Ottobot.moonwalker(1,600,30,1);
  Ottobot.moonwalker(1,600,30,-1);
  Ottobot.walk(1,1500,-1);
  Ottobot.walk(2,1000,1);
  Ottobot.shakeLeg(1,700,1);
  Ottobot.shakeLeg(1,700,-1);
  delay(1000);
  Ottobot.home();
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.moonwalker(1,3000,50,-1);
  delay(100);
  Ottobot.moonwalker(1,3000,50,-1);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.home();
  delay(100);
}

void happy_dance_2() {
    Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.updown(3,900,30);
  Ottobot.jitter(3,1000,20);
  Ottobot.updown(3,900,30);
  Ottobot.jitter(3,1000,20);
  Ottobot.jump(1,400);
  Ottobot.jump(1,400);
  Ottobot.jump(1,400);
  Ottobot.jump(1,400);
  Ottobot.jitter(4,800,20);
  Ottobot.jump(1,400);
  Ottobot.jump(1,400);
  Ottobot.jump(1,400);
  Ottobot.jump(1,400);
  Ottobot.jitter(2,800,20);
  delay(1500);
  Ottobot.home();
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.moonwalker(1,3000,50,-1);
  delay(100);
  Ottobot.moonwalker(1,3000,50,-1);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.moonwalker(1,3000,50,-1);
  delay(100);
  Ottobot.moonwalker(1,3000,50,-1);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.home();
  Ottobot.updown(3,900,30);
  Ottobot.jitter(3,1000,20);
  Ottobot.updown(3,900,30);
  Ottobot.jitter(3,1000,20);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.moonwalker(1,3000,50,-1);
  delay(100);
  Ottobot.moonwalker(1,3000,50,-1);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.moonwalker(1,3000,50,-1);
  delay(100);
  Ottobot.moonwalker(1,3000,50,-1);
  Ottobot.moonwalker(1,3000,50,1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.jump(1,370);
  Ottobot.bend(1,100,1);
  Ottobot.bend(1,100,-1);
  Ottobot.home();
}

void waitResponse()
{
  // BT.flush();
  String command = "";
  while (BT.available()==0); 
  while(BT.available()) { // While there is more to be read, keep reading.
      command += (char)BT.read();    
     // while (BT.available()==0);
     delay(100);
    } 
   Serial.println(command);  


}
