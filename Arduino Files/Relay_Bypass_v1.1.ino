
/* 
     v1.1 (non-latching relay)
     
        - Hold-momentary mode (1 second)
        - Debounce time: 30ms
        - Mute time: 10ms -> switch relay -> 20ms
        - Active-low switch


                    ______
        (RESET) 5 -|1 \/ 8|-   (Vcc)
    (Relay/LED) 3 -|2    7|- 2 (Switch)
         (Mute) 4 -|3    6|- 1 (NC)
          (GND)   -|4____5|- 0 (NC)
          
         -github test-
*/



const int pin_Switch = 2;            // the number of the pushbutton pin
const int pin_Activate = 3;          // pin to switch the "on" coil of the relay
const int pin_Mute = 4;              // pin to trigger mute


int pedalState = LOW;                // starts pedal in bypass mode
int switchstate;                     // the current reading from the input pin
int lastswitchstate = HIGH;          // the previous reading from the input pin



const int debounceDelay = 30;        // the debounce time; increase if the output flickers
const int toggleDelay = 1000;        // time until hold-momentary mode active
const int muteDelay = 20;            // time pedal is muted after relay is switched


unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long lastStateChange = 0;   // the last time the state was changed (effect on or off)


void setup() {
  pinMode(pin_Switch, INPUT);
  pinMode(pin_Activate, OUTPUT);
  pinMode(pin_Mute, OUTPUT);

  // set initial pedal state
  digitalWrite(pin_Activate, pedalState);
  digitalWrite(pin_Mute, LOW);
}

void loop() {
  
  int reading = digitalRead(pin_Switch);    // reading is the current state of pin_Switch 
  
  if (reading != lastswitchstate) {
    lastDebounceTime = millis();            // reset the debouncing time to current time
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {      //wait until timer is 30ms past debouncing time
    
    if (reading != switchstate)                       // if the button state has changed:
    {   
      switchstate = reading;
      
        if (switchstate == LOW)                       // if the switch is currently pressed 
        {   
          pedalState = !pedalState;                   // change pedal state
          lastStateChange = millis();
                                                      // now actually change LED and switch relay
          digitalWrite(pin_Mute, HIGH);               // mute output of circuit
          delay(10);                                  // wait for mute
          digitalWrite(pin_Activate, pedalState);     // turn effect on/off
          delay(muteDelay);                           // wait for relay to switch
          digitalWrite(pin_Mute, LOW);                // turn mute off
        }
        else if (millis()-lastStateChange>toggleDelay)      // if switch is held longer than one second, switch state when switch is released
        {      
          pedalState = !pedalState;                   // change pedal state
          lastStateChange = millis();             
                                                      // now actually change LED and switch relay
          digitalWrite(pin_Mute, HIGH);               // mute output of circuit
          delay(10);                                  // wait for mute
          digitalWrite(pin_Activate, pedalState);     // turn effect on/off
          delay(muteDelay);                           // wait for relay to switch
          digitalWrite(pin_Mute, LOW);                // turn mute off
        }
    }
  }

  lastswitchstate = reading;                          //save current reading as lastswitchstate for next time through loop
}

