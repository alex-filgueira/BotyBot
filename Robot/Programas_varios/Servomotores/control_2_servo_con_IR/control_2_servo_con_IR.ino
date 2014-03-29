#include <Servo.h> 

Servo myservo; 
Servo myservo2;
int pos = 0;    // variable to store the servo position 

int sensorPin = 0; //pin analogico 0
int varSensor = 0;

void setup(){
  Serial.begin(9600);
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
    myservo2.attach(5); 
}

void loop(){// no hay tiempos de espera para el servo...
  varSensor = analogRead(sensorPin);
  Serial.print("Sensor indica: ");
  Serial.print(varSensor);
  
  if(varSensor < 40){ //visualizando con el monitor serie se observa que el sensor detecta distancias superiores a 30cm, por ejemplo 40*5/1023=0.19V que equivale a x cm
    Serial.println(" = Adelante"); 
      myservo.attach(3); 
      myservo2.attach(5);   
     pos = 180 ;           // movimiento "adelante" rapido
      myservo.write(pos);
      myservo2.write(pos); 
      
     
  }
  else{
    Serial.println(" = Para");
    myservo.detach();  // desconecta la salida de myservo
    myservo2.detach(); 
  }
     
}

//////////////////////
// si pos< 95 aprox giro atras
// si pos > 95 aprox giro adelante
// 0 <= pos <= 180
 


//////////////////

