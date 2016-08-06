//Sender Code

#define numberOfTorches 8

char str[4];
int chaseSpeed = 0;
int chaseNumber = 0;
unsigned long nextChase = 0;
int allTorchesPin = 11;
byte someByte;
int keepAlive = 0;


void setup() {
  for(int i=2; i<2+numberOfTorches; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(allTorchesPin, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void sendData(int someValue) {
  Serial.write(someValue);
  delay(10);
}

void loop() {
  for(int i=2; i<2+numberOfTorches; i++) {
    if (digitalRead(i) == LOW) {
      sendData(i - 2);
    }
  }
  if (analogRead(0) > 10) {
    chaseSpeed = map(analogRead(0), 0, 1024, 1024, 100);
    //sendData(chaseNumber);
    if (millis() > nextChase) {
      chaseNumber++;
      nextChase = millis() + chaseSpeed;
    }
    if (chaseNumber >= numberOfTorches) {
      chaseNumber = 0;
    }
  }
  if (digitalRead(allTorchesPin) == LOW) {
    for (int i=0; i<numberOfTorches; i++) {
      sendData(i);
    }
  }
}
