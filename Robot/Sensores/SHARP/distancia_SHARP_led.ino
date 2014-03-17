/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
Este programa aumenta el tiempo que el led conectado al pin 13 está encendido si la distancia al sensor
es menor de 30 cm y aumenta el tiempo apagado si la distancia es mayor que 30 cm.
Variar el valor de sesorPin con el adecuado para la conexión de nuestro sensor.
En las tarjetas Arduino el pin 13 está conectado a un led.
*/
int sensorPin = 0; //pin analogico 0
int ledPin = 13; 
int varSensor = 0;

void setup(){
 
  pinMode(ledPin,OUTPUT);
}

void loop(){
  varSensor = analogRead(sensorPin);

  if(varSensor > 81.84)// si esta a menos de 30cm, envia una señal superior a 0.4V, si multiplico 0.4*1023/5=81.84
  { 
    
    digitalWrite(ledPin, HIGH); 
    delay(varSensor);       // delay aumenta segun disminuye la distancia, pero a los 2 cm vuelve a reducirse.
  }			// Esto es por el bajón de tensión que se produce en la salida del sensor al disminuir esta distancia.
			//Ver la gráfica ofrecida por el fabricante que relaciona tensión de salida con distancia.  
  
  else
  {
    digitalWrite(ledPin, LOW);   
    delay(varSensor); 
  }
}

