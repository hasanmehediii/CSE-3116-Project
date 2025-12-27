//#include <WiFi.h>
//#include <WebServer.h>
//#include <ESP32Servo.h>
//
///* ================= WIFI SETTINGS ================= */
//const char* ssid = "FireFighter_Bot";
//const char* password = "12345678";
//WebServer server(80);
//
///* ================= MOTOR PINS (L298N) ================= */
//// Left Motor (OUT1 & OUT2 on L298N)
//#define ENA 25
//#define IN1 26
//#define IN2 27
//
//// Right Motor (OUT3 & OUT4 on L298N)
//// IMPORTANT: PIN 12 changed to 18 to prevent boot failure!
//#define ENB 14
//#define IN3 18 
//#define IN4 13
//
///* ================= MOTOR DIRECTION CONFIG ================= */
///* If your robot moves backward when it should go forward, 
//   swap HIGH and LOW in these sections. */
//
//// LEFT MOTOR Logic
//#define LEFT_FORWARD_LOGIC_IN1 HIGH
//#define LEFT_FORWARD_LOGIC_IN2 LOW
//
//// RIGHT MOTOR Logic
//// If the right side spins backwards, swap these to LOW / HIGH
//#define RIGHT_FORWARD_LOGIC_IN3 HIGH 
//#define RIGHT_FORWARD_LOGIC_IN4 LOW 
//
///* ================= SERVO ================= */
//#define SERVO_PIN 33
//Servo waterServo;
//
///* ================= FLAME SENSORS ================= */
//// Sensors read LOW (0) when fire is detected
//#define FLAME_LEFT 34
//#define FLAME_CENTER 35
//#define FLAME_RIGHT 32
//
///* ================= RELAY / PUMP ================= */
//#define PUMP_RELAY 23
//
///* ================= GLOBAL VARIABLES ================= */
//bool isAutoMode = true;
//bool spraying = false;
//unsigned long sprayTime = 0;
//
//// Motor Speed (0-255). Lower if robot moves too fast.
//const int motorSpeed = 160; 
//
///* ================= FUNCTION DECLARATIONS ================= */
//void stopMoving();
//void moveForward();
//void moveBackward();
//void turnLeft();
//void turnRight();
//void handleRoot();
//void startSpray();
//
///* ================= SETUP ================= */
//void setup() {
//  Serial.begin(115200);
//
//  // 1. Setup Motor Pins
//  pinMode(IN1, OUTPUT);
//  pinMode(IN2, OUTPUT);
//  pinMode(IN3, OUTPUT);
//  pinMode(IN4, OUTPUT);
//  pinMode(ENA, OUTPUT);
//  pinMode(ENB, OUTPUT);
//
//  // 2. Setup Relay
//  pinMode(PUMP_RELAY, OUTPUT);
//  digitalWrite(PUMP_RELAY, LOW); // Pump OFF
//
//  // 3. Setup Flame Sensors
//  pinMode(FLAME_LEFT, INPUT);
//  pinMode(FLAME_CENTER, INPUT);
//  pinMode(FLAME_RIGHT, INPUT);
//
//  // 4. Initial Stop
//  stopMoving();
//
//  // 5. Setup Servo
//  waterServo.attach(SERVO_PIN);
//  waterServo.write(90); // Center position
//
//  // 6. Setup WiFi
//  WiFi.softAP(ssid, password);
//  Serial.print("AP IP address: ");
//  Serial.println(WiFi.softAPIP());
//
//  // 7. Define Web Routes
//  server.on("/", handleRoot);
//  server.on("/auto", [](){ isAutoMode = true; stopMoving(); server.send(200,"text/plain","AUTO"); });
//  server.on("/manual", [](){ isAutoMode = false; stopMoving(); server.send(200,"text/plain","MANUAL"); });
//  server.on("/forward", [](){ if(!isAutoMode) moveForward(); server.send(200,"text/plain","OK"); });
//  server.on("/left", [](){ if(!isAutoMode) turnLeft(); server.send(200,"text/plain","OK"); });
//  server.on("/right", [](){ if(!isAutoMode) turnRight(); server.send(200,"text/plain","OK"); });
//  server.on("/stop", [](){ stopMoving(); server.send(200,"text/plain","STOP"); });
//  server.on("/pump_on", [](){ digitalWrite(PUMP_RELAY, HIGH); server.send(200,"text/plain","PUMP ON"); });
//  server.on("/pump_off", [](){ digitalWrite(PUMP_RELAY, LOW); server.send(200,"text/plain","PUMP OFF"); });
//
//  server.begin();
//}
//
///* ================= LOOP ================= */
//void loop() {
//  server.handleClient();
//
//  // If MANUAL mode, do nothing automatically
//  if(!isAutoMode) return;
//
//  // Read Sensors (LOW = FIRE DETECTED)
//  int sLeft = digitalRead(FLAME_LEFT);
//  int sCenter = digitalRead(FLAME_CENTER);
//  int sRight = digitalRead(FLAME_RIGHT);
//
//  if(!spraying){
//    // Priority 1: Center Fire -> Move Forward
//    if(sCenter == LOW){
//      Serial.println("Fire Center! Moving Forward.");
//      waterServo.write(90); 
//      moveForward();
//      delay(300); // Small move to get closer
//      stopMoving();
//      startSpray();
//    }
//    // Priority 2: Left Fire -> Turn Left
//    else if(sLeft == LOW){
//      Serial.println("Fire Left! Turning Left.");
//      waterServo.write(140); 
//      turnRight();
//      delay(250); // Small turn
//      stopMoving();
//      startSpray();
//    }
//    // Priority 3: Right Fire -> Turn Right
//    else if(sRight == LOW){
//      Serial.println("Fire Right! Turning Right.");
//      waterServo.write(40);
//      turnLeft();
//      delay(250); // Small turn
//      stopMoving();
//      startSpray();
//    }
//    // No Fire -> Stop
//    else{
//      stopMoving();
//    }
//  }
//
//  // Handle Spray Timer
//  if(spraying && millis() - sprayTime >= 2000){ // Spray for 2 seconds
//    digitalWrite(PUMP_RELAY, LOW);
//    spraying = false;
//    stopMoving();
//    waterServo.write(90); // Reset servo
//  }
//}
//
///* ================= SPRAY CONTROL ================= */
//void startSpray(){
//  spraying = true;
//  sprayTime = millis();
//  digitalWrite(PUMP_RELAY, HIGH);
//}
//
///* ================= MOTOR FUNCTIONS ================= */
//
//void stopMoving(){
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, LOW);
//  digitalWrite(IN3, LOW);
//  digitalWrite(IN4, LOW);
//  analogWrite(ENA, 0);
//  analogWrite(ENB, 0);
//}
//
//void moveForward(){
//  // Left Motor Forward
//  digitalWrite(IN1, LEFT_FORWARD_LOGIC_IN1);
//  digitalWrite(IN2, LEFT_FORWARD_LOGIC_IN2);
//  
//  // Right Motor Forward
//  digitalWrite(IN3, RIGHT_FORWARD_LOGIC_IN3);
//  digitalWrite(IN4, RIGHT_FORWARD_LOGIC_IN4);
//  
//  analogWrite(ENA, motorSpeed);
//  analogWrite(ENB, motorSpeed);
//}
//
//void moveBackward(){
//  // Left Motor Backward (Inverse of Forward)
//  digitalWrite(IN1, !LEFT_FORWARD_LOGIC_IN1);
//  digitalWrite(IN2, !LEFT_FORWARD_LOGIC_IN2);
//  
//  // Right Motor Backward
//  digitalWrite(IN3, !RIGHT_FORWARD_LOGIC_IN3);
//  digitalWrite(IN4, !RIGHT_FORWARD_LOGIC_IN4);
//
//  analogWrite(ENA, motorSpeed);
//  analogWrite(ENB, motorSpeed);
//}
//
//void turnLeft(){
//  // Left Motor Backward
//  digitalWrite(IN1, !LEFT_FORWARD_LOGIC_IN1);
//  digitalWrite(IN2, !LEFT_FORWARD_LOGIC_IN2);
//  
//  // Right Motor Forward
//  digitalWrite(IN3, RIGHT_FORWARD_LOGIC_IN3);
//  digitalWrite(IN4, RIGHT_FORWARD_LOGIC_IN4);
//  
//  analogWrite(ENA, motorSpeed);
//  analogWrite(ENB, motorSpeed);
//}
//
//void turnRight(){
//  // Left Motor Forward
//  digitalWrite(IN1, LEFT_FORWARD_LOGIC_IN1);
//  digitalWrite(IN2, LEFT_FORWARD_LOGIC_IN2);
//  
//  // Right Motor Backward
//  digitalWrite(IN3, !RIGHT_FORWARD_LOGIC_IN3);
//  digitalWrite(IN4, !RIGHT_FORWARD_LOGIC_IN4);
//  
//  analogWrite(ENA, motorSpeed);
//  analogWrite(ENB, motorSpeed);
//}
//
///* ================= WEB PAGE ================= */
//void handleRoot(){
//  server.send(200,"text/html",R"rawliteral(
//<!DOCTYPE html>
//<html>
//<head>
//<meta name="viewport" content="width=device-width, initial-scale=1">
//<style>
//body{background:#222;color:white;text-align:center;font-family:sans-serif;}
//button{width:100px;height:50px;font-size:16px;margin:5px;border-radius:8px;}
//.mode{background:#555;color:white;}
//.ctrl{background:#007BFF;color:white;}
//.pump{background:#FF4500;color:white;}
//</style>
//</head>
//<body>
//<h2>🔥 Fire Fighter Bot 🔥</h2>
//<button class="mode" onclick="fetch('/auto')">AUTO</button>
//<button class="mode" onclick="fetch('/manual')">MANUAL</button><br><br>
//<button class="ctrl" onclick="fetch('/forward')">FWD</button><br>
//<button class="ctrl" onclick="fetch('/left')">LEFT</button>
//<button class="ctrl" onclick="fetch('/stop')">STOP</button>
//<button class="ctrl" onclick="fetch('/right')">RIGHT</button><br><br>
//<button class="pump" onclick="fetch('/pump_on')">PUMP ON</button>
//<button class="pump" onclick="fetch('/pump_off')">PUMP OFF</button>
//</body></html>
//)rawliteral");
//}

