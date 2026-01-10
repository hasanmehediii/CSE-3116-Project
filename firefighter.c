#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

/* ================= WIFI SETTINGS ================= */
const char* ssid = "FireFighter_Bot";
const char* password = "12345678";
WebServer server(80);

/* ================= MOTOR PINS (L298N) ================= */
#define ENA 25
#define IN1 26
#define IN2 27
#define ENB 14
#define IN3 18  
#define IN4 13

/* ================= SERVO ================= */
#define SERVO_PIN 33
Servo waterServo;

/* ================= FLAME SENSORS ================= */
#define FLAME_LEFT 34
#define FLAME_CENTER 35
#define FLAME_RIGHT 32

/* ================= RELAY / PUMP ================= */
#define PUMP_RELAY 23

/* ================= MOTOR DIRECTION CONFIG ================= */
#define LEFT_FORWARD_LOGIC_IN1 HIGH
#define LEFT_FORWARD_LOGIC_IN2 LOW
#define RIGHT_FORWARD_LOGIC_IN3 HIGH 
#define RIGHT_FORWARD_LOGIC_IN4 LOW 

/* ================= GLOBAL VARIABLES ================= */
bool isAutoMode = true;
const int motorSpeed = 165;
const int turnSpeed = 150;

/* ================= FUNCTION DECLARATIONS ================= */
void stopMoving();
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void handleRoot();
void executeSprayCycle();

/* ================= SETUP ================= */
void setup() {
  Serial.begin(115200);
  
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  
  pinMode(PUMP_RELAY, OUTPUT);
  digitalWrite(PUMP_RELAY, LOW); 

  pinMode(FLAME_LEFT, INPUT);
  pinMode(FLAME_CENTER, INPUT);
  pinMode(FLAME_RIGHT, INPUT);

  waterServo.attach(SERVO_PIN);
  waterServo.write(90); 

  stopMoving();

  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.on("/auto", [](){ isAutoMode = true; stopMoving(); server.send(200); });
  server.on("/manual", [](){ isAutoMode = false; stopMoving(); server.send(200); });
  server.on("/fwd", [](){ if(!isAutoMode) moveForward(); server.send(200); });
  server.on("/back", [](){ if(!isAutoMode) moveBackward(); server.send(200); });
  server.on("/left", [](){ if(!isAutoMode) turnLeft(); server.send(200); });
  server.on("/right", [](){ if(!isAutoMode) turnRight(); server.send(200); });
  server.on("/stop", [](){ stopMoving(); server.send(200); });
  server.on("/p_on", [](){ digitalWrite(PUMP_RELAY, HIGH); server.send(200); });
  server.on("/p_off", [](){ digitalWrite(PUMP_RELAY, LOW); server.send(200); });
  
  server.begin();
  Serial.println("System Ready");
}

/* ================= LOOP ================= */
void loop() {
  server.handleClient();
  if(!isAutoMode) return;

  int sLeft = digitalRead(FLAME_LEFT);
  int sCenter = digitalRead(FLAME_CENTER);
  int sRight = digitalRead(FLAME_RIGHT);

  if(sCenter == LOW) {
    // 1. Fire is centered: Move closer for 0.6 seconds
    Serial.println("Target Locked: Moving Closer...");
    moveForward();
    delay(600); 
    stopMoving();
    delay(200);
    
    // 2. Double check if fire is still there before spraying
    if(digitalRead(FLAME_CENTER) == LOW) {
      executeSprayCycle();
    }
  } 
  else if(sLeft == LOW) {
    // Rotate left until center sensor catches fire
    while(digitalRead(FLAME_CENTER) == HIGH && digitalRead(FLAME_LEFT) == LOW) {
      turnRight();
      delay(30); 
    }
    stopMoving();
  }
  else if(sRight == LOW) {
    // Rotate right until center sensor catches fire
    while(digitalRead(FLAME_CENTER) == HIGH && digitalRead(FLAME_RIGHT) == LOW) {
      turnLeft();
      delay(30);
    }
    stopMoving();
  } 
  else {
    stopMoving();
  }
}

/* ================= SPRAY LOGIC ================= */
void executeSprayCycle() {
  digitalWrite(PUMP_RELAY, HIGH);
  // Sweep servo from 60 to 120 and back
  for(int pos = 70; pos <= 110; pos += 2) {
    waterServo.write(pos);
    delay(25);
  }
  for(int pos = 110; pos >= 70; pos -= 2) {
    waterServo.write(pos);
    delay(25);
  }
  waterServo.write(90);
  digitalWrite(PUMP_RELAY, LOW);
  delay(500); 
}

