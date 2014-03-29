#include <Servo.h> 
#include <Ultrasonic.h>

Ultrasonic ultrasonic(5,7); //(Trigger pin,Echo pin) pines digitales

Servo myservo; 
Servo myservo2; 

int pos = 0;    // variable to store the servo position 

int dist_cm=0; //Ultra sonidos

int sensorPin = 0; //pin analogico 0 IR
int sensorPin1 = 1; //pin analogico 1 IR, considero izquierda
int sensorPin3 = 3; //pin analogico 3 IR  considero derecha

int varSensor = 0; //IR
int varSensor1 = 0; //IR
int varSensor3 = 0; //IR

void setup(){
  Serial.begin(9600); //conexion USB
    myservo.attach(3);  // attaches the servo on pin 3 to the servo object 
    myservo2.attach(9);  // attaches the servo on pin 3 to the servo object 
}

void loop()
{


//  MEDIDA DE DISTANCIA SENSOR US
    dist_cm=ultrasonic.Ranging(CM); //Medimos la distancia en cm.
  
    Serial.print("ultrasonidos indica: ");
    Serial.println(dist_cm);
    delay(6);          //este delay es para realizar bien las medidas, con menos tiempo se observan problemas
/////////////////////////

//COMPROBACION OBJETO IR0
    varSensor = analogRead(sensorPin);
    Serial.print(" Sensor 0 = ");
    Serial.print(varSensor);
  /////////////////
  
    if(varSensor < 30  ) //visualizando con el monitor serie se observa que el sensor detecta distancias superiores a 30cm, por ejemplo 40*5/1023=0.19V que equivale a x cm
      {
         Serial.println(" = Adelante"); 
         myservo.attach(3);  
         myservo2.attach(9); 
         pos = 180 ;           // movimiento "adelante" rapido
         myservo.write(pos);  
         myservo2.write(pos);   
        goto fin; 
      }
     
  //IR0 o US detectan algo entonces...
    else
      {
        Serial.println(" = Para");
        myservo.detach();  // desconecta la salida de myservo
        myservo2.detach();  // desconecta la salida de myservo2
        delay (500);// para que le de tiempo a pararse, podria usar millis de alguna forma
      }
     
que_hacer:
         varSensor1 = analogRead(sensorPin1);//mide IR1
         Serial.print(" Sensor 1 = ");
         Serial.print(varSensor1);
         varSensor3 = analogRead(sensorPin3);//mide IR3
         Serial.print(" Sensor 3 = ");
         Serial.print(varSensor3);
         
         //dist_cm=ultrasonic.Ranging(CM); //Medimos la distancia en cm.
        // Serial.print("ultrasonidos indica: ");
        // Serial.println(dist_cm);
        // delay(6);          //este delay es para realizar bien las medidas, con menos tiempo se observan problemas

         
         if(varSensor1 <40)
            {
              Serial.println(" = Izquierda "); 
              myservo.attach(3);  
              myservo2.attach(9); 
              pos = 0 ;           // movimiento "atras" rapido
              myservo.write(pos);  //considero servo izquierda
              pos = 180;            //movimiento "adelante" rapido
              myservo2.write(pos);  // considero servo derecha
              delay (3000); // tiempo para que gire 90º
              
            }
          else
            {

              if(varSensor3 < 40)
                {
                  Serial.println(" = Derecha "); 
                  myservo.attach(3);  
                  myservo2.attach(9); 
                  pos = 180 ;           // movimiento "adelante" rapido
                  myservo.write(pos);  //considero servo izquierda
                  pos = 0;            //movimiento "atras" rapido
                  myservo2.write(pos);  // considero servo derecha
                  delay (3000);      // tiempo para que gire 90º

                }
              else
              {
              Serial.println(" = Atras ");
              myservo.attach(3);  
              myservo2.attach(9); 
              pos = 0 ;           // movimiento "atras" rapido
              myservo.write(pos);  //considero servo izquierda
              pos = 0;            //movimiento "atras" rapido
              myservo2.write(pos);  // considero servo derecha
              goto que_hacer; // salta a que_hacer para ver si sigue retrocediendo o gira a izquierad o derecha
              }
            }  
            
fin:;       
       }


     
       

//////////////////////
// si pos< 95 aprox giro atras
// si pos > 95 aprox giro adelante
// 0 <= pos <= 180
 


//////////////////