//#include <WiFi.h>
//#include <WebServer.h>
//#include <ESP32Servo.h>
//
///* ================= WIFI SETTINGS ================= */
//const char* ssid = "FireFighter_Bot";
//const char* password = "12345678";
//WebServer server(80);
//
///* ================= MOTOR PINS (L298N) ================= */
//// Left Motor (OUT1 & OUT2 on L298N)
//#define ENA 25
//#define IN1 26
//#define IN2 27
//
//// Right Motor (OUT3 & OUT4 on L298N)
//#define ENB 14
//#define IN3 18 
//#define IN4 13
//
///* ================= MOTOR DIRECTION CONFIG ================= */
//// LEFT MOTOR Logic
//#define LEFT_FORWARD_LOGIC_IN1 HIGH
//#define LEFT_FORWARD_LOGIC_IN2 LOW
//
//// RIGHT MOTOR Logic
//#define RIGHT_FORWARD_LOGIC_IN3 HIGH 
//#define RIGHT_FORWARD_LOGIC_IN4 LOW 
//
///* ================= SERVO ================= */
//#define SERVO_PIN 33
//Servo waterServo;
//
//// Servo angle definitions - adjust these based on your mechanical setup
//#define SERVO_CENTER 90
//#define SERVO_LEFT 60    // Changed from 140 - adjust if needed
//#define SERVO_RIGHT 120  // Changed from 40 - adjust if needed
//
///* ================= FLAME SENSORS ================= */
//#define FLAME_LEFT 34
//#define FLAME_CENTER 35
//#define FLAME_RIGHT 32
//
///* ================= RELAY / PUMP ================= */
//#define PUMP_RELAY 23
//
///* ================= GLOBAL VARIABLES ================= */
//bool isAutoMode = true;
//bool spraying = false;
//unsigned long sprayTime = 0;
//int currentServoAngle = SERVO_CENTER;
//
//const int motorSpeed = 160; 
//
///* ================= FUNCTION DECLARATIONS ================= */
//void stopMoving();
//void moveForward();
//void moveBackward();
//void turnLeft();
//void turnRight();
//void handleRoot();
//void startSpray();
//void setServoAngle(int angle);
//
///* ================= SETUP ================= */
//void setup() {
//  Serial.begin(115200);
//  delay(1000); // Give serial time to initialize
//
//  // 1. Setup Motor Pins
//  pinMode(IN1, OUTPUT);
//  pinMode(IN2, OUTPUT);
//  pinMode(IN3, OUTPUT);
//  pinMode(IN4, OUTPUT);
//  pinMode(ENA, OUTPUT);
//  pinMode(ENB, OUTPUT);
//
//  // 2. Setup Relay
//  pinMode(PUMP_RELAY, OUTPUT);
//  digitalWrite(PUMP_RELAY, LOW);
//
//  // 3. Setup Flame Sensors
//  pinMode(FLAME_LEFT, INPUT);
//  pinMode(FLAME_CENTER, INPUT);
//  pinMode(FLAME_RIGHT, INPUT);
//
//  // 4. Initial Stop
//  stopMoving();
//
//  // 5. Setup Servo with improved initialization
//  ESP32PWM::allocateTimer(0);
//  ESP32PWM::allocateTimer(1);
//  ESP32PWM::allocateTimer(2);
//  ESP32PWM::allocateTimer(3);
//  
//  waterServo.setPeriodHertz(50);    // Standard 50hz servo
//  waterServo.attach(SERVO_PIN, 500, 2400); // Min/Max pulse width in microseconds
//  
//  delay(100);
//  setServoAngle(SERVO_CENTER);
//  delay(500); // Give servo time to reach position
//  
//  Serial.println("Servo initialized at center position");
//
//  // 6. Setup WiFi
//  WiFi.softAP(ssid, password);
//  Serial.print("AP IP address: ");
//  Serial.println(WiFi.softAPIP());
//
//  // 7. Define Web Routes
//  server.on("/", handleRoot);
//  server.on("/auto", [](){ 
//    isAutoMode = true; 
//    stopMoving(); 
//    setServoAngle(SERVO_CENTER);
//    server.send(200,"text/plain","AUTO"); 
//  });
//  server.on("/manual", [](){ 
//    isAutoMode = false; 
//    stopMoving(); 
//    server.send(200,"text/plain","MANUAL"); 
//  });
//  server.on("/forward", [](){ 
//    if(!isAutoMode) moveForward(); 
//    server.send(200,"text/plain","OK"); 
//  });
//  server.on("/left", [](){ 
//    if(!isAutoMode) turnLeft(); 
//    server.send(200,"text/plain","OK"); 
//  });
//  server.on("/right", [](){ 
//    if(!isAutoMode) turnRight(); 
//    server.send(200,"text/plain","OK"); 
//  });
//  server.on("/stop", [](){ 
//    stopMoving(); 
//    server.send(200,"text/plain","STOP"); 
//  });
//  server.on("/pump_on", [](){ 
//    digitalWrite(PUMP_RELAY, HIGH); 
//    server.send(200,"text/plain","PUMP ON"); 
//  });
//  server.on("/pump_off", [](){ 
//    digitalWrite(PUMP_RELAY, LOW); 
//    server.send(200,"text/plain","PUMP OFF"); 
//  });
//  
//  // Add servo test endpoints
//  server.on("/servo_left", [](){ 
//    setServoAngle(SERVO_LEFT); 
//    server.send(200,"text/plain","SERVO LEFT"); 
//  });
//  server.on("/servo_center", [](){ 
//    setServoAngle(SERVO_CENTER); 
//    server.send(200,"text/plain","SERVO CENTER"); 
//  });
//  server.on("/servo_right", [](){ 
//    setServoAngle(SERVO_RIGHT); 
//    server.send(200,"text/plain","SERVO RIGHT"); 
//  });
//
//  server.begin();
//  Serial.println("Server started");
//}
//
///* ================= LOOP ================= */
//void loop() {
//  server.handleClient();
//
//  if(!isAutoMode) return;
//
//  // Read Sensors (LOW = FIRE DETECTED)
//  int sLeft = digitalRead(FLAME_LEFT);
//  int sCenter = digitalRead(FLAME_CENTER);
//  int sRight = digitalRead(FLAME_RIGHT);
//
//  if(!spraying){
//    // Priority 1: Center Fire
//    if(sCenter == LOW){
//      Serial.println("Fire Center! Moving Forward.");
//      setServoAngle(SERVO_CENTER);
//      delay(200); // Give servo time to move
//      moveForward();
//      delay(300);
//      stopMoving();
//      startSpray();
//    }
//    // Priority 2: Left Fire
//    else if(sLeft == LOW){
//      Serial.println("Fire Left! Turning Right to face it.");
//      setServoAngle(SERVO_LEFT);
//      delay(200); // Give servo time to move
//      turnRight();
//      delay(250);
//      stopMoving();
//      startSpray();
//    }
//    // Priority 3: Right Fire
//    else if(sRight == LOW){
//      Serial.println("Fire Right! Turning Left to face it.");
//      setServoAngle(SERVO_RIGHT);
//      delay(200); // Give servo time to move
//      turnLeft();
//      delay(250);
//      stopMoving();
//      startSpray();
//    }
//    else{
//      stopMoving();
//    }
//  }
//
//  // Handle Spray Timer
//  if(spraying && millis() - sprayTime >= 2000){
//    digitalWrite(PUMP_RELAY, LOW);
//    spraying = false;
//    stopMoving();
//    setServoAngle(SERVO_CENTER);
//    Serial.println("Spray complete. Returning to center.");
//  }
//}
//
///* ================= SERVO CONTROL ================= */
//void setServoAngle(int angle){
//  if(angle != currentServoAngle){
//    waterServo.write(angle);
//    currentServoAngle = angle;
//    Serial.print("Servo moved to: ");
//    Serial.println(angle);
//  }
//}
//
///* ================= SPRAY CONTROL ================= */
//void startSpray(){
//  spraying = true;
//  sprayTime = millis();
//  digitalWrite(PUMP_RELAY, HIGH);
//  Serial.println("Starting spray!");
//}
//
///* ================= MOTOR FUNCTIONS ================= */
//
//void stopMoving(){
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, LOW);
//  digitalWrite(IN3, LOW);
//  digitalWrite(IN4, LOW);
//  analogWrite(ENA, 0);
//  analogWrite(ENB, 0);
//}
//
//void moveForward(){
//  digitalWrite(IN1, LEFT_FORWARD_LOGIC_IN1);
//  digitalWrite(IN2, LEFT_FORWARD_LOGIC_IN2);
//  digitalWrite(IN3, RIGHT_FORWARD_LOGIC_IN3);
//  digitalWrite(IN4, RIGHT_FORWARD_LOGIC_IN4);
//  analogWrite(ENA, motorSpeed);
//  analogWrite(ENB, motorSpeed);
//}
//
//void moveBackward(){
//  digitalWrite(IN1, !LEFT_FORWARD_LOGIC_IN1);
//  digitalWrite(IN2, !LEFT_FORWARD_LOGIC_IN2);
//  digitalWrite(IN3, !RIGHT_FORWARD_LOGIC_IN3);
//  digitalWrite(IN4, !RIGHT_FORWARD_LOGIC_IN4);
//  analogWrite(ENA, motorSpeed);
//  analogWrite(ENB, motorSpeed);
//}
//
//void turnLeft(){
//  digitalWrite(IN1, !LEFT_FORWARD_LOGIC_IN1);
//  digitalWrite(IN2, !LEFT_FORWARD_LOGIC_IN2);
//  digitalWrite(IN3, RIGHT_FORWARD_LOGIC_IN3);
//  digitalWrite(IN4, RIGHT_FORWARD_LOGIC_IN4);
//  analogWrite(ENA, motorSpeed);
//  analogWrite(ENB, motorSpeed);
//}
//
//void turnRight(){
//  digitalWrite(IN1, LEFT_FORWARD_LOGIC_IN1);
//  digitalWrite(IN2, LEFT_FORWARD_LOGIC_IN2);
//  digitalWrite(IN3, !RIGHT_FORWARD_LOGIC_IN3);
//  digitalWrite(IN4, !RIGHT_FORWARD_LOGIC_IN4);
//  analogWrite(ENA, motorSpeed);
//  analogWrite(ENB, motorSpeed);
//}
//
///* ================= WEB PAGE ================= */
//void handleRoot(){
//  server.send(200,"text/html",R"rawliteral(
//<!DOCTYPE html>
//<html>
//<head>
//<meta name="viewport" content="width=device-width, initial-scale=1">
//<style>
//body{background:#222;color:white;text-align:center;font-family:sans-serif;padding:20px;}
//button{width:100px;height:50px;font-size:16px;margin:5px;border-radius:8px;border:none;cursor:pointer;}
//.mode{background:#555;color:white;}
//.ctrl{background:#007BFF;color:white;}
//.pump{background:#FF4500;color:white;}
//.servo{background:#28a745;color:white;}
//h2{margin-bottom:20px;}
//.section{margin:20px 0;padding:10px;border-top:1px solid #444;}
//</style>
//</head>
//<body>
//<h2>🔥 Fire Fighter Bot 🔥</h2>
//
//<div class="section">
//<h3>Mode Control</h3>
//<button class="mode" onclick="fetch('/auto')">AUTO</button>
//<button class="mode" onclick="fetch('/manual')">MANUAL</button>
//</div>
//
//<div class="section">
//<h3>Movement Control</h3>
//<button class="ctrl" onclick="fetch('/forward')">FWD</button><br>
//<button class="ctrl" onclick="fetch('/left')">LEFT</button>
//<button class="ctrl" onclick="fetch('/stop')">STOP</button>
//<button class="ctrl" onclick="fetch('/right')">RIGHT</button>
//</div>
//
//<div class="section">
//<h3>Pump Control</h3>
//<button class="pump" onclick="fetch('/pump_on')">PUMP ON</button>
//<button class="pump" onclick="fetch('/pump_off')">PUMP OFF</button>
//</div>
//
//<div class="section">
//<h3>Servo Test</h3>
//<button class="servo" onclick="fetch('/servo_left')">◄ LEFT</button>
//<button class="servo" onclick="fetch('/servo_center')">● CENTER</button>
//<button class="servo" onclick="fetch('/servo_right')">RIGHT ►</button>
//</div>
//
//</body></html>
//)rawliteral");
//}


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