/* ================= MOTOR CONTROL ================= */
void stopMoving(){
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}
void moveForward(){
  digitalWrite(IN1, LEFT_FORWARD_LOGIC_IN1); digitalWrite(IN2, LEFT_FORWARD_LOGIC_IN2);
  digitalWrite(IN3, RIGHT_FORWARD_LOGIC_IN3); digitalWrite(IN4, RIGHT_FORWARD_LOGIC_IN4);
  analogWrite(ENA, motorSpeed); analogWrite(ENB, motorSpeed);
}
void moveBackward(){
  digitalWrite(IN1, !LEFT_FORWARD_LOGIC_IN1); digitalWrite(IN2, !LEFT_FORWARD_LOGIC_IN2);
  digitalWrite(IN3, !RIGHT_FORWARD_LOGIC_IN3); digitalWrite(IN4, !RIGHT_FORWARD_LOGIC_IN4);
  analogWrite(ENA, motorSpeed); analogWrite(ENB, motorSpeed);
}
void turnLeft(){
  digitalWrite(IN1, !LEFT_FORWARD_LOGIC_IN1); digitalWrite(IN2, !LEFT_FORWARD_LOGIC_IN2);
  digitalWrite(IN3, RIGHT_FORWARD_LOGIC_IN3); digitalWrite(IN4, RIGHT_FORWARD_LOGIC_IN4);
  analogWrite(ENA, turnSpeed); analogWrite(ENB, turnSpeed);
}
void turnRight(){
  digitalWrite(IN1, LEFT_FORWARD_LOGIC_IN1); digitalWrite(IN2, LEFT_FORWARD_LOGIC_IN2);
  digitalWrite(IN3, !RIGHT_FORWARD_LOGIC_IN3); digitalWrite(IN4, !RIGHT_FORWARD_LOGIC_IN4);
  analogWrite(ENA, turnSpeed); analogWrite(ENB, turnSpeed);
}

/* ================= HTML / CSS INTERFACE ================= */
void handleRoot(){
  String html = R"rawliteral(
<!DOCTYPE html><html><head><title>FireBot Pro</title>
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<style>
  body { background: #121212; color: white; font-family: sans-serif; text-align: center; margin: 0; padding: 10px; }
  .grid { display: grid; grid-template-columns: 1fr 1fr 1fr; gap: 10px; max-width: 300px; margin: 20px auto; }
  button { padding: 20px; border-radius: 12px; border: none; font-weight: bold; font-size: 16px; cursor: pointer; transition: 0.2s; }
  .btn-move { background: #333; color: white; }
  .btn-move:active { background: #555; transform: scale(0.9); }
  .btn-mode { background: #ff9800; color: black; grid-column: span 1; }
  .btn-pump { background: #2196f3; color: white; grid-column: span 3; padding: 25px; }
  .active-mode { background: #4caf50 !important; color: white; }
  h2 { color: #f44336; margin-bottom: 5px; }
  .status { font-size: 12px; color: #888; margin-bottom: 20px; }
</style></head>
<body>
  <h2>FIREBOT CONTROL</h2>
  <div class="status">Connected to ESP32 Fire Fighter</div>
  
  <div class="grid">
    <button id="auto" class="btn-mode active-mode" onclick="setMode('auto')">AUTO</button>
    <div></div>
    <button id="manual" class="btn-mode" onclick="setMode('manual')">MANUAL</button>
  </div>

  <div class="grid">
    <div></div><button class="btn-move" onmousedown="call('/fwd')" onmouseup="call('/stop')">▲</button><div></div>
    <button class="btn-move" onmousedown="call('/left')" onmouseup="call('/stop')">◄</button>
    <button class="btn-move" style="background:#f44336" onclick="call('/stop')">■</button>
    <button class="btn-move" onmousedown="call('/right')" onmouseup="call('/stop')">►</button>
    <div></div><button class="btn-move" onmousedown="call('/back')" onmouseup="call('/stop')">▼</button><div></div>
  </div>

  <div class="grid">
    <button class="btn-pump" onmousedown="call('/p_on')" onmouseup="call('/p_off')">HOLD FOR WATER 💧</button>
  </div>

<script>
  function call(path) { fetch(path); }
  function setMode(m) {
    call('/' + m);
    document.getElementById('auto').classList.toggle('active-mode', m=='auto');
    document.getElementById('manual').classList.toggle('active-mode', m=='manual');
  }
</script>
</body></html>)rawliteral";
  server.send(200, "text/html", html);
}
