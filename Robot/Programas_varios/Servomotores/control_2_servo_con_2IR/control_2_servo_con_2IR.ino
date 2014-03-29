/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
*/
#include <Servo.h> 

Servo myservo; 
Servo myservo2;
int pos = 0;    // variable to store the servo position 

int sensorPin = 0; //pin analogico 0
int varSensor = 0;
int sensorPin2 = 2; //pin analogico 2
int varSensor2 = 0;

void setup(){
  Serial.begin(9600);
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
    myservo2.attach(5); 
}

void loop(){// no hay tiempos de espera para el servo...
  varSensor = analogRead(sensorPin);
  varSensor2 = analogRead(sensorPin2);
  Serial.print("Sensor = ");
  Serial.print(varSensor);
    Serial.print(" Sensor2 = ");
  Serial.print(varSensor2);
  
  
  if(varSensor < 100 && varSensor2 < 100){ //visualizando con el monitor serie se observa que el sensor detecta distancias superiores a 30cm, por ejemplo 40*5/1023=0.19V que equivale a x cm
    Serial.println(" = Adelante "); // un buen valor es 40
      myservo.attach(3); 
      myservo2.attach(5);   
     pos = 85 ;           // movimiento "adelante" rapido
      myservo.write(pos);
      pos= 105;
      myservo2.write(pos); 
       delay (1000);
     
  }
  else{
    Serial.println(" =  gira ");
 
     
     pos = 105;// gira
      myservo.write(pos);
       myservo2.write(pos);
       delay (3000);
   
  }
     
}

//////////////////////
// si pos< 95 aprox giro atras
// si pos > 95 aprox giro adelante
// 0 <= pos <= 180
 


//////////////////

