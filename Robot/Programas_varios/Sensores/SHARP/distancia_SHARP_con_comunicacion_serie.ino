/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
Este programa envía por comunicación serie el valor del sensor en:
Bits, recordar 1024 bits = 5V
Y en Voltios.
*/
int sensorPin = 0; //pin analogico 0
int ledPin = 13; 
int varSensor = 0;

void setup(){
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);
}

void loop(){

  varSensor = analogRead(sensorPin);
  Serial.print("Sensor indica: ");
  Serial.println(varSensor);
  varSensor = varSensor * 5 / 1024;
  Serial.print("Sensor voltios: ");
  Serial.println(varSensor);
  delay(1000);
          
}

