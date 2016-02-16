#include <LiquidCrystal.h>
#include <MQ135.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
MQ135 gasSensor = MQ135(2);

const int columns = 16;
const int rows = 2;
const int buttonAnalogPort = A0;
const int tempAnalogPort = A1;
const int gasAnalogPort = A2;

int screenState = 0;
int previousBtnState = 0;
int currentBtnState = 0;
unsigned long previousTime = 0;
const int refreshRate = 300;
const int btnStates[] = {0, 1, 2};
const int btn0Val = 1020;
const int btn1Val = 990;
const int btn2Val = 500;

void setup() {
  Serial.begin(9600);
  lcd.begin(columns, rows);
  lcd.clear();
  lcd.print("Temperature");
  lcd.setCursor(0, 1);
  lcd.print("Gas");
}

void loop() {
  int btnVal = analogRead(buttonAnalogPort);
  Serial.println(btnVal);

  // checked button press
  // doesnt set when no button pressed
  if (btnVal >= btn0Val) {
    currentBtnState = btnStates[0];
  } else if (btnVal >= btn1Val) {
    currentBtnState = btnStates[1];
  } else if (btnVal >= btn2Val) {
    currentBtnState = btnStates[2];
  }

  // update screen
  unsigned long currentTime = millis();
  if (currentTime - previousTime > refreshRate) {
    Serial.println(currentTime - previousTime);
    previousTime = currentTime;
    previousBtnState = currentBtnState;

    if (previousBtnState == 0) {
      screenState = 0;
      lcd.clear();
      lcd.print("Temperature");
      lcd.setCursor(0, 1);
      lcd.print("Gas");
    } else if (screenState == 0 && previousBtnState == 1) {
      screenState = 1;

      int tempAnalogVal = analogRead(tempAnalogPort);
      float tempVolt = (tempAnalogVal / 1024.0) * 5;
      float temp = (tempVolt - 0.5) * 100;
      Serial.println(temp);

      lcd.clear();
      lcd.print("Temp: ");
      lcd.print(temp);
      lcd.print(" C");
      // do lighting also
    } else if (screenState == 0 && previousBtnState == 2){
      screenState = 2;

      lcd.clear();
      lcd.print("CO2: ");
      lcd.print(gasSensor.getPPM());
      lcd.print(" ppm");
    }
  }
}