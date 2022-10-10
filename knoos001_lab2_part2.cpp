/*        Your Name & E-mail: Kianoosh Nooshi - knoos001@ucr.edu
 
 *
 
 *          Discussion Section: 22

 *         Assignment: Lab #2  Exercise #2

 *         I acknowledge all content contained herein, excluding template or example code, is my own original work.

 *

 *         Demo Link: <https://youtu.be/flEQwx-Tdtc>

 */
int B[4] = {2, 3, 4, 5};
int PD = 6;
int PU = 7;
int delayValue = 750;
int num = 750;
bool wasClicked = false;
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

int button(int xButton, int yButton, int delayValue) {
  xButton = digitalRead(6); //left
  yButton = digitalRead(7); //right
   //int delayValue = 750;
  if (xButton == HIGH){
      if (delayValue != 1250){ 
      delayValue = delayValue + 500; 
      }
    } else if (yButton == LOW) {
      if (delayValue != 250){ 
      delayValue = delayValue - 500;
      }
   }
  Serial.println(delayValue);
  return delayValue;
}  
void loop() {

  
  pinMode(6, INPUT);
  pinMode(7, INPUT_PULLUP);
  int xButton = 0;
  int yButton = 0;
  
  xButton = digitalRead(6); //left
  yButton = digitalRead(7); //right

    
  
    for (int i = 5; i > 1; i--) { //right
      /*if (xButton == HIGH){
        delayValue = delayValue + 500; 
      } else if (yButton == LOW){
        delayValue = delayValue - 500;
      } */
 
      delayValue = button(xButton,  yButton, delayValue);
      delay(delayValue);
      digitalWrite(i, HIGH);
      
      
   }
    for (int i = 2; i < 6; i++){  //left
      /*if (xButton == HIGH){
        delayValue = delayValue + 500; 
      } else if (yButton == LOW){
        delayValue = delayValue - 500;
      }*/
      delayValue = button(xButton, yButton, delayValue);
      delay(delayValue);
      digitalWrite(i, LOW);
      
      
      
   }
  
  
  
}
