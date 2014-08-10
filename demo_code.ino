#include <Akeru.h>
#include <SoftwareSerial.h>

int counter;
int inPin = 12;

void setup() {
  
  pinMode(inPin, INPUT); 

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("Starting...");

  // Wait 1 second for the modem to warm up
  delay(1000);

  // Init modem
  Akeru.begin();

  counter = 0;
  
  pinMode(13, OUTPUT);//Trying to see if the Arduino has a LED attached to it.
}

float getTemp(int pinValue) {
  // Convert 0 ~ 1024 to 0.0 ~ 5.0v
  float volts = pinValue * (5.0/1024.0);

  // TMP36 sends 10 mV/deg - 500 mV offset
  return (volts - 0.5) * 100;
}

// int is 16 bits, float is 32 bits. All little endian
typedef struct {
  int counter;
  int light;
  float temp;
  int pinValue;
} Payload;


void loop() {

  Payload p;

  p.counter = counter;
  p.light = analogRead(A1);
  p.temp = getTemp(analogRead(A2));

  while (!Akeru.isReady()) {
    Serial.println("Modem not ready");
    delay(1000);
  }
  
  int val = digitalRead(inPin); //Reading from the input pin
  Serial.print("Input pin = ");
  Serial.println(val);
  p.pinValue = val;

  Serial.print("Counter= "); Serial.println(p.counter);
  Serial.print("Light  = "); Serial.println(p.light);
  Serial.print("Temp   = "); Serial.println(p.temp);

  counter++;

  // Send in the mighty cloud!
  Akeru.send(&p, sizeof(p));

  Serial.println("Sent");
  
  digitalWrite(13, HIGH);
  delay(1000);               // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW

  // Wait for 10 minutes.
  // Note that delay(600000) will block the Arduino (bug in delay()?)
  /*
  for (int second = 0; second < 600; second++) {
    delay(1000);
  }
  */
  
  ////  Wait for 1 min.
  for (int second = 0; second < 60; second++) {
    delay(1000);
  }
}
