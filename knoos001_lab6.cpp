  int Pin3_Blue = 3;
  int Pin5_White = 5;
  int Pin6_Green = 6;
  int Pin2_Yellow = 2;
  int Pin4_Red = 4;
  int left = 680;
  int right = 680;
  int BlueLeds = LOW;
  int WhiteLeds = LOW;
  int GreenLeds = LOW;
  bool lightsOn = false;
  bool blueFlag = false;
  bool whiteFlag = false;
  bool greenFlag = false;
  int counterBlue = 0;
  int counterWhite = 0;
  int counterGreen = 0;
  int waitCounter = 0;
  unsigned long millisPassed = 0;
  unsigned long millisPassedsecondval = 0;
  unsigned long millisPassedthirdval = 0;
  

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;

int delay_gcd;
const unsigned short tasksNum = 4;
task tasks[tasksNum];

//state machine red&yellow LEDs
enum SMRedYellow_states { SMRedYellow_INIT, on, off};
int SMRedYellow_Tick(int state1) {
  switch (state1) { // State transitions
    case SMRedYellow_INIT:
      state1 = off;
      break;
    case on:
      left = analogRead(A0);
      right = analogRead(A1);
     if(left > 6 || right > 6){
      state1 = on;
     }else if (left <= 6 && right <= 6){
       state1 = off;
     }
      break;
    case off:
      left = analogRead(A0);
      right = analogRead(A1);
     if(left <= 6 && right <= 6){
      state1 = off;
     }else if (left > 6 || right > 6){
       state1 = on;
     }
    break;
  }
  switch (state1) { // State Action
    case SMRedYellow_INIT:
      break;
    case on:
    if (left <= 6 && right >= 6){
      digitalWrite(Pin4_Red, HIGH);
    } //else 
      
      if (right <= 6 && left >=6){
      digitalWrite(Pin2_Yellow, HIGH);
      
    } else if (right >=6 && left >= 6) {
    digitalWrite(Pin2_Yellow,LOW);
      digitalWrite(Pin2_Yellow,LOW);
    }
      break;
    
    case off:
    //Serial.println("state off");
      digitalWrite(Pin4_Red,LOW);
      digitalWrite(Pin2_Yellow,LOW);
    break;
  }
  return state1;
}

//SM blue LED
enum SMBlue_States { SMBlue_INIT, on3, off3};
int SMBlue_Tick(int state3) {
  switch (state3) { // State transitions
    case SMBlue_INIT:
      left = analogRead(A0);
      right = analogRead(A1);
      if (left <= 6 && right <= 6){
              digitalWrite(Pin3_Blue, LOW);
        
      }
      else if ((left >= 10 && right >= 10) && (counterGreen > 7)) {
        state3 = on3;
      }
      else {
        state3 = SMBlue_INIT;
      }
    
      break;
    case on3:
      if (left >= 10 && right >= 10 && counterGreen < 14) {
        state3 = on3;
      }
      else if (left <= 10 && right <= 10) {
        state3 = off3;
      }
      else {
        state3 = off3;
      }
      break;
    case off3:
      if (waitCounter < 19) {
        state3 = off3;
      }
      else {
        state3 = SMBlue_INIT;
      }
      break;

  }
  
  switch (state3) { // State Action
    case SMBlue_INIT:
      counterGreen++;
      break;
    case on3:
      counterGreen++;
      Serial.println("case blue on");
      Serial.println(counterGreen);
      if (millis() - millisPassed >= 300) {
        millisPassed = millis();

        if (BlueLeds == LOW) {
          BlueLeds = HIGH;
        } else {
          BlueLeds = LOW;
        }

        digitalWrite(Pin3_Blue, BlueLeds);
        

        
      }      
      break;
    case off3:
      digitalWrite(Pin3_Blue, LOW);
      Serial.println("!!!!!!blue off!!!!!!");
      blueFlag = false;
      counterGreen = 0;
      waitCounter++;
      break;
  }

  return state3;
}


//White
enum SMWhite_States { SMWhite_INIT, on4, off4};
int SMWhite_Tick(int state4) {
  switch (state4) { // State transitions
    case SMWhite_INIT:
      left = analogRead(A0);
      right = analogRead(A1);
      if ((left >= 10 && right >= 10) && (counterBlue > 7)) {
        state4 = on4;
      }
      else {
        state4 = SMWhite_INIT;
      }
    
      break;
    case on4:
      if (left >= 10 && right >= 10 && counterBlue < 20) {
        state4 = on4;
      }
      else if (left <= 10 && right <= 10) {
        state4 = off4;
      }
      else {
        state4 = off4;
      }
      break;
    case off4:
      Serial.println("white off");
      Serial.println(counterBlue);
      state4 = SMWhite_INIT;
      break;

  }

  switch (state4) { // State Action
    case SMWhite_INIT:
      counterBlue++;
      break;
    case on4:
      counterBlue++;
      if (millis() - millisPassedsecondval >= 500) {
        millisPassedsecondval = millis();

        if (WhiteLeds == LOW) {
          WhiteLeds = HIGH;
        } else {
          WhiteLeds = LOW;
        }

        digitalWrite(Pin5_White, WhiteLeds);
      }      
      break;
    case off4:
      counterBlue = 0;
      digitalWrite(Pin5_White, LOW);
      break;
  }

  return state4;
}

enum SMGreen_States { SMGreen_INIT, on5, off5};
int SMGreen_Tick(int state5) {
  left = analogRead(A0);
  right = analogRead(A1);
  switch (state5) { // State transitions
    case SMGreen_INIT:
      if ((left >= 10 && right >= 10) && (counterWhite > 100)) {
        state5 = on5;
      }
      else {
        state5 = SMGreen_INIT;
      }
    
      break;
    case on5:
      if (left >= 10 && right >= 10 && counterWhite < 120) {
        state5 = on5;
      }
      else if (left <= 10 && right <= 10) {
        state5 = off5;
      }
      else {
        state5 = off5;
      }
      break;
    case off5:
      state5 = SMGreen_INIT;
      break;

  }
  
  switch (state5) { // State Action
    case SMGreen_INIT:
      counterWhite++;
      break;
    case on5:
      counterWhite++;
      if (millis() - millisPassedthirdval >= 100) {
        millisPassedthirdval = millis();

        if (GreenLeds == LOW) {
          GreenLeds = HIGH;
        } else {
          GreenLeds = LOW;
        }

        digitalWrite(Pin6_Green, GreenLeds);
        
      }      
      break;
    case off4:
      counterWhite = 0;
      digitalWrite(Pin6_Green, LOW);
      break;
  }

  return state5;
}





void setup() {
  // put your setup code here, to run once:
  unsigned char i = 0;
  tasks[i].state = SMRedYellow_INIT;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SMRedYellow_Tick;
  i++;
  
  tasks[i].state = SMBlue_INIT;
  tasks[i].period = 300;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SMBlue_Tick;
  i++;
  
 
  tasks[i].state = SMWhite_INIT;
  tasks[i].period = 500;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SMWhite_Tick;
  i++;
  
  tasks[i].state = SMGreen_INIT;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &SMGreen_Tick;
  i++;

  
  
 

  delay_gcd = 1000; // GCD
  Serial.begin(9600);
}

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
}
