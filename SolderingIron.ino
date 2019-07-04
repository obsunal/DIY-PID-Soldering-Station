#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define sleep_pin 2
#define sens_pin A0
#define heater_pin 9
#define up_pin 7
#define down_pin 8
#define menu_pin 12

LiquidCrystal_I2C lcd(0x27, 20, 4);

int8_t pwm_value;

int current_read;
int target_read;
int target = 270;
int idle = 50;

double p_error;
double last_p = 0;
double i_error = 0;
double d_error = 0;

double kp;
double ki;
double kd;

char state = "Booting";

void setup() {
  Serial.begin(9600);
  Serial.write(&state);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0); //column row, dikey yatay
  lcd.print(state);
  lcd.setCursor(0, 1);

  attachInterrupt(sleep_pin, change_mode, CHANGE);

  pinMode(sens_pin, INPUT);
  pinMode(heater_pin, OUTPUT);

  pinMode(menu_pin, INPUT_PULLUP);
  pinMode(up_pin, INPUT_PULLUP);
  pinMode(down_pin, INPUT_PULLUP);

  state = "Ready";
  lcd.setCursor(0, 0);
  lcd.print(state);
  Serial.write(state);
  while (!digitalRead(menu_pin)) {
    delay(20);
  }

  lcd.setCursor(0, 0);
  lcd.print("Target: ");
  lcd.print(convert_to_temp(target_read));
  lcd.setCursor(0, 1);
  lcd.print("Current Temp:");
  lcd.print(convert_to_temp(analogRead(sens_pin)));
}

void loop() {
  Serial.write(current_read);
  current_read = analogRead(sens_pin);
  p_error = target_read - current_read;
  i_error += p_error;
  d_error = p_error - last_p;

  pwm_value = (kp * p_error) + (ki * i_error) + (kd * d_error);

  analogWrite(heater_pin, pwm_value);
  
  last_p = p_error;
}

void change_mode () {
  if (digitalRead(sleep_pin)) {
    target_read = convert_to_adc(idle);
    state = "Sleep";
    Serial.write(state);
  }
  else {
    target_read = convert_to_adc(target);
    state = "Active";
    Serial.write(state);
  }
}
//To Be Implemented
int convert_to_temp (int adc_value) {
  return 0;
}
//To Be Implemented
int convert_to_adc (int temp) {
  return 1;
}
