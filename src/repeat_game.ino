#include <Arduino.h>

#define MIN_SEQ_LEN 3
#define MAX_SEQ_LEN 5

const int BUTTON1 = 2;
const int BUTTON1_CODE = 1;
const int BUTTON2 = 3;
const int BUTTON2_CODE = 2;
const int BUTTON3 = 4;
const int BUTTON3_CODE = 3;
const int RED = 9;
const int GREEN = 10;
const int BLUE = 11;

int sequence[MAX_SEQ_LEN];
int curSeqLen = 3;
int curPos = 0;
int lastCode = 0;

enum GameState {
  GAME_READY,
  GAME_INPUT,
};
GameState curState = GAME_READY;

void setup() {
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(BUTTON3, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  int keyCode = readKey();
  if (keyCode != 0) {
    Serial.println(keyCode);
    switch (curState) {
      case GAME_READY:
        Serial.println("Start");
        generateSequence();
        playSequence();
        curPos = 0;
        curState = GAME_INPUT;
        break;
      case GAME_INPUT:
        checkKey(keyCode);
        break;
    }
  }
}

int readKey() {
  int keyCode = 0;

  if (digitalRead(BUTTON1)) {
    keyCode = 1;
  } else if (digitalRead(BUTTON2)) {
    keyCode = 2;
  } else if (digitalRead(BUTTON3)) {
    keyCode = 3;
  }

  if (lastCode != keyCode) {
    lastCode = keyCode;
    return keyCode;
  }

  return 0;
}

void checkKey(int keyCode) {
  int choice = 0;
  switch (keyCode) {
    case BUTTON1_CODE:
      choice = RED;
      break;
    case BUTTON2_CODE:
      choice = GREEN;
      break;
    case BUTTON3_CODE:
      choice = BLUE;
      break;
  }
  blinkLed(choice);
  if (sequence[curPos] == choice) {
    Serial.println("Right");
    curPos++;
    if (curPos == curSeqLen) {
      Serial.println("Win");
      if (curSeqLen == MAX_SEQ_LEN) {
        for (int i=0; i<9; i++) {
          blinkAll();
        }
        curSeqLen = MIN_SEQ_LEN;
      } else {
        for (int i=0; i<3; i++) {
          blinkAll();
        }
        curSeqLen++;
      }
      curState = GAME_READY;
    }
  } else {
    blinkRed();
    Serial.println("Wrong");
    curState = GAME_READY;
    curSeqLen = MIN_SEQ_LEN;
  }
}

void blinkLed(int ledCode) {
  digitalWrite(ledCode, HIGH);
  delay(500);
  digitalWrite(ledCode, LOW);
  delay(500);
}

void generateSequence() {
  for (int i=0; i<curSeqLen; i++) {
    sequence[i] = random(RED, BLUE+1);
  }
}

void playSequence() {
  Serial.println("New sequence:");
  for (int i=0; i<curSeqLen; i++) {
    Serial.print(sequence[i]);
    Serial.print(" ");
    blinkLed(sequence[i]);
  }
  Serial.println("");
}

void blinkAll() {
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  delay(200);
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  delay(200);
}

void blinkRed() {
  for (int i=0; i<3; i++) {
    digitalWrite(RED, HIGH);
    delay(150);
    digitalWrite(RED, LOW);
    delay(150);
  }
}
