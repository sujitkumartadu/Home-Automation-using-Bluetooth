#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#define DHTPIN 3

DHT dht(DHTPIN,DHT11);
SoftwareSerial BTserial(10, 11); // RX & TX configured using software serial
int r1=2; // Pin for triggering relay 1
int r2=4; // Pin for triggering relay 2
int ad1=1; //address for storing relay 1 status in the EEPROM
int ad2=2; //address for storing relay 2 status in the EEPROM
byte h; //
byte i; // Variables to store relay condition  
float tem; //temperature variable
float hum; //humidity variable 
float ambientlight;
byte pir;
char BluetoothData; // For recieving BT input



void setup()
{
  Serial.begin(9600);
  BTserial.begin(9600);
  dht.begin();
  pinMode (r1,OUTPUT);
  pinMode (r2,OUTPUT);
  pinMode (12,INPUT);
  pinMode (9,INPUT);
  Serial.println("HI");
  BTserial.println("HI");
  Serial.println("Welcome home! please choose a mode");
  BTserial.println("Welcome home! please choose a mode");

  h = EEPROM.read(ad1);
  i = EEPROM.read(ad2);

// Initialising the Relays according to their last status written in the EEPROM.

//---------------------------------------------------------------------------------------------------//
// RELAY - 1
    
     if(h==1)
      {
       digitalWrite(r1,HIGH);
      }
     else if(h==0)
      {
       digitalWrite(r1,LOW);
      }
// RELAY - 2
     if(i==1)
      {
       digitalWrite(r2,HIGH);
      }
     else if(i==0)
      {
       digitalWrite(r2,LOW);
      }
      
//---------------------------------------------------------------------------------------------------//

}

void loop() 
{
  
 printtemperature();
 printhumidity();
 
 if (BTserial.available()>0)
  { 
   BluetoothData=BTserial.read();
   ambientlight = analogRead(9);
   pir = digitalRead(12);

   delay(2000);
// RELAY - 1
      if(BluetoothData == 'B')
      {
                digitalWrite(r1,HIGH);
                EEPROM.write(ad1,1);
      }
      else if(BluetoothData == 'A')
      {
                digitalWrite(r1,LOW);
                EEPROM.write(ad1,0);
      }
// RELAY - 2
      else if(BluetoothData == 'Y')
      {
                digitalWrite(r2,HIGH);
                EEPROM.write(ad2,1);
      }
      else if(BluetoothData == 'X')
      {
                digitalWrite(r2,LOW);
                EEPROM.write(ad2,0);
      }
// SMART MODE(MUST CHANGE)
      else if(BluetoothData == 'V')
      {
                Serial.println("Turned on Smart Mode");
                BTserial.println("Turned on Smart Mode");
                if(dht.readHumidity() >= 27.00)  //Turn on fan1 when temperature is reaches the threshold
                {
                  digitalWrite(r1,HIGH);
                  EEPROM.write(ad1,1);

                }
                else
                {
                  digitalWrite(r1,LOW);
                  EEPROM.write(ad1,0);
                }
                if(pir == HIGH || ambientlight <= 200) //if detected human or low light
                {
                  digitalWrite(r2,HIGH);
                  EEPROM.write(ad2,1);
                }
                else
                {
                  digitalWrite(r2,LOW);
                  EEPROM.write(ad2,0);
                }
      }
      else if(BluetoothData == 'U')
      {   
                Serial.println("Turned off Smart Mode");
                BTserial.println("Turned off Smart Mode");                                   //turning off Smart Mode
                if(h==1)
                {
                  digitalWrite(r1,HIGH);
                }
                else if(h==0)
                {
                  digitalWrite(r1,LOW);
                }
                if(i==1)
                {
                  digitalWrite(r2,HIGH);
                }
                else if(i==0)
                {
                  digitalWrite(r2,LOW);
                }
      }
               
   }
delay(1000);
}

void printtemperature(){

  // Read temperature as Celsius (the default)
  tem = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(tem)) {
    Serial.println(F("Failed to read temperature from DHT sensor!"));
    BTserial.println(F("Failed to read temperature from DHT sensor!"));
  }
  else
  {
    Serial.print(F("Temperature: "));
    Serial.print(tem);
    Serial.print(F("°C "));
    BTserial.println(F("Temperature: "));
    BTserial.print(tem);
    BTserial.print(F("°C "));
  }
    
    
}

void printhumidity(){

  hum = dht.readHumidity();
  // Read humidity (the default)

  // Check if any reads failed and exit early (to try again).
  if (isnan(hum)) {
    Serial.println(F("Failed to read temperature from DHT sensor!"));
    BTserial.println(F("Failed to read temperature from DHT sensor!"));
  }
  else
  {
    Serial.println("  ");
    Serial.print(F("Humidity: "));
    Serial.print(hum);
    Serial.println("  ");
    BTserial.println(F("Humidity: "));
    BTserial.print(hum);
  }
    
    
}
