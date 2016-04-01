// author NG JING SIANG (GERALD)
// MATRIC NO.: A0124321Y

#include <LiquidCrystal.h>
#include <MQ135.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
MQ135 gasSensor = MQ135(2);

const int columns = 16;
const int rows = 2;
const int buttonAnalogPort = A0;
const int tempAnalogPort = A1;
const int gasAnalogPort = A2;
const int ledPort = 13;

int previousBtnState = 0;
int currentBtnState = 0;
unsigned long previousTime = 0;
const int refreshRate = 300;
const int btnStates[] = {0, 1, 2};
const int btn0Val = 1000;
const int btn1Val = 950;
const int btn2Val = 490;

float gasThreshold = 400.0;

void setup() {
  pinMode(ledPort, OUTPUT);
  lcd.begin(columns, rows);
  lcd.clear();
  lcd.print("Temperature");
  lcd.setCursor(0, 1);
  lcd.print("Gas");
}

void loop() {
  int btnVal = analogRead(buttonAnalogPort);

  // checked button press
  // doesnt set when no button pressed
  if (btnVal >= btn0Val) {
    currentBtnState = btnStates[0];
  } else if (btnVal >= btn1Val) {
    currentBtnState = btnStates[1];
  } else if (btnVal >= btn2Val) {
    currentBtnState = btnStates[2];
  }

  // // update screen
  unsigned long currentTime = millis();
  if (currentTime - previousTime > refreshRate) {
    // menu
    if (previousBtnState == 0) {
      // reset
      digitalWrite(ledPort, LOW);
      lcd.clear();

      lcd.print("Temperature");
      lcd.setCursor(0, 1);
      lcd.print("Gas");
    } else if (previousBtnState == 1) {
      // reset
      digitalWrite(ledPort, LOW);

      float temp = getTemperature(tempAnalogPort);

      // update temp display value
      lcd.clear();
      lcd.print("Temp: ");
      lcd.print(temp);
      lcd.print(" C");
    } else if (previousBtnState == 2) {

      float gasLevel = gasSensor.getPPM();
      // update gas display value
      lcd.clear();
      lcd.print("CO2: ");
      lcd.print(gasLevel);
      lcd.print(" ppm");

      // control LED
      if (gasSensor.getPPM() >= gasThreshold) {
        digitalWrite(ledPort, HIGH);
      } else {
        digitalWrite(ledPort, LOW);
      }

    }
    // update time
    previousTime = currentTime;
    previousBtnState = currentBtnState;
  }
}

float getTemperature(int pin) {
  // the numbers are for converting the analog value to temperature
  float analogRange = 1024.0;

  int tempAnalogVal = analogRead(tempAnalogPort);
  float tempVolt = (tempAnalogVal / analogRange) * 5;
  float temp = (tempVolt - 0.5) * 100;

  return temp;
}