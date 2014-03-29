
float tension = 0;
float entrada = 0;



void setup()
{
  Serial. begin(9600);
}
void loop()
{
 Serial.println(tension);
delay(2000);
entrada = analogRead(5) ;// lee el valor de la entrada analogica 5 y lo multiplica por 1023
tension= entrada *5 /1023 *3/2; // asi recupero el valor real antes del divisor de tension (respecto a 5V) que multiplica la tension en su entrada por 2/3


}
