
/******************************************************************************* 


     v1.2 (non-latching relay)
     
        - Hold-momentary mode (1 second)
        - Debounce time: 30ms
        - Mute time: 10ms -> switch relay -> 20ms
        - Active-low switch

                ATTINY25/45/85
                    ______
        (RESET) 5 -|1 \/ 8|-   (Vcc)
    (Relay/LED) 3 -|2    7|- 2 (Switch)
         (Mute) 4 -|3    6|- 1 (NC)
          (GND)   -|4____5|- 0 (NC)


          
*********************************************************************************/



const int switch_pin = 2;         // the number of the pushbutton pin
const int coil_pin = 3;           // pin to switch the relay coil
const int mute_pin = 4;           // pin to trigger mute

byte pedalState = LOW;
byte lastReading = HIGH;

void setup() {
  pinMode(switch_pin, INPUT);
  pinMode(coil_pin, OUTPUT);
  pinMode(mute_pin, OUTPUT);

  digitalWrite(coil_pin, LOW);    // set initial pedal state
  digitalWrite(mute_pin, LOW);
}


void loop() {
  static unsigned long debounce_timestamp;
  static unsigned long state_change_timestamp;
  byte currentReading = digitalRead(switch_pin);    // reading is the current state of switch_pin 
  
  if (currentReading !=  lastReading && millis()-debounce_timestamp > 50) {             // if the reading has changed (and it's not in the middle of the 30ms debounce),
    debounce_timestamp = millis();                                                      // set debounce timestamp to current millis
  }
  if (((millis() - debounce_timestamp) > 30) && (currentReading != lastReading)) {      // after 30ms debounce, if reading is still changed:
    if (currentReading == LOW || millis() - state_change_timestamp > 1000)              // if the switch is pressed down, or it's released after being held for 1 second
    {   
      switchPedalState();                       
      state_change_timestamp = millis();          // set state change timestamp to current millis
    }
    lastReading = currentReading;                 // save current reading as lastReading for next time through loop
  }
}


void switchPedalState(){ 
  pedalState = !pedalState;                // change pedal state
  digitalWrite(mute_pin, HIGH);            // mute output of circuit
  delay(4);                                // wait for mute
  digitalWrite(coil_pin, pedalState);      // turn effect on/off
  delay(30);                               // wait 20ms for relay to switch
  digitalWrite(mute_pin, LOW);             // turn mute off
}




















