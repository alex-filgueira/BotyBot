/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
*/

void setup()
{

//Velocidad del modulo bluetooth, 9600 por defecto

Serial1.begin(9600);
Serial.begin(9600);

}
void loop()
{
while (Serial1.available()  > 0 ){

Serial.write(Serial1.read());
}
}
