/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
*/
///// Control de dos servos con 3 sensores IR  segun los cuales si el sensorPin <40 para y decide en funcion de sensorPin2 y sensorPin4
//Problema con sensorPin, pues detecta mucho ruido, hay que separarlo para que funcione bien. --> Considerable mejora con la tecnica de smoothing
// las interrupciones estn asociadas co nlas siguientes patillas:
//Int0-->3
//Int1-->2
//Int2-->0
//Int3-->1
#include <Servo.h> 

const int numReadings = 40; //numero de medidas para tomar en la media

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // la media
//////////////
const int numReadings2 = 40; //numero de medidas para tomar en la media

int readings2[numReadings2];      // the readings from the analog input
int index2 = 0;                  // the index of the current reading
int total2 = 0;                  // the running total
int average2 = 0;                // la media
//////////////
Servo myservo; 
Servo myservo2;
int pos = 0;    // variable to store the servo position 

int sensorPin = 0; //pin analogico 0
int varSensor = 0;
int sensorPin2 = 2; //pin analogico 2
int varSensor2 = 0;
int sensorPin4 = 4; //pin analogico 4
int varSensor4 = 0;
int sensorPin5 = 5; //pin analogico 5
int varSensor5 = 0;

int servo1=0;
int servo2=0;

void setup(){
  Serial.begin(9600);
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
    myservo2.attach(5); 

 attachInterrupt(2, fan_interrupt, CHANGE);// pin 0
  attachInterrupt(1, fan2_interrupt, CHANGE);// pin 2

  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;    
}

void loop(){// 
//PARA EL PRIMER SENSOR
// con esta funcion calcula la media de los sensores tomando  numReading medidas( en este caso 40 )
  // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(sensorPin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings) {             
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         
  // send it to the computer as ASCII digits 
  delay(2);        // delay in between reads for stability            
  }
  
  ////PARA EL SEGUNDO SENSOR
    // subtract the last reading:
  total2= total2 - readings2[index2];         
  // read from the sensor:  
  readings2[index2] = analogRead(sensorPin5); 
  // add the reading to the total:
  total2= total2 + readings2[index2];       
  // advance to the next position in the array:  
  index2 = index2 + 1;                    

  // if we're at the end of the array...
  if (index2 >= numReadings2) {             
    // ...wrap around to the beginning: 
    index2 = 0;                           

  // calculate the average:
  average2 = total2 / numReadings2;         
  // send it to the computer as ASCII digits 
  delay(2);        // delay in between reads for stability            
  }
  else goto fin; // para que siga haciendo la media

/////////////AQUI SIGO
  varSensor = average;   // Coge el valor de la media en vez de directamente el sensor
   varSensor5 = average2;   // Coge el valor de la media en vez de directamente el sensor
  Serial.print(" Delantera1 = ");
  Serial.println(varSensor);
    Serial.print(" Delantera5 = ");
  Serial.println(varSensor5);

  
  if(varSensor < 40 && varSensor5 < 40){ //visualizando con el monitor serie se observa que el sensor detecta distancias superiores a 30cm, por ejemplo 40*5/1023=0.19V que equivale a x cm

      Serial.println(" = Adelante "); 
      myservo.attach(3); 
      myservo2.attach(5);   
      pos = 15 ;           // movimiento "adelante" rapido
      myservo.write(pos);
      pos = 165; 
      myservo2.write(pos); 
      delay(2000);
      goto fin;
     
  }

que_hacer:

  // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(sensorPin2); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings) {             
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         
  // send it to the computer as ASCII digits 
  delay(2);        // delay in between reads for stability            
  }
  else goto que_hacer;  // para que siga haciendo la media
/////////////////////////////////////////////
  varSensor2 = average;  // Coge el valor de la media en vez de directamente el sensor
  if(varSensor2 < 40 ){ 
    servo1=0;
    servo2=0;
gizquierda:
    if(servo1 < 3 || servo2<3)
    {
      Serial.print(" Sensor2 = ");
      Serial.print(varSensor2);
      Serial.println(" Izquierda ");
      myservo.attach(3); 
      myservo2.attach(5);   
      pos = 15 ;          
      myservo.write(pos); // considero servo izquierda
      myservo2.write(pos); //considero servo derecha
      delay (15); //timpo de cortesia
      goto gizquierda;
    }
   else
   {
    goto fin;
   }
  }
  
    else{
derecha:     //se pueden hacer etiquetas apuntando al interior de bucles
        // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(sensorPin4); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings) {             
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         
  // send it to the computer as ASCII digits 
  delay(2);        // delay in between reads for stability            
  }
  else goto derecha;   // para que siga haciendo la media
      /////////////////////////////
      varSensor4 = average; // Coge el valor de la media en vez de directamente el sensor
      if(varSensor4 < 40){
        servo1=0;
        servo2=0;
gderecha:
        if(servo1 < 3 || servo2<3)
        {
          Serial.print(" Sensor4 = ");
          Serial.print(varSensor4);
          Serial.println(" Derecha ");
          myservo.attach(3); 
          myservo2.attach(5);   
          pos = 165 ;           
          myservo.write(pos); // considero servo izquierda
          myservo2.write(pos); //considero servo derecha
          delay (15); //timpo  de cortesia
          goto gderecha;
        }
        else goto fin;
      }
      
      else{
        Serial.print(" Atras ");
        myservo.attach(3); 
        myservo2.attach(5);   
        pos = 165 ;           // movimiento "atras" rapido
        myservo.write(pos); // considero servo izquierda
        pos= 15;
        myservo2.write(pos); //considero servo derecha
        
        goto que_hacer; 
      }
    }
  
fin:  ; 
}
void fan_interrupt()
{
   servo1++;
   
}

void fan2_interrupt()
{
  servo2++;
   
}

//////////////////////
// si pos< 95 aprox giro atras
// si pos > 95 aprox giro adelante
// 0 <= pos <= 180
 


//////////////////

