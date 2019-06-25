/*
 * TODO
 * Bug when powerloss wont start until d3 and d4 are unplugged during boot
 * Add wifi
 * Add mqtt
 */


//Analog inputs
int sensor_pin = A0;

int solenoid_1 = D3;
int water_pump = D4;
int light_sensor_vcc = D5;
int humidity_sensor_vcc = D6;
int humidity_sensor2_vcc = D7;


void setup() {
  // Init the pins
  pinMode(humidity_sensor_vcc, OUTPUT);
  digitalWrite(humidity_sensor_vcc, LOW);
  pinMode(humidity_sensor2_vcc, OUTPUT);
  digitalWrite(humidity_sensor2_vcc, LOW);
  pinMode(light_sensor_vcc, OUTPUT);
  digitalWrite(light_sensor_vcc, LOW);
  pinMode(solenoid_1, OUTPUT);
  digitalWrite(solenoid_1, LOW);
  pinMode(water_pump, OUTPUT);
  digitalWrite(water_pump, LOW);
  
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
  delay(500);
  int value = analogRead(sensor_pin);
  digitalWrite(light_sensor_vcc, LOW);
  return value;
}

void open_solenoid() {
  digitalWrite(solenoid_1, HIGH);
}

void close_solenoid() {
  digitalWrite(solenoid_1, LOW);
}

void start_water_pump() {
  digitalWrite(water_pump, HIGH);
}

void stop_water_pump() {
  digitalWrite(water_pump, LOW);
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
  Serial.println("Open solenoid");
  open_solenoid(); 
  delay(200);
  Serial.println("Starting waterpump");
  start_water_pump(); 
  delay(5000);
  Serial.println("Stoping waterpump");
  stop_water_pump(); 
  delay(200);
  Serial.println("Close solenoid");
  close_solenoid(); 
  delay(5000);
  
}
