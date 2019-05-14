

# TN_SwitchState

[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

This Arduino library includes the following features.
* Detect single press/release, double press/release and long press/release.
* Detect toggle action ON/OFF/Rising/Falling.

## Usage

As for the push button circuit, please refer to the [link (arduino tutorial)](https://www.arduino.cc/en/tutorial/button).

```cpp
#include <TN_SwitchState.h>

TN_Switch btnA;

/* pushbutton connected to this pin. */
const uint8_t SWITCH_PIN = 2;

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT);

  /* If you want to change the time threshold, please set in this way. */
  btnA.set_long_press_time(300);
  btnA.set_double_press_time(150);

  Serial.println(btnA.long_press_time());
  Serial.println(btnA.double_press_time());

}

void loop() {

  /* It goes HIGH when pushbutton is pressed. */
  bool is_high = digitalRead(SWITCH_PIN);
  // Serial.println(is_high);
  
  /* Give a signal to proceed the state transition. */
  TnSwitchState btnA_state = btnA.GiveSignal(is_high);

  /* Get switch state. */
  switch(btnA_state){
    case TnSwitchState::kIdring:        /* Serial.println("Idring"); */ break;
    case TnSwitchState::kSinglePress:   Serial.print("\nSingle Press"); break;
    case TnSwitchState::kSingleHold:    /* Serial.print("\nSingle Hold"); */  break;
    case TnSwitchState::kSingleRelease: Serial.print("\nSingle Release"); break;
    case TnSwitchState::kDoubleIdring:  /* Serial.print("\nDouble Idring");*/ break;
    case TnSwitchState::kDoublePress:   Serial.print("\nDouble Press"); break;
    case TnSwitchState::kDoubleHold:    /* Serial.print("\nDouble Hold"); */  break;
    case TnSwitchState::kDoubleRelease: Serial.print("\nDouble Release"); break;
    case TnSwitchState::kLongPress:     Serial.print("\nLong Press"); break;
    case TnSwitchState::kLongHold:      /* Serial.print("\nLong Hold"); */  break;
    case TnSwitchState::kLongRelease:   Serial.print("\nLong Release"); break;
    case TnSwitchState::kLowPassFiltering:   /* Serial.print("\nLow Pass Filtering"); */ break;
    default: Serial.print("\nUNKNOWN"); break;
  }

  /* Get enum index value. */
  uint8_t state_index = static_cast<uint8_t>(btnA_state);
  // Serial.println(state_index);

}
```
