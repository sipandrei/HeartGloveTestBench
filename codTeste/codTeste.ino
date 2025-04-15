// DE CALCULAT VITEZA MEDIE A MANUSII

#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>

File movementRecording;
LiquidCrystal_I2C lcd(0x27,20,4);

int trigPin = 6;
int echoPin = 5;
long cm, baseCm;

int startButton = 3;
int BDCbutton = 2;

int buttonState = 1;
int debounce = 50;
int lastDebounce = 0;
bool isRecording = false;

const int SDPin = 10;
Sd2Card card;
int startTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  pinMode(startButton, INPUT_PULLUP);
  pinMode(BDCbutton, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);

  if(!SD.begin(SDPin)){
    Serial.println("SD initialization failed");
    lcd.clear();
    lcd.print("SD initialization FAILED!");
    while(1);
  }
  Serial.println("SD initialized");
  lcd.print("SD initialized!");
}

void loop() {
  if(isRecording == false){
    if(movementRecording)
      movementRecording.close();
    lcd.clear();
    lcd.print("Waiting for start   command...");
    
    while(isRecording == false){
      buttonDebounce(digitalRead(BDCbutton));
    }
    baseCm = distanceCalculation(0);
    lcd.clear();
    lcd.print("Initializing test...");
    movementRecording= SD.open(nameMaker(), FILE_WRITE);
    addCsvLine(movementRecording, "Recording Time", "Distance[cm]");
    startTime = millis();
    Serial.println("start time: "+ String(int(startTime)));
    lcd.clear();
    lcd.print("Test running . . .");
  }
  cm = distanceCalculation(baseCm);
  Serial.println(cm);
  addCsvLine(movementRecording,String(millis()-startTime), String(cm));
  buttonDebounce(digitalRead(startButton));
}

long distanceCalculation(long adjustment){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin,HIGH);
  return (duration/2) / 29.1 - adjustment;  
}

void buttonDebounce(int buttonReading){ 
  if(buttonReading != buttonState and lastDebounce == 0)  
    lastDebounce = millis();
  if((millis()-lastDebounce) > debounce){
    if(buttonReading == 1 and buttonState == 0)
      isRecording = !isRecording;
    if(buttonReading != buttonState)
      buttonState = buttonReading;
      lastDebounce = 0;
    }
}

String nameMaker(){
  String filename;
  int testNumber = 0;
  String prefix = "test";
  String suffix = ".csv";
  filename = prefix + testNumber + suffix;
  while(SD.exists(filename)){
    testNumber++;
    filename = prefix + testNumber + suffix;  
  }
  return filename;
}

void addCsvLine(File file, String readTime, String distance){
  //Serial.println(readTime + " " + rawValue);
  file.print(readTime);
  file.print(",");
  file.print(distance);
  }
