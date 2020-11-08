//
// Temperature sensor : TMP36
// LCD display : HD44780
//
// Displayed temperature is an average from twenty values rolling window, 
// based on one measure every 2 seconds.
//

#include <Streaming.h>
#include <LiquidCrystal.h>

#define TAB_SIZE 20

const int sensorPin = A0;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
float temp_tab[TAB_SIZE];
int a = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin( 9600 );
  Serial << "Welcom !\n";

  lcd.begin(16, 2);       // set up the LCD's number of columns and rows
  lcd << "Temperature :"; // Print a message to the LCD.

  initTemp();
}

void initTemp() {
  float current_temp = getTempFromSensor();

  for( int i = 0; i < TAB_SIZE; i++) {
    temp_tab[i] = current_temp;
  }
}

void loop() {
  delay(2000);             // wait for 2 seconds
  printTemp();
}

void printTemp( void ) {
  updateTemp();

  lcd.setCursor(0, 1);
  lcd << getTempAverage();
}

void updateTemp( void ) {
  temp_tab[a] = getTempFromSensor();
  a = (++a%TAB_SIZE); // to avoid a value issue
}

// temp = ((analogValue_in_mV * 5.0V / 1024.0) - 0.5V) * 100
float getTempFromSensor( void ) {
  int sensorVal = analogRead( sensorPin );
  float volt = (sensorVal/1024.0) * 5.0;
  float temp = ((volt - 0.5) * 100);

  Serial << "Valeur capteur : " << sensorVal << " - ";
  Serial << "Voltage : " << volt << " - ";
  Serial << "Température : " << temp << "\n";

  return temp;
}

float getTempAverage( void ) {
  float rez = 0;
  
  for(int i = 0; i < TAB_SIZE; i++) {
    rez += temp_tab[i];
  }

  return (rez/TAB_SIZE);
}
