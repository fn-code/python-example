
static const unsigned long REFRESH_INTERVAL = 5000; // ms
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    Serial.println("Serial is connected!!");
  }
}

void loop() {
  static unsigned long lastRefreshTime = 0;
  if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
  {
    lastRefreshTime += REFRESH_INTERVAL;
    sendData();
  }
}

void sendData()
{
  double firstSensor = 123.45;
  double secondSensor = 10.332123;
//
  byte *bFirst = (byte *) &firstSensor;
  byte *bSecond = (byte *) &secondSensor;
  
  byte payload[sizeof(firstSensor) + sizeof(firstSensor)];

  Serial.write(0x81);
  for (size_t i = 0; i < sizeof(firstSensor); i++) {
    payload[i] = bFirst[i];
    Serial.write(bFirst[i]);
  }

  for (size_t i = 0; i < sizeof(secondSensor); i++) {
    payload[i] = bSecond[i];
    Serial.write(bSecond[i]);
  }
//  Serial.println(0xFF);

//  String reply = String("hola");
// short delay before we send data 

// byte payload[11] = {0x4C, 0x75, 0x64, 0x69, 0x6E, 0x20, 0x4C, 0x75, 0x64, 0x69, 0x6E};
// Serial.write(0x81);
// Serial.write(0x80 | reply.length());


}
