
#include <Adafruit_ST7735.h> 
#include <SPI.h>
#include <Wire.h>
#define EEPROM_I2C_ADDRESS_0 0x50
#define EEPROM_I2C_ADDRESS_1 0x51
#if defined(ARDUINO_FEATHER_ESP32) 
  #define TFT_CS         14
  #define TFT_RST        15
  #define TFT_DC         32
#elif defined(ESP8266)
  #define TFT_CS         4
  #define TFT_RST        16                                            
  #define TFT_DC         5
#else
  #define TFT_CS        10
  #define TFT_RST        9 
  #define TFT_DC         8
#endif

typedef struct task {
    int state;
    unsigned long period;
    unsigned long elapsedTime;
    int (*TickFct)(int);
} task;
int delay_gcd;
const unsigned short tasksNum = 4;
task tasks[tasksNum];
const int b_size = 4;
const int b[b_size] = {8, 9, 10, 11};
int EEPROM_I2C_ADDRESS = 0;
byte x = 0;
bool empty = true;
unsigned int timer = millis() / 1000;
unsigned int counter = millis() / 1000;
const int pumpPin = 4;
const int soilSensor = A0;
//pinMode(pumpPin, OUTPUT);
//pinMode(soilSensor, INPUT);
int currentMoisture;
int desiredMoisture;
int desiredTime;
int address = 0;
int volume = Serial.parseInt();
int maximum = (volume / 28) ; //discharge rate of pump
int water = 0;
int over_ridepump = Serial.parseInt();
int over_ridemem = Serial.parseInt();


//timer = millis() - time;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


enum eeprom_States { eeprom_INIT, eeprom_read, eeprom_write, eeprom_update };
int eeprom_Tick(int eeprom) {
    switch (eeprom) { // State transitions
    case eeprom_INIT:
        if(over_ridemem == 1){
          
          eeprom = eeprom_write;
        } else if (over_ridemem == 2); {

          eeprom = eeprom_read;
        }
        
        break;
    case eeprom_read: 
        if(over_ridemem == 2){
          
          eeprom = eeprom_read;
        }else if (over_ridemem == 1) {
          
          eeprom = eeprom_update;
            
            
          }
        break;
        
    case eeprom_write: 
        if(over_ridemem == 1){
          
          eeprom = eeprom_update;
        }else if (over_ridemem == 2) {
          
            eeprom = eeprom_read;
          }
       
        break;

     case eeprom_update:
         if (over_ridemem == 2) {

          eeprom = eeprom_read;
         } else if (over_ridemem == 1){

          eeprom = eeprom_update;
         }
         break;
         }
    
    switch (eeprom) { // State Action
    case eeprom_INIT:         
        break;
    case eeprom_read:
           x = 0;
          Wire.beginTransmission(2);
          Wire.write(byte(address));
          Wire.endTransmission();

          if(Wire.available()){
          x =  Wire.read();
          x = desiredMoisture;
          }else {
          x =  Wire.read();
          x = desiredTime;
          }

  return x;
         
        break;
    case eeprom_write:
          Wire.beginTransmission(1);
          Wire.write(byte(address));
          Wire.write(byte(desiredMoisture));
           Wire.endTransmission();

           Wire.beginTransmission(2);
          Wire.write(byte(address));
          Wire.write(byte(desiredTime));
           Wire.endTransmission();
        break;

     case eeprom_update:
          Wire.beginTransmission(1);
          Wire.write(byte(address));
          Wire.write(byte(desiredMoisture));
           Wire.endTransmission();

             Wire.beginTransmission(2);
          Wire.write(byte(address));
          Wire.write(byte(desiredTime));
           Wire.endTransmission();
      break;

      
    
    return eeprom;
}
}



enum sensor_States { sensor_INIT, sensor_read };
int sensor_Tick(int sensor) {
    switch (sensor) { // State transitions
    case sensor_INIT:
         sensor = sensor_read;
        break;
    case sensor_read:
        
         break;
    }
    
    switch (sensor) { // State Action
    case sensor_INIT:         
        break;
    case sensor_read:
        currentMoisture = analogRead(soilSensor);
        break;
    }
   }

