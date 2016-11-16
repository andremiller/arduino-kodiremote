// An Infrared Remote receiver that emulates a USB keyboard, to be used as a remote receiver for Kodi
// By Andre Miller - www.andremiller.net
// 2016-11-16
// For Arduino Pro Micro
// Board chosen is Arduino IDE Leonardo
// Requires IRRemote library by shirrif
// Hardware connections:
//     LED on Pin 8 (+) and Pin 9 (-), with suitable resistor
//     IR Receiver on Pins GND, VCC and A2

// Uncomment this line to run in test mode.
// In test mode the hex codes of buttons pressed on your remote will be printed to the serial console
//#define TESTMODE

#include <IRremote.h>
#include "Keyboard.h"
int recvPin = A2;
IRrecv irrecv(recvPin);
unsigned long ledTimer;

void setup() {

  irrecv.enableIRIn();  // Start the receiver
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(8, LOW);
  digitalWrite(9, LOW);

#ifdef TESTMODE
  // In test, start the serial and wait for serial console to connect
  Serial.begin(9600);   // Status message will be sent to PC at 9600 baud
  while (!Serial) ; // Wait for serial to be connected. Required on Leonardo with integrated USB
  Serial.println("Begin test mode");
#else
  // In normal mode, emulate a keyboard
  Keyboard.begin();
#endif

  // Flash LED
  for (int i = 0; i < 3; i++) {
    digitalWrite(8, HIGH);
    delay(250);
    digitalWrite(8, LOW);
    delay(250);
  }
}

void loop() {
  decode_results  results;        // Somewhere to store the results
  if (irrecv.decode(&results)) {  // Grab an IR code
#ifdef TESTMODE
    // In test mode, print the HEX code on the serial monitor
    Serial.print(results.decode_type, HEX);
    Serial.print(" - ");
    Serial.println(results.value, HEX);
#else
    // In normal mode, press a key on the keyboard
    if (results.decode_type == 0x03) { // Change the decode type to your remote type (see test mode)
      digitalWrite(8, HIGH);
      ledTimer = millis();
      // Change the values to match your remote (see test mode)
      switch (results.value) {
        case 0x80E848B7 : Keyboard.write(KEY_LEFT_ARROW); break; // Left
        case 0x80E828D7 : Keyboard.write(KEY_RIGHT_ARROW); break;// Right
        case 0x80E88877 :  Keyboard.write(KEY_UP_ARROW); break; // Up
        case 0x80E8A857 :  Keyboard.write(KEY_DOWN_ARROW); break; // Down
        case 0x80E8C837 :  Keyboard.write(KEY_RETURN); break; // OK
        case 0x80E86897 :  Keyboard.write(KEY_BACKSPACE); break; // Exit
        case 0x80E8B24D :  Keyboard.write('i'); break; // NICAM
        case 0x80E8708F :  Keyboard.write('c'); break; // Menu
        case 0x80E8A659 :  Keyboard.write('r'); break; // Rewind
        case 0x80E826D9 :  Keyboard.write('f'); break; // Fast Forward
        case 0x80E806F9 :  Keyboard.write(' '); break; // Play (Does play/pause)
        //case 0x80E88679 :  Keyboard.write(''); break; // Record
        case 0x80E8C639 :  Keyboard.write('x'); break; // Stop
        case 0x80E816E9 :  Keyboard.write(' '); break; // Pause (Does play/pause)
        //case 0x80E8B04F :  Keyboard.write(''); break; // TV/AV
        case 0x20DF40BF :  Keyboard.write('+'); break; // Vol Up
        case 0x20DFC03F :  Keyboard.write('-'); break; // Vol Down
        case 0x80E8926D :  Keyboard.write(KEY_PAGE_UP); break; // Prog Up
        case 0x80E852AD :  Keyboard.write(KEY_PAGE_DOWN); break; // Prog Down
        case 0x80E858A7 :  Keyboard.write(KEY_F8); break; // Mute
        case 0x80E8D827 :  Keyboard.write('t'); break; // LCR (Toggle Subtitles)
        case 0x80E8B847 :  Keyboard.write('w'); break; // Button below mute (Mark Watched)
        case 0x80E87887 :  Keyboard.write('t'); break; // Button A (toggle subtitles)
        //case 0x80E858A7 :  Keyboard.write(''); break; // Button B
        //case 0x80E838C7 :  Keyboard.write(''); break; // Button C
        //case 0x80E87887 :  Keyboard.write(''); break; // Button D
        case 0x80E8C03F :  Keyboard.write('1'); break; // 1
        case 0x80E820DF :  Keyboard.write('2'); break; // 2
        case 0x80E8A05F :  Keyboard.write('3'); break; // 3
        case 0x80E8609F :  Keyboard.write('4'); break; // 4
        case 0x80E8E01F :  Keyboard.write('5'); break; // 5
        case 0x80E810EF :  Keyboard.write('6'); break; // 6
        case 0x80E8906F :  Keyboard.write('7'); break; // 7
        case 0x80E850AF :  Keyboard.write('8'); break; // 8
        case 0x80E8D02F :  Keyboard.write('9'); break; // 9
        case 0x80E830CF :  Keyboard.write('0'); break; // 0
        //case 0x80E8F20D :  Keyboard.write(''); break; // 12

        case 0xFFFFFFFF :  break; // Repeat
        default : break;
      }
    }
#endif
    irrecv.resume();
  }
  if (millis() - ledTimer >= 100) {
    digitalWrite(8, LOW);
  }
}
