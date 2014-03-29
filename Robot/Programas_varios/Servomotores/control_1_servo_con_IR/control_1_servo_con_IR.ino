#include <Servo.h> 

Servo myservo; 
int pos = 0;    // variable to store the servo position 

int sensorPin = 0; //pin analogico 0
int varSensor = 0;

void setup(){
  Serial.begin(9600);
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
}

void loop(){// no hay tiempos de espera para el servo...
  varSensor = analogRead(sensorPin);
  Serial.print("Sensor indica: ");
  Serial.print(varSensor);
  
  if(varSensor < 40){ //visualizando con el monitor serie se observa que el sensor detecta distancias superiores a 30cm, por ejemplo 40*5/1023=0.19V que equivale a x cm
    Serial.println(" = Adelante"); 
      myservo.attach(3);   
     pos = 180 ;           // movimiento "adelante" rapido
      myservo.write(pos); 
  delay(15);    
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

