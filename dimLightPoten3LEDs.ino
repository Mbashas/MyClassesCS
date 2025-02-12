#define LED_PIN1 4
#define LED_PIN2 5
#define LED_PIN3 6

#define POTENTIOMETER_PIN A0

void setup()
{
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
}

void loop()
{
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  int brightness = potentiometerValue / 4;

  analogWrite(LED_PIN1, brightness);
  analogWrite(LED_PIN2, brightness);
  analogWrite(LED_PIN3, brightness);
}