/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
*/

int value = 90; // Indica velocidad a girar

int pwmpinA = 5; // motor A connected to analog pin 1

int pwmpinB = 6; // motor B connected to analog pin 2

int dirpinA = 7; // motor A direction pin

int dirpinB = 8; // motor B direction pin

boolean directionA = true; // toggling direction for motor A

boolean directionB = true; // toggling direction for motor B

void setup() {

   // nothing for setup 

} void loop() {

  
      digitalWrite(dirpinB,HIGH);//esto es para cambiar sentido giro
   
      digitalWrite(dirpinA,HIGH);
 


      analogWrite(pwmpinA, value);  // sets the value (range from 0 to 255)
      analogWrite(pwmpinB, value);  // sets the value (range from 0 to 255)

 

} 
