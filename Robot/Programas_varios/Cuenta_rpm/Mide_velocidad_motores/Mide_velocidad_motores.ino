int Setpoint=0;
int pwmD=5;
int pwmI=6;
int dirD=7;
int dirI=8;

int nI=24;
int nD=24;

float tiempoI=0;
float tiempoD=0;
float velocidadI=0;
float velocidadD=0;
int contadorI=0;
int contadorD=0;

int contar=0;

void calculoVel();

void setup()
{
  Serial.begin(9600);
  
     //Interrupciones de los encoders
  attachInterrupt(1, I_interrupt, CHANGE); //dig3
  attachInterrupt(0, D_interrupt, CHANGE); //dig2
    
  pinMode(2,INPUT); // pin de interrupcion 0 
  pinMode(3,INPUT);
}

void loop()
{
 while (Serial.available())
 {
  char dato = Serial.read();
  switch(dato)
  {
   case'w':
   {
    Serial.println("ADELANTE");
    Serial.println(Setpoint);
    digitalWrite(dirD,HIGH);
    digitalWrite(dirI,HIGH);
    analogWrite(pwmD, Setpoint);  // sets the value (range from 0 to 255)
    analogWrite(pwmI, Setpoint);  // sets the value (range from 0 to 255)
    contar=1;
    break;
   }
   case 'e':
   {
    Serial.println("ATRAS");
            Serial.println(Setpoint);
    digitalWrite(dirI,LOW);//esto es para cambiar sentido giro
    digitalWrite(dirD,LOW);
    analogWrite(pwmI, Setpoint);  // sets the value (range from 0 to 255)
    analogWrite(pwmD, Setpoint);  // sets the value (range from 0 to 255)
    contar=1;
    break;
   }
   case 'r':
   {
    Serial.println("Parar");
    analogWrite(pwmD, 0);  //Fuerzo un 0 en el PWM
    analogWrite(pwmI, 0); 
    calculoVel();
    break;
   }
   case 'v':
   {
    Serial.print("+VEL");
    Setpoint = Setpoint+10; 
    Serial.println(Setpoint);
    break;
   } 
    case 'x':
   {
    Serial.print("-VEL");
    Setpoint = Setpoint-10; 
    Serial.println(Setpoint);
    break;
   }   
  }
 } 
 if(contar==1)
 {
  if(contadorI== 24)//espera una vuelta para alcanzar velocidad marcada
  {
    tiempoI=micros();
  }
  if(contadorD==24)
  {
   tiempoD=micros(); 
  }
 }
}

void calculoVel()
 {
   contar=0;
   velocidadI=(contadorI/nI)/(micros()-tiempoI);//Resulta vueltas/microsegundo
   velocidadI=1000000*60*velocidadI;
   
   velocidadD=(contadorD/nD)/(micros()-tiempoD);//Resulta vueltas/microsegundo
   velocidadD=1000000*60*velocidadD;
   
   contadorI=0;
   contadorD=0;
   
   Serial.print("A ");
   Serial.print(Setpoint);
   Serial.println("PWM");
   
   Serial.print("VEL_I = ");
   Serial.println(velocidadI);
   Serial.print("VEL_D = ");
   Serial.println(velocidadD);
 }

void I_interrupt() //rutina de interrupcion
 {
  contadorI++;
 }

void D_interrupt() //rutina de interrupcion
 {
  contadorD++;
 }
