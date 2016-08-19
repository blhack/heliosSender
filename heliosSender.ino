//Sender Code

#define numberOfTorches 8
#define numberOfButtons 12

//hack for using analog pins as outputs
//thanks to this answer on SA
//http://arduino.stackexchange.com/questions/1615/how-to-loop-over-analog-pins
static const uint8_t analogPins[10] = {A6,A7,A8,A9,A10,A11,A12,A13,A14,A15};


char str[4];
int chaseSpeed = 0;
int chaseNumber = 0;
unsigned long nextChase = 0;
int allTorchesPin = 21;
byte someByte;
int keepAlive = 0;
int lightingSelect = 0;
int lightingProgram = 0;

int runLightingPin = 18;
int pauseLightingPin = 14;
int stopLightingPin = 15;
int functionPinA = 16;
int functionPinB = 17;


void setup() {
  //i = 2 because of RX and TX pins.
  //set all of the button pins to INPUT_PULLUP
  //buttons are connected to the arduino sequentially to make this easier.
  for(int i=2; i<2+numberOfButtons; i++) {
    pinMode(i, INPUT_PULLUP);
  }

  for (int i=0; i<10; i++) {
    pinMode(analogPins[i], OUTPUT);
  }
  
  pinMode(allTorchesPin, INPUT_PULLUP);
  pinMode(runLightingPin, INPUT_PULLUP);
  pinMode(stopLightingPin, INPUT_PULLUP);
  pinMode(pauseLightingPin, INPUT_PULLUP);
  pinMode(functionPinA, INPUT_PULLUP);
  pinMode(functionPinB, INPUT_PULLUP);
  Serial.begin(9600);
  startUp();
}

void startUp() {
  for (int i=0; i<5; i++) {
    digitalWrite(analogPins[i], HIGH);
    digitalWrite(analogPins[i+5], HIGH);
    delay(100);
  }
  delay(500);
  for (int i=0; i<5; i++) {
    digitalWrite(analogPins[i], LOW);
    digitalWrite(analogPins[i+5], LOW);
    delay(100);
  }
}

void sendData(int someValue) {
  Serial.write(someValue);
  //Serial.println(someValue);
  delay(10);
}

void immediateTorch() {
  for(int i=2; i<2+numberOfButtons; i++) {
    if (digitalRead(i) == LOW) {
      sendData(i - 2);
    }
  }
}

void chase() {
  if (analogRead(1) < 1013) {
    chaseSpeed = map(analogRead(1), 1024, 0, 500, 80);
    if (millis() > nextChase) {
      if (chaseSpeed == 80) {
        chaseNumber = random(0,8);
      }
      else {
        chaseNumber++;
      }
      nextChase = millis() + chaseSpeed;
    }
    if (chaseNumber >= numberOfTorches) {
      chaseNumber = 0;
    }
    sendData(chaseNumber);
  }
}

void allTorches() {
  if (digitalRead(allTorchesPin) == LOW) {
    for (int i=0; i<numberOfTorches; i++) {
      sendData(i);
    }
  }
}

void lighting() {
  lightingSelect = analogRead(0);
  lightingProgram = map(lightingSelect, 1024,0,0,9);
  for (int i=0; i<10; i++) {
    if (i == lightingProgram) {
      digitalWrite(analogPins[i], HIGH);
    } else {
      digitalWrite(analogPins[i], LOW);
    }
  }
  if (digitalRead(runLightingPin) == LOW) {
    sendData(100+lightingProgram);
  }
  if (digitalRead(pauseLightingPin) == LOW) {
    sendData(110);
  }
  if (digitalRead(stopLightingPin) == LOW) {
    sendData(111);
  }
  if (digitalRead(functionPinA) == LOW) {
    sendData(112);
  }
  if (digitalRead(functionPinB) == LOW) {
    sendData(113);
  }
}

void loop() {
  immediateTorch();
  chase();
  allTorches();
  lighting();
}
