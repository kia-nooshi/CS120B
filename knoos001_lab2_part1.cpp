/*        Your Name & E-mail: Kianoosh Nooshi - knoos001@ucr.edu
 
 *
 
 *          Discussion Section: 22

 *         Assignment: Lab #2  Exercise #1
 
 *         goal: cause the lights on state machine to turn on or off with corresponding button push
     
 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *         Demo Link: <https://youtu.be/iVKCUIxJQiE>

 */


int B[4] = {2, 3, 4, 5};
int PD = 6;
int PU = 7;

int num = 750;
void setup() {

  // put your setup code here, to run once:

  for (int i = 0; i < 4; i++) {
    pinMode(B[i], OUTPUT);
  }
  Serial.begin(9600);
}

void flash() {
    for (int i = 0; i < 4; i++) {
      digitalWrite(B[i], HIGH);
    }
    delay(500);
    for (int i = 0; i < 4; i++) {
      digitalWrite(B[i], LOW);
    }
    delay(500);
    Serial.println(analogRead(A0));
}
void loop() {

  
  pinMode(6, INPUT);
  pinMode(7, INPUT_PULLUP);
  int onButton = 0;
  int offButton = 0;
  
  onButton = digitalRead(6);
  if  (onButton == HIGH){
    for (int i = 0; i < 4; i++){
      digitalWrite(B[i], HIGH);
    }
  }
  offButton = digitalRead(7);
  if (offButton == LOW){
    for (int i = 0; i < 4; i++) {
        digitalWrite(B[i], LOW);
    }
  }
  
}
