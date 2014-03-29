/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
*/
#include <Ultrasonic.h>
/*
Mario Pérez Esteso
https://www.youtube.com/user/telecoreference
http://www.diarioelectronicohoy.com/arduino/
https://twitter.com/_Mario_Perez

Facebook page! Join us with a LIKE!
https://www.facebook.com/TutorialesArduino
*/
#include <Servo.h> 


Ultrasonic ultrasonic(5,7); //(Trigger pin,Echo pin) pines digitales
Servo myservo; 

int pos = 0;    // variable to store the servo position 
int dist_cm=0;
int sensorPin = 0; //pin analogico 0
int varSensor = 0;

void setup(){
  Serial.begin(9600);
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
}

void loop(){// no hay tiempos de espera para el servo...


  dist_cm=ultrasonic.Ranging(CM); //Medimos la distancia en cm.
  
  Serial.print("ultrasonidos indica: ");
  Serial.println(dist_cm);
  delay(6);          //este delay es para realizar bien las medidas, con menos tiempo se observan problemas



  varSensor = analogRead(sensorPin);
  Serial.print(" Sensor indica: ");
  Serial.print(varSensor);
  
  if(varSensor < 40 && dist_cm > 20){ //visualizando con el monitor serie se observa que el sensor detecta distancias superiores a 30cm, por ejemplo 40*5/1023=0.19V que equivale a x cm
    Serial.println(" = Adelante"); 
      myservo.attach(3);   
     pos = 180 ;           // movimiento "adelante" rapido
      myservo.write(pos);     
  }
  else{
    Serial.println(" = Para");
    myservo.detach();  // desconecta la salida de myservo
  }
     
}

//////////////////////
// si pos< 95 aprox giro atras
// si pos > 95 aprox giro adelante
// 0 <= pos <= 180
 


//////////////////


