//Sender Code

#define numberOfTorches 8
#define numberOfButtons 19

char str[4];
int chaseSpeed = 0;
int chaseNumber = 0;
unsigned long nextChase = 0;
int allTorchesPin = 21;
byte someByte;
int keepAlive = 0;


void setup() {
  //i = 2 because of RX and TX pins.
  //set all of the button pins to INPUT_PULLUP
  //buttons are connected to the arduino sequentially to make this easier.
  for(int i=2; i<2+numberOfButtons; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  
  pinMode(allTorchesPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void sendData(int someValue) {
  Serial.write(someValue);
  delay(10);
}

void loop() {
  for(int i=2; i<2+numberOfButtons; i++) {
    if (digitalRead(i) == LOW) {
      sendData(i - 2);
    }
  }
  if (analogRead(1) < 1013) {
    chaseSpeed = map(analogRead(1), 1024, 0, 800, 100);
    if (millis() > nextChase) {
      if (chaseSpeed == 100) {
        chaseNumber = random(0,8);
      }
      else {
        chaseNumber++;
      }
      nextChase = millis() + chaseSpeed;
      sendData(chaseNumber);
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
