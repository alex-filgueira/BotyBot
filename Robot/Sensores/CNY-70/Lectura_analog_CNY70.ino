/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
Este programa lee el valor de 3 sensores CNY-70
Estos sensores ofrecen una salida analógica y eestán conectados a los pines:
2,3 y 4. El valor obtenido es enviado por puerto serie.
*/
int CNY1 = 4; //pin analog
int CNY2 = 2; //pin analog
int CNY3 = 3; //pin analog

int varCNY1 = 0;//valores de los CNY
int varCNY2 = 0;
int varCNY3 = 0;


void setup()
{
  Serial.begin(9600);
 
}

void loop()
{
  varCNY1 = analogRead(CNY1);//lectura de los valores analog
  varCNY2 = analogRead(CNY2);
  varCNY3 = analogRead(CNY3);

  varCNY1 = varCNY1 * 5 / 1024; // Paso de bits a voltios
  varCNY2 = varCNY1 * 5 / 1024;
  varCNY3 = varCNY1 * 5 / 1024;
 
  Serial.print("CNY1 = ");
  Serial.println(varCNY1);
  Serial.print("CNY2 = ");
  Serial.println(varCNY3);
  Serial.print("CNY3 = ");
  Serial.println(varCNY3);

  delay (1000);// espera 1 seg
     
}