enum lcd_States { lcd_INIT, lcd_title, lcd_display };
int lcd_Tick(int lcd) {
    switch (lcd) { // State transitions
    case lcd_INIT:
        lcd = lcd_title;
        break;
    case lcd_title: 
        if(timer > 5) {
          
          lcd = lcd_display;
        } else{

          lcd = lcd_title;
        }
        break;
    case lcd_display: 
        lcd = lcd_display;
        break;
    }
    switch (lcd) { // State Action
    case lcd_INIT:
      tft.fillScreen(ST77XX_BLACK);         
        break;
    case lcd_title:
      tft.fillScreen(ST77XX_BLACK);
      tft.setCursor(0, 30);
      tft.setTextSize(2);
      tft.println("Hello :)");
      tft.setTextSize(1);
      tft.println("Welcome to your Auto Plant Watering System");
        break;
    case lcd_display:
      tft.fillScreen(ST77XX_BLACK);
      tft.setTextSize(1);
      tft.setTextColor(ST77XX_WHITE);
      tft.print("Current Moisture:");
      tft.println(currentMoisture = analogRead(soilSensor));
      tft.println(" ");
      tft.println("Desired Moisture: ");
      tft.print(desiredMoisture);
      tft.println(" ");
      tft.println("Desired Time: ");
      tft.print(desiredTime);
      tft.println(" seconds");
      tft.println(" ");
      tft.println("Override?: Y or N");
      tft.println(" ");
      tft.println("1 = yes & 2 = no");
      tft.println(" ");
        break;
    }
    return lcd;
}

enum pump_States { pump_INIT, pump_off, pump_on, pump_empty, pump_override };
int pump_Tick(int pump) {
    switch (pump) { // State transitions
    case pump_INIT:
          
          pump = pump_off;
        
        break;
        
    case pump_off:
        if(currentMoisture < desiredMoisture) {
          
          pump = pump_on;
        }else if (currentMoisture > desiredMoisture){
          
          pump = pump_off;
        }else if (currentMoisture < desiredMoisture){
          
          pump = pump_on;
        }else if (over_ridepump == 1) {
          
            pump = pump_override;
        }
        
        break;
        
    case pump_on:
        if(counter <= desiredTime || !empty) {
          
          pump = pump_on;
        }else if(counter >= desiredTime){
          
          pump = pump_off;
          counter = 0;
      
        } else if(empty){

          pump = pump_empty;
          counter = 0;
        }
        break;
    case pump_empty: 

     if (counter > 5){
        pump = pump_off;
        counter = 0;
    }
        break;
    case pump_override: 

        if (over_ridepump == 1){
           
           pump = pump_off;
          
        }else if(empty){
          
           pump = pump_empty;
           
        }else if(!empty){

          pump = pump_override;
          
        }
        break;
    }
    switch (pump) { // State Action
    case pump_INIT:

        break;
    case pump_off:
      digitalWrite(pumpPin, LOW);
        break;
    case pump_on:
      
      water = maximum - counter;
       if (volume == 0){

        empty = true;
       }
      digitalWrite(pumpPin, HIGH);
      counter++;
        break;
    case pump_empty:
        digitalWrite(pumpPin, LOW);
        counter++;
        empty = true;
        
        break;
    case pump_override:

       water = maximum - counter;
       if (volume == 0){

        empty = true;
       }
        digitalWrite(pumpPin, HIGH);
        
        break;
    }
    return pump;
}


void setup() {
    Serial.begin(9600);
    unsigned char i = 0;
    tasks[i].state = eeprom_INIT;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &eeprom_Tick;
    i++;
    tasks[i].state = sensor_INIT;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &sensor_Tick;
    i++;
    tasks[i].state = lcd_INIT;
    tasks[i].period = 1000;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = lcd_Tick;
    i++;
    tasks[i].state = pump_INIT;
    tasks[i].period = 1000;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = pump_Tick;
     
     for (i = 0; i < b_size; i++)
  {
      pinMode(b[i], OUTPUT);
  }
  
}

void loop() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i)
  {
      if ((millis() - tasks[i].elapsedTime) >= tasks[i].period)
      {
          tasks[i].state = tasks[i].TickFct(tasks[i].state);
          tasks[i].elapsedTime = millis();
      }
  }
  
}
