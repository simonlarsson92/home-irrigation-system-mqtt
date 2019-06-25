//Analog inputs
int light_sensor_vcc = D5; //Find better output pin
int sensor_pin = A0;
int humidity_sensor_vcc = D6;
int humidity_sensor2_vcc = D7;

void setup() {
  // Init the humidity sensor board
  pinMode(humidity_sensor_vcc, OUTPUT);
  digitalWrite(humidity_sensor_vcc, LOW);
  pinMode(humidity_sensor2_vcc, OUTPUT);
  digitalWrite(humidity_sensor2_vcc, LOW);
  
  Serial.begin(9600);
}

int read_humidity_sensor() {
  digitalWrite(humidity_sensor_vcc, HIGH);
  delay(500);
  int value = analogRead(sensor_pin);
  digitalWrite(humidity_sensor_vcc, LOW);
  return 1023 - value;
}

int read_humidity_sensor2() {
  digitalWrite(humidity_sensor2_vcc, HIGH);
  delay(500);
  int value = analogRead(sensor_pin);
  digitalWrite(humidity_sensor2_vcc, LOW);
  return 1023 - value;
}

int read_light_sensor() {
  digitalWrite(light_sensor_vcc, HIGH);
  delay(4000);
  int value = analogRead(sensor_pin);
  digitalWrite(light_sensor_vcc, LOW);
  return value;
}

void loop() {
  Serial.print("Humidity Sensor 1 Level (0-1023): ");
  Serial.println(read_humidity_sensor()); 
  delay(5000);
  Serial.print("Humidity Sensor 2 Level (0-1023): ");
  Serial.println(read_humidity_sensor2()); 
  delay(5000);
  Serial.print("Light Sensor Level (0-1023): ");
  Serial.println(read_light_sensor()); 
  delay(5000);
  
}
