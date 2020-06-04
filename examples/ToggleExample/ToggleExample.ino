#include <TN_SwitchState.h>

/* If "is_toggle" is setted "true", it behaves as toggle switch. */
TN_Switch toggle_btnA(true);

/* pushbutton connected to this pin. */
const uint8_t SWITCH_PIN = 2;

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH_PIN, INPUT);

  Serial.println(toggle_btnA.is_toggle());
}

void loop() {

  /* It goes HIGH when pushbutton is pressed. */
  bool is_high = digitalRead(SWITCH_PIN);
  // Serial.println(is_high);
  
  /* Give a signal to proceed the state transition. */
  TnSwitchState btnA_state = toggle_btnA.GiveSignal(is_high);

  /* Get switch state. */
  switch(btnA_state){
    case TnSwitchState::kToggleOff:         Serial.print("\nToggle Off"); break;
    case TnSwitchState::kToggleRise:        Serial.print("\nToggle Rise"); break;
    case TnSwitchState::kToggleRising:      /* Serial.print("\nToggle Rising"); */  break;
    case TnSwitchState::kToggleOn:          Serial.print("\nToggle ON"); break;
    case TnSwitchState::kToggleFall:        Serial.print("\nToggle Fall"); break;
    case TnSwitchState::kToggleFalling:     /* Serial.print("\nToggle Falling"); */  break;
    case TnSwitchState::kLowPassFiltering:  /* Serial.print("\nLow Pass Filtering"); */ break;
    default: Serial.print("\nUNKNOWN"); break;
  }

  /* Get enum index value. */
  uint8_t state_index = static_cast<uint8_t>(btnA_state);
  // Serial.println(state_index);

}