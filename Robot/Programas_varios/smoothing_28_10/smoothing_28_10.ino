#include "Ultrasonic.h" //Se incluye la librería


 Ultrasonic ultrasonicD(10,11); //(Trigger pin,Echo pin)
 Ultrasonic ultrasonicI(13,12); //(Trigger pin,Echo pin)
 
 
void dondeestoy();
int smoothing(int nueva_lectura);

const int numeroLecturas=10;
int lecturas[numeroLecturas];
int index=0;
int total=0;

void setup()
{
  Serial.begin(9600);
}
void loop()
{

dondeestoy();
  

}  
  
  
void dondeestoy()//FUNCION UTILIZADA CUANDO NO HAY NINGUN OBSTACULO=PARED CERCA
{

 //Lectura en cm de los sensores laterales

 int distD_cm=ultrasonicD.Ranging(CM); //Medimos la distancia en cm.
 int distI_cm=ultrasonicI.Ranging(CM); //Medimos la distancia en cm.

   if((smoothing(distI_cm))< 35)//Si hay algo a la derecha
   {
    Serial.println("Izq");
   } 
}

int smoothing(int nueva_lectura)
{
  
  total=total-lecturas[index];
  lecturas[index]=nueva_lectura;
  total=total+lecturas[index];
  index=index+1;
  if(index==10)
  {
    index=0;
  }
 int media=total/numeroLecturas;
  Serial.println(media);
  return media;
}






