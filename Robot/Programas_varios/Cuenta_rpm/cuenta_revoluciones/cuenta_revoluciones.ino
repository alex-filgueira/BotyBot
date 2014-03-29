/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
*/

float revoluciones = 0 ;
volatile float variacionT = 0;
volatile float Tant = 0;
float m = 0;
volatile long interrupciones = 0;

void setup()                    
{
  pinMode(3,INPUT);  
  attachInterrupt(0, rpm, RISING);
  Serial.begin(9600);
}

void rpm()
{
 variacionT=millis() - Tant;
 Tant= millis();
 interrupciones ++;

}

void loop()                    
{
m = variacionT/60000;
revoluciones = 1 / m;

 Serial.println(" revoluciones = ");
  Serial.println(revoluciones);
   Serial.println(" interrupciones = ");
    Serial.println(interrupciones);
  

    delay(2000);



  }
    
  



