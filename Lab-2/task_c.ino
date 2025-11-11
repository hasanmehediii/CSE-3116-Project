#include <Arduino.h>

 
const int TIMER_LED = 26;      
const int BUTTON_PIN = 4;     
const int BUZZER_LED = 27;    

 
volatile bool timerLEDState = LOW;
volatile bool buzzerLEDState = LOW;

 
hw_timer_t *timer = NULL;

 
void IRAM_ATTR onTimer() {
  timerLEDState = !timerLEDState;
  digitalWrite(TIMER_LED, timerLEDState);
}

 
void IRAM_ATTR onButtonPress() {
  buzzerLEDState = !buzzerLEDState;
  digitalWrite(BUZZER_LED, buzzerLEDState);
}

 
void setup() {
  Serial.begin(115200);
 
  pinMode(TIMER_LED, OUTPUT);
  pinMode(BUZZER_LED, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);  

  
  timer = timerBegin(1000000);          // 1 MHz timer tick = 1 µs
  timerAttachInterrupt(timer, &onTimer);
  timerAlarm(timer, 1000000, true, 0);  // 1,000,000 µs = 1 sec interval, auto reload
  timerStart(timer);
 
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPress, FALLING);

}

 
void loop() {
  Serial.println("Main loop running...");
  delay(1000);
}