///// Control de dos servos con 3 sensores IR  segun los cuales si el sensorPin <40 para y decide en funcion de sensorPin2 y sensorPin4
//Problema con sensorPin, pues detecta mucho ruido, hay que separarlo para que funcione bien. --> Considerable mejora con la tecnica de smoothing
#include <Servo.h> 

const int numReadings = 40; //numero de medidas para tomar en la media

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // la media
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

void setup(){
  Serial.begin(9600);
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
    myservo2.attach(5); 

  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0;    
}

void loop(){// 
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
  else goto fin; // para que siga haciendo la media

/////////////AQUI SIGO
  varSensor = average;   // Coge el valor de la media en vez de directamente el sensor
  Serial.print(" Sensor = ");
  Serial.print(varSensor);

  
  if(varSensor < 100 ){ //visualizando con el monitor serie se observa que el sensor detecta distancias superiores a 30cm, por ejemplo 40*5/1023=0.19V que equivale a x cm
      Serial.println(" = Adelante "); 
      myservo.attach(3); 
      myservo2.attach(5);   
      pos = 125 ;           // movimiento "adelante" rapido
      myservo.write(pos);
      pos = 65; 
      myservo2.write(pos); 
      delay(2000);
      goto fin;
     
  }
  else{
    Serial.println(" = Para");
    myservo.detach();  // desconecta la salida de myservo
    myservo2.detach(); 
    delay (500); //tiempo para que pare
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
  if(varSensor2 < 110 ){ 
    Serial.print(" Sensor2 = ");
    Serial.print(varSensor2);
    Serial.println(" Izquierda ");
    myservo.attach(3); 
    myservo2.attach(5);   
    pos = 65 ;           // movimiento "atras" rapido
    myservo.write(pos); // considero servo izquierda
    myservo2.write(pos); //considero servo derecha
    delay (1000); //timpo para que gire lo suficiente
    goto fin;
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
      if(varSensor4 < 110){
        Serial.print(" Sensor4 = ");
        Serial.print(varSensor4);
        Serial.println(" Derecha ");
        myservo.attach(3); 
        myservo2.attach(5);   
        pos = 125 ;           // movimiento "adelante" rapido
        myservo.write(pos); // considero servo izquierda
        myservo2.write(pos); //considero servo derecha
        delay (1000); //timpo para que gire lo suficiente
        goto fin;
      }
      
      else{
        Serial.print(" Atras ");
        myservo.attach(3); 
        myservo2.attach(5);   
        pos = 65 ;           // movimiento "atras" rapido
        myservo.write(pos); // considero servo izquierda
        pos= 125;
        myservo2.write(pos); //considero servo derecha
        
        goto que_hacer; 
      }
    }
  
fin:  ; 
}

//////////////////////
// si pos< 95 aprox giro atras
// si pos > 95 aprox giro adelante
// 0 <= pos <= 180
 


//////////////////

