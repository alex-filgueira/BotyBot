/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
Este programa es el empleado en el robot BotyBot a día 28/01/2014.
*/
#include "Ultrasonic.h" //Se incluye la librería
// Ultrasonic - Library for HR-SC04 Ultrasonic Ranging Module.
// Rev.4 (06/2012)
// J.Rodrigo ( www.jra.so )
// more info at www.ardublog.com

//FUNCIONS Basicas
void  leer_bluetooth();
float  calculo_v();
float  regulador();
int  rpm_a_pwm(); //Paso de rpm a pwm

//VARIABLES Basicas
int parar=1;
int contadorI;//Usado para o calculo da velocidade
int contadorD;
int nI=24;//Numero de marcas dos encoders
int nD=24;
int v_referencia=50;

int pwmD=5;//pines conexion motor
int pwmI=6;
int dirD=7;//pines conexion motor
int dirI=8;

float tiempoI=0;//Utilizado para o calcuo da velocidade
float tiempoD=0;

float v_realI;
float v_realD;


float kp1=0.5;//Utilizado no regulador
float kp2=1;
float rpmI;
float rpmD;
float ultima_regulacion;
float tiempo_entre_regulaciones= 333.3;//333.3 millis

float ultimo_calculo;//A ultima actualizacion da velocidade
float tiempo_entre_calculos=333.3;// Ten que ser igual a tiempo_entre_regulaciones

//Declaro funcions MODO AUTOMATICO
void enpared_derecha();
void enpared_izquierda();
void enpared_delantera();
void dondeestoy();
void girar90grados();
//Declaro variables de acceso para as funcions anteriores
int en_pared_derecha=0;
int en_pared_izquierda=0;
int en_pared_delantera=0;
int donde_estoy=1; 
int girar_90_grados;

//IR
int sensor_delant_izq=5;//Declaro cada sensor IR co seu pin
int sensor_delant_decha=1;
//US
Ultrasonic ultrasonicD(10,11); //(Trigger pin,Echo pin)
Ultrasonic ultrasonicI(13,12); //(Trigger pin,Echo pin)


int acelerando_izquierda=0;//Variable utilizada para acercarse ou alexarse dunha parede
int acelerando_derecha=0;
int motores_ON=0;
int alejando=0;
int acercando=0;
int xirar_a_dereita=0;

const float pi=3.14;//Para Xirar
int grados_a_girar;
int pulsos_a_girar=0;

int esta_en_pared_derecha=0;
int xirar_a_esquerda=0;

int atras=0;
int contadorD2_ultimo=0;
int contadorI2_ultimo=0;//Garda o valor do contadorI2
float ultimo_control_parado=0;// Variable para medir o tempo que esta parado e ver cando se "atasca"
int contadorI2=0;//Se utiliza para recorrer unha distancia determinada hacia atras
int contadorD2=0;

//SMOOTHING
int smoothing_izq(int nueva_lectura);//Array

const int numeroLecturas=20;//Numero de valores almaceados na taboa, ATENCION! ten que ser tipo const para que NON de ERRO
int lecturas_izq[numeroLecturas];//Declaracion da taboa
int index_izq=0;//Puntero
int total_izq=0;//Variable usada para sumar o valor total da taboa

int smoothing_dcha(int nueva_lectura);

int lecturas_dcha[numeroLecturas];
int index_dcha=0;
int total_dcha=0;
////
int contadorI3=0;//Utilizado para os xiros
int pared_izq_perdida=0;//Constante que indica que xa non "ve a parede"
float tiempo_pared_izq_perdida=0;//Utilizado para medir o tempo que pasa sen  "ver a parede" e asi recorrer unha distancia de seguridade

int pared_dcha_perdida=0;
float tiempo_pared_dcha_perdida=0;
///App Inventor
void manual();// Funcion principal do modo MANUAL
void automatico();// Funcion principal do modo AUTOMATICO
int control_manual=1;
int control_automatico=0;

int antichoque=0;
float tempo_avisos_cercania=0;// Variable utilizada para medir o tempo entre os avisos de cercania
int adiante=0;
int numero_de_avisos=0; //numero de avisos emitidos cada vez
int rpm_enviar = 0; //Variable utilizada para enviar a velocidade das rodas
//////
////SIGUE_LINEAS
void sigueLineas();
int CNY1 = 4; //pin analog
int CNY2 = 2; //pin analog
int CNY3 = 3; //pin analog
int varCNY3=0;
int varCNY2=0;
int varCNY1=0;
int error_sl=0; //Variables utilizadas no regulador do modo SIGUE_LINEAS
int error_sl_ant=0;
int sigue_lineas=0;

float ultima_regulacion_sl=0;
int Variar_V=0;

float Kp_sl=0;
float Kd_sl=0;

int autoaxuste = 0;
float ulti_autoaxuste = 0.0;

int minValorCNY = 0;
int maxValorCNY = 0;
int valorCNY = 600; // Este e un valor que funciona ben
//////////////
///CONTROL CARGA
float nivel_bateria=0.0;
int pin_bateria=0;//O control da bateria esta conectado ao ANALOXICO-0
float control_carga_anterior = 0.0;
int cont_avisos_carga_baixa = 0;
int confirmacion_carga = 0;
int cont_avisos_carga_alta = 0;


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
  //LEE BLUETOOTH
  leer_bluetooth();

  ///////////MANUAL
  if(control_manual==1)
  {
    manual();  
  }
  /////FIN MANUAL

  //////////////AUTOMATICO
  if(control_automatico==1)
  {
    automatico();
  }
  ///////FIN AUTOMATICO

  ///////////SIGUE_LINEAS
  if(sigue_lineas==1)
  {
    sigueLineas();  
  }
  /////FIN SIGUE_LINEAS


  ///------------------------PARTE COMUN-------------
  //CALCULO DA VELOCIDADE
  if (millis()-ultimo_calculo > tiempo_entre_calculos)//este tempo ten que ser igual a tiempo_entre_regulaciones
  {
    ultimo_calculo=millis();

    v_realD=calculo_v(contadorD, nD, tiempoD);
    contadorD=0; //Resetea contador
    tiempoD=micros(); // Actualiza tempo para calculos

      v_realI=calculo_v(contadorI, nI, tiempoI);
    contadorI=0;
    tiempoI=micros();
  }

  //CALCULA REGULADOR
  if(sigue_lineas==0)//O siguelineas ten seu propio regulador
  {
    if(millis()- ultima_regulacion > tiempo_entre_regulaciones)//Para chamar ao regulador cada ZZ segundos
    {
      rpmI=regulador(v_realI, v_realD, acelerando_izquierda);
      rpmD=regulador(v_realD, v_realI, acelerando_derecha); 
    }
  }
  
  //CONTROL CARGA
  if (millis()- control_carga_anterior > 5000) // Tempo que pasa entre cada control
  {
    control_carga_anterior = millis();
    nivel_bateria = analogRead(pin_bateria); // Lee o pin indicado
    if (nivel_bateria > 100) // Esto evita que envie cando non esta conectado a bateria
    {
      nivel_bateria = (nivel_bateria * 5) / 1023; // Converte o valor en Voltios
      nivel_bateria = nivel_bateria * 3.35; // 3.35 e polo que dividen as resistencias do divisor de tension
      if(nivel_bateria < 7.2) //7.2 // Este e o nivel de control
      {
       if(confirmacion_carga >=3) // Confirma un numero de veces que pasou por debaixo do limite
       {
        //if (cont_avisos_carga_baixa < 3000) // envia 3 veces o aviso de carga baixa
        //{
           Serial.println('3');
           cont_avisos_carga_baixa = cont_avisos_carga_baixa + 1;
           cont_avisos_carga_alta = 0;
        //}
       }
      else
      {
         confirmacion_carga = confirmacion_carga +1;
      } 
      }
      else
      {
         if (confirmacion_carga > 0) // Asi asegura 3 veces que a carga  alta
         {
            confirmacion_carga = confirmacion_carga -1;
         } 
         else
         {
           if (nivel_bateria > 8) // si o nivel da carga e maior que 8 e que seguramente foi recargada
           {
             if (cont_avisos_carga_alta <= 3) // Avisa 3 veces do nivel de carga alta
             {
                Serial.println('4');
                cont_avisos_carga_baixa = 0; 
                cont_avisos_carga_alta = cont_avisos_carga_alta + 1;
             }
           }
         }
      }

    }
  }


  //CONTROL MOTORES

  if(atras==1) //Si esta parado, retrocede mentras esta constante sea = 1
  {
    digitalWrite(dirI,LOW);//esto e para cambiar sentido xiro
    digitalWrite(dirD,LOW);
    analogWrite(pwmI, rpm_a_pwm(rpmI));  // chama a funcion rpm_a_pwm e lle carga o valor rpmI
    analogWrite(pwmD, rpm_a_pwm(rpmD));  
    //Serial.println("atras");
  }
  else
  {
    if((parar==1) ||(motores_ON==0))// constante para que pare e arranque
    {
      analogWrite(pwmI, 0); 
      analogWrite(pwmD, 0);
    }
    else
    {
      analogWrite(pwmI, rpm_a_pwm(rpmI));  // Chama a funcion rpm_a_pwm e lle carga o valor rpmI
      analogWrite(pwmD, rpm_a_pwm(rpmD));
    }
    if(xirar_a_dereita==1)//Xirando a dereita
    {
      digitalWrite(dirI,HIGH);//Isto e para cambiar sentido xiro
      digitalWrite(dirD,LOW);
    }
    else
    {
      if(xirar_a_esquerda==1)//Xira a esquerda
      {
        digitalWrite(dirI,LOW);
        digitalWrite(dirD,HIGH); 
      }
      else
      {
        if(adiante==0)
        {
          analogWrite(pwmI, 0); 
          analogWrite(pwmD, 0);
        }
        digitalWrite(dirI,HIGH);//Sentido adiante
        digitalWrite(dirD,HIGH);
      }
    }
  }
}  

void sigueLineas()//RUTINA SIGUE_LINEAS
{
  varCNY1 = analogRead(CNY1);//lectura dos valores analog
  //Serial.print("CNY1 = ");
  //Serial.println(varCNY1);
  varCNY2 = analogRead(CNY2);
  //Serial.print("CNY2 = ");
  //Serial.println(varCNY2);
  varCNY3 = analogRead(CNY3);
  //Serial.print("CNY3 = ");
  //Serial.println(varCNY3);
  //delay(1000);

  //Localizando linea
  //-------------------------------------
  if(autoaxuste == 1)
  {
    if((millis() - ulti_autoaxuste) < 4000) // 4 segundos xirando
    {
      xirar_a_dereita = 1;
      rpmI = 50;
      rpmD = 50;
      adiante=0;
      motores_ON=1;
      parar=0;
      xirar_a_esquerda=0;
      atras=0;
      
      // Actualizar valores max
      if(varCNY1 > maxValorCNY)
      {
        maxValorCNY = varCNY1;
      }
      if(varCNY2 > maxValorCNY)
      {
        maxValorCNY = varCNY2;
      }
      if(varCNY3 > maxValorCNY)
      {
        maxValorCNY = varCNY3;
      }
      // Actualizar valores min
      if(varCNY1 < minValorCNY)
      {
        minValorCNY = varCNY1;
      }
      if(varCNY2 < minValorCNY)
      {
        minValorCNY = varCNY2;
      }
       if(varCNY3 < minValorCNY)
      {
        minValorCNY = varCNY3;
      }

    }
    else
    {

      autoaxuste = 0;
      xirar_a_dereita = 0;
      rpmI = 0;
      rpmD = 0;
      motores_ON=0;
      parar = 1;
      
      valorCNY = (maxValorCNY - minValorCNY)/2;
    }
  }
  else
  {
    
  //-------------------------------------
  if(varCNY1<valorCNY) //Elixo 600 por que e un valor que diferencia a linea da mesa 
  {
    if(varCNY2<valorCNY)
    {
      error_sl=-1;// Desplazado un espazo do centro
    } 
    else
    {
      error_sl=-2; //A linea esta desplazada dous espazos do centro
    }
  }
  if(varCNY3<valorCNY)
  {
    if(varCNY2<valorCNY)
    {
      error_sl=1;
    } 
    else
    {
      error_sl=2; 
    }
  }
  if((varCNY2 < valorCNY) && (varCNY1 > valorCNY) && (varCNY3 > valorCNY))
  {
    error_sl=0;//Linea centrada no sensor central
  }
  if((error_sl_ant==-2)&&(varCNY2>valorCNY) && (varCNY1>valorCNY) && (varCNY3>valorCNY))//Se sae pola esquerda
  {
    error_sl=-3; 
  }
  if((error_sl_ant==2)&&(varCNY2>valorCNY) && (varCNY1>valorCNY) && (varCNY3>valorCNY))//Se sae pola dereita
  {
    error_sl=3;
  }
  //Serial.print("error = ");
  //Serial.println(error_sl);

  //Linea localizada
  //Regulador
  if((millis()-ultima_regulacion_sl)>100)//Isto e necesario para o correcto funcionamento do regulador derivativo
  {
    Variar_V=Kp_sl*error_sl + Kd_sl*(error_sl-error_sl_ant);
    error_sl_ant=error_sl;
    ultima_regulacion_sl=millis();
    rpmI=v_referencia + Variar_V;
    rpmD=v_referencia - Variar_V;
    //Control para que non rebase a velocidade maxima calculada nin para que non sexa menor que 0
    if(rpmI<0)
    {
      rpmI=0; 
    }
    if(rpmI>v_referencia)
    {
      rpmI=v_referencia;
    }
    if(rpmD<0)
    {
      rpmD=0; 
    }
    if(rpmD>v_referencia)
    {
      rpmD=v_referencia;
    }
  }
  //Serial.println(Variar_V);
  }
}



void automatico()//RUTINA DE CONTROL_AUTOMATICO
{
  if((millis()-ultimo_control_parado)>700)//CONTROL DE BLOQUEADO  e tempo que este retrocedendo
  {
    ultimo_control_parado=millis();
    if((contadorI2==contadorI2_ultimo)&&(contadorD2==contadorD2_ultimo)&&(parar==0)&&(motores_ON==1))// Comproba si esta parado e tiña que estar movendose
    {
      atras=1; //constante para uso en motores
    }
    else
    {
      atras=0; 
      contadorI2_ultimo=contadorI2;
      contadorD2_ultimo=contadorD2;
    }
  }

  //SELECCIONS DE FUNCIOns DE RUTA
  if(donde_estoy==1)
  {
    dondeestoy(); 
    //Serial.println("donde_estoy");
  }
  if(girar_90_grados==1)
  {
    girar90grados();
  }
  if(en_pared_delantera==1)
  {
    enpareddelantera(); 
    //Serial.println("en_pared_delantera");
  }
  if(en_pared_izquierda==1)
  {
    enparedizquierda();
    //Serial.println("en_pared_izquierda"); 
  }
  if(en_pared_derecha==1)
  {
    enparedderecha(); 
    //  Serial.println("en_pared_derecha");
  }
}



void manual()//RUTINA DE CONTROL_MANUAL
{
  if((millis()-tempo_avisos_cercania)>200)// Tempo que pasa entre cada control de obstaculos
  {
    tempo_avisos_cercania=millis();

    //lectura en V dos sensores dianteiros
    float valor_sensor_delant_izq= (analogRead(sensor_delant_izq))*5/1024;
    float valor_sensor_delant_decha= (analogRead(sensor_delant_decha))*5/1024;
    if(valor_sensor_delant_izq > 1.2 || valor_sensor_delant_decha > 1.2)//Si atopa algo a menos de Xcm
    {
      numero_de_avisos=0;
      Serial.println('1');//Perigo obstaculo diante de Boty
      if(antichoque==1)//Si o antichoque esta activado, non permite avanzar adiante
      {
        adiante=0;        
      }
    } 
    else
    {
      if(numero_de_avisos<3)
      {
        Serial.println('2');// Isto significa que non ai perigo
        numero_de_avisos++;
      }
    }
  }    
}


void dondeestoy()//FUNCION UTILIZADA CANDO NON HAI NINGUN OBSTACULO=PARED CERCA
{

  //lectura en V dos sensores dianteiros
  float valor_sensor_delant_izq= (analogRead(sensor_delant_izq))*5/1024;
  float valor_sensor_delant_decha= (analogRead(sensor_delant_decha))*5/1024;

  //Lectura en cm dos sensores laterais

  int distD_cm=ultrasonicD.Ranging(CM); //Medimos la distancia en cm.
  int distI_cm=ultrasonicI.Ranging(CM); //Medimos la distancia en cm.

  if(valor_sensor_delant_izq > 1.2 || valor_sensor_delant_decha > 1.2)//Si hai algo a menos de Xcm
  {
    motores_ON=0;
    en_pared_delantera=1;
    donde_estoy=0;
  }
  else
  {
    if((smoothing_dcha(distD_cm))< 20)//Chama a funcion smoothing_dcha e lle pasa o valor distD_cm, o valor devolto por esta funcion e comparado co <
    {
      en_pared_derecha=1;
      donde_estoy=0; 
    }
    else
    {
      if((smoothing_izq(distI_cm))<20)//Si hai algo a esquerda
      {
        en_pared_izquierda=1;
        donde_estoy=0; 
      }
      else// si non encontra nada SIGUE ADIANTE
      {
        if((pared_izq_perdida==1)&&(millis()-tiempo_pared_izq_perdida>500))
        {
          donde_estoy=0;
          girar_90_grados=1;
          contadorI3=0;
        }
        else
        {
          if((pared_dcha_perdida==1)&&(millis()-tiempo_pared_dcha_perdida>500))
          {
            donde_estoy=0;
            girar_90_grados=1;
            contadorI3=0;
          }
          else
          {
            motores_ON=1;
            adiante=1;
          }   
        }
      }
    } 
  }
}

void girar90grados() //Funcion empregada para XIRAR 90º
{
  if(contadorI3>6)
  { 
    girar_90_grados=0;
    donde_estoy=1;

    pared_izq_perdida=0;
    xirar_a_esquerda=0;
    pared_dcha_perdida=0;
    xirar_a_dereita=0;
  } 
  else
  {
    if(pared_izq_perdida==1)
    {
      xirar_a_esquerda=1;
    }
    if(pared_dcha_perdida==1)
    {
      xirar_a_dereita=1; 
    }
  }
}


void enpareddelantera()//FUNCION UTILIZADA SI HAI UN OBSTACULO=PARED DELANTE
{
  //lectura en V dos sensores delanteiros
  float valor_sensor_delant_izq= (analogRead(sensor_delant_izq))*5/1024;
  float valor_sensor_delant_decha= (analogRead(sensor_delant_decha))*5/1024;

  if((esta_en_pared_derecha==1)&&( valor_sensor_delant_izq > 1 || valor_sensor_delant_decha > 1))//Si estaba seguindo unha parede a sua dereita xira cara a esquerda para seguir seguindoa
  {
    motores_ON=1;
    xirar_a_esquerda=1;   
  }
  else
  {
    if(( valor_sensor_delant_izq > 1 || valor_sensor_delant_decha > 1)&&(esta_en_pared_derecha==0)) //Si non estaba seguindo unha parede a su dereita
    {
      motores_ON=1;
      xirar_a_dereita=1;
    }
    else// Si xa non hai obstaculo
    {
      motores_ON=0;
      xirar_a_dereita=0;
      xirar_a_esquerda=0;
      en_pared_delantera=0;
      donde_estoy=1;
    }
  }
}



void enparedizquierda()//FUNCION PARA SEGUIR UNHA PAREDE A ESQUERDA
{
  pared_izq_perdida=0;
  esta_en_pared_derecha=0;

  float valor_sensor_delant_izq= (analogRead(sensor_delant_izq))*5/1024;
  float valor_sensor_delant_decha= (analogRead(sensor_delant_decha))*5/1024;

  if( valor_sensor_delant_izq > 1 || valor_sensor_delant_decha > 1) //Si hai algo a menos de X cm
  {
    en_pared_izquierda=0;
    donde_estoy=1;
    alejando=0;
    acercando=0;
    acelerando_izquierda=0;
    acelerando_derecha=0;
  }
  else
  {
    //Lectura en cm dos sensores laterais
    int distI_cm=ultrasonicI.Ranging(CM); //Medimos a distancia en cm.
    //Serial.println(distI_cm);
    if((smoothing_izq(distI_cm))<15)//Distancia a ESQUERDA < Distancia MAX
    {
      if(alejando==1)
      {
        motores_ON=1;
        acelerando_izquierda=19;// Variable responsable de acercarse e alexarse das paredes
        acelerando_derecha=0;
          //Serial.println("alexando < 12");
        adiante=1;
      } 
      else
      {
        if(acercando==1)
        {
          if((smoothing_izq(distI_cm))<9)//Distancia a ESQUERDA < Distancia min
          {
            acercando=0;
            alejando=1; 
          }
          else
          {
            motores_ON=1;
            acelerando_derecha=10; 
            acelerando_izquierda=0;
            //Serial.println("acercando > 9");
            adiante=1;
          }
        } 
        else
        {
          alejando=1; 
        }
      }
    }
    else
    {
      if((smoothing_izq(distI_cm))<35)// Distancia a ESQUERDA < Distancia de parede perdida
      {
        alejando=0;
        acercando=1;
        motores_ON=1;
        acelerando_derecha=10;
        acelerando_izquierda=0;
         //Serial.println("acercando < 35");
        adiante=1;
      }
      else
      {
        en_pared_izquierda=0;
        donde_estoy=1; 
        alejando=0;
        acercando=0;
        acelerando_izquierda=0;
        acelerando_derecha=0;

        pared_izq_perdida=1;
        tiempo_pared_izq_perdida=millis();
      }
    }
  }
}


void enparedderecha() //FUNCION UTILIZADA PARA SEGUIR UNHA PAREDE A DEREITA
{
  pared_dcha_perdida=0;
  esta_en_pared_derecha=1;

  float valor_sensor_delant_izq= (analogRead(sensor_delant_izq))*5/1024;
  float valor_sensor_delant_decha= (analogRead(sensor_delant_decha))*5/1024;

  if( valor_sensor_delant_izq > 1 || valor_sensor_delant_decha > 1) //Si hai algo a menos de X cm
  {
    en_pared_derecha=0;
    donde_estoy=1;
    alejando=0;
    acercando=0;
    acelerando_izquierda=0;
    acelerando_derecha=0;
  }
  else
  {
    
    int distD_cm=ultrasonicD.Ranging(CM); //Medimos a distancia en cm.
    if((smoothing_dcha(distD_cm))<12)//Distancia a DEREITA < Distancia MAX
    {
      if(alejando==1)
      {
        motores_ON=1;
        acelerando_izquierda=0;
        acelerando_derecha=19;
        //Serial.println("alexando");
        adiante=1;
      } 
      else
      {
        if(acercando==1)
        {
          if((smoothing_dcha(distD_cm))<9)//Distancia a DEREITA < Distancia min
          {
            acercando=0;
            alejando=1; 
          }
          else
          {
            motores_ON=1;
            acelerando_derecha=0; 
            acelerando_izquierda=10;
            // Serial.println("acercando");
            adiante=1;
          }
        } 
        else
        {
          alejando=1; 
        }
      }
    }
    else
    {
      if((smoothing_dcha(distD_cm))<30)// Distancia a DEREITA < Distancia de pared perdida
      {
        alejando=0;
        acercando=1;
        motores_ON=1;
        acelerando_derecha=0;
        acelerando_izquierda=10;
        //Serial.println("acercando superada MAX");
        adiante=1;
      }
      else
      {
        en_pared_derecha=0;
        donde_estoy=1; 
        alejando=0;
        acercando=0;
        acelerando_izquierda=0;
        acelerando_derecha=0;
        pared_dcha_perdida=1;
        tiempo_pared_dcha_perdida=millis();
      }
    }
  }
}

//////////////////////////////////////////////FUNCIONES DE SMOOTHING//////////////////////////////////////////////////
int smoothing_izq(int nueva_lectura)// Funcion para calcular a media do US esquerdo
{

  total_izq=total_izq-lecturas_izq[index_izq];//Elimina o valor existente nesa posicion
  lecturas_izq[index_izq]=nueva_lectura;//Carga a lectura na taboa
  total_izq=total_izq+lecturas_izq[index_izq];//Suma o valor desta nova lectura
  index_izq=index_izq+1;//aumenta o punteiro da taboa
  if(index_izq==numeroLecturas)//Si o punteiro desborda, volve a 0
  {
    index_izq=0;
  }
  int media=total_izq/numeroLecturas;//Calcula a media dos valores almaceados
  delay(2);
  return media;//Devolve la media
}

int smoothing_dcha(int nueva_lectura)// Funcion para calcular a media do US dereito
{

  total_dcha=total_dcha-lecturas_dcha[index_dcha];
  lecturas_dcha[index_dcha]=nueva_lectura;
  total_dcha=total_dcha+lecturas_dcha[index_dcha];
  index_dcha=index_dcha+1;
  if(index_dcha==numeroLecturas)
  {
    index_dcha=0;
  }
  int media=total_dcha/numeroLecturas;
  delay(2);
  return media;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void leer_bluetooth()
{
  while (Serial.available())//Si hai datos esperando...
  {
    char dato = Serial.read();
    switch(dato)
    {
    case'a':
      {
        // Serial.println("ADIANTE");
        adiante=1;
        motores_ON=1;
        parar=0;
        xirar_a_dereita=0;
        xirar_a_esquerda=0;
        atras=0;
        sigue_lineas=0;
        break;
      }
    case's':
      {
        // Serial.println("Sigue_lineas");
        sigue_lineas=1;
        adiante=1;
        motores_ON=1;
        parar=0;
        xirar_a_dereita=0;
        xirar_a_esquerda=0;
        atras=0;
        break;
      }
    case 't':
      {
        // Serial.println("ATRAS");
        atras=1;
        motores_ON=1;
        adiante=0;
        parar=0;
        xirar_a_dereita=0;
        xirar_a_esquerda=0;
        sigue_lineas=0;
        break;
      }
    case 'p':
      {
        //Serial.println("PARAR");
        parar=1; 
        adiante=0;
        atras=0;
        xirar_a_dereita=0;
        xirar_a_esquerda=0;
        sigue_lineas=0;
        break;
      }
    case 'e':
      {
        // Serial.println("ESQUERDA");
        motores_ON=1;
        adiante=0;
        parar=0; 
        xirar_a_esquerda=1;
        xirar_a_dereita=0;
        atras=0;
        sigue_lineas=0;
        break;
      }
    case 'd':
      {
        // Serial.println("DEREITA");
        motores_ON=1;
        adiante=0;
        parar=0; 
        xirar_a_dereita=1;
        xirar_a_esquerda=0;
        atras=0;
        sigue_lineas=0;
        break;
      }
    case '+':
      {
        // Serial.print("+VEL");
        v_referencia = v_referencia+10;
        if(v_referencia>=175)
        {
          v_referencia=175; 
        }
        //Serial.println(v_referencia);
        break;
      } 
    case '-':
      {
        // Serial.print("-VEL");
        v_referencia = v_referencia-10;
        if(v_referencia<=0)
        {
          v_referencia=0; 
        }
        // Serial.println(v_referencia);
        break;
      }
    case 'c':
      {
        // Serial.println("ANTICHOQUE");
        antichoque=1;
        break;
      }
    case 'k':
      {
        // Serial.println("ANTICHOQUE");
        antichoque=0;
        break;
      }
    case 'z':
      {
        // Serial.println("AUTOMATICO");
        control_automatico=1;
        control_manual=0;
        motores_ON=0;
        adiante=0;
        parar=0; 
        xirar_a_dereita=0;
        xirar_a_esquerda=0;
        atras=0;
        sigue_lineas=0;
        break;
      }
    case 'm':
      {
        // Serial.println("MANUAL");
        control_automatico=0;
        control_manual=1;
        motores_ON=0;
        adiante=0;
        parar=0; 
        xirar_a_dereita=0;
        xirar_a_esquerda=0;
        atras=0;
        sigue_lineas=0;
        break;
      }
    case 'v':
      {
        rpm_enviar=(v_realD+v_realI)/2;
        Serial.println(rpm_enviar);//Cada vez que reciba v envia as rpm
        break;
      }
    case 'l':
      {
        // Serial.print("kp_sl = ");
        Kp_sl = Kp_sl+1; 
        //Serial.println(Kp_sl);
        break;
      }  
    case 'u':
      {
        //Serial.print("kp_sl = ");
        Kp_sl = Kp_sl-1; 
        //Serial.println(Kp_sl);
        break;  
      }
    case 'f':
      {
        // Serial.print("kd_sl = ");
        Kd_sl = Kd_sl+1; 
        //Serial.println(Kd_sl);
        break;
      }  
    case 'h':
      {
        //Serial.print("kd_sl = ");
        Kd_sl = Kd_sl-1; 
        //Serial.println(Kd_sl);
        break;  
      }
    case 'x':
      {
        //Serial.println("Autoaxuste");
        autoaxuste = 1;
        sigue_lineas=1;
        ulti_autoaxuste = millis();
        break;  
      }  
    } 
  }
}

int rpm_a_pwm(float rpm)//PASO DE RPM A PWM
{// En funcin do valor de rpm aplica unha ou outra formula para adaptarse a ecuacion achada no estudo do comportamento dos motores
  int pwm=0;
  if(rpm<1)
  {
    pwm=0; 
  }
  else
  {
    if(rpm<=101)
    {
      pwm=(rpm+202)/2.525;
    } 
    else
    {
      if(rpm<=151) 
      {
        pwm=(rpm-1.006)/0.8333; 
      }
      else
      {
        if(rpm<=155)
        {
          pwm=(rpm-115)/0.2;
        } 
        else
        {
          pwm=(rpm-75)/0.4; 
        }
      }
    }
  }
  return pwm;//Valor que devolve
}

float regulador(float v_real,float v_otra, float aceleracion)// REGULADOR empregado sempre menos no caso do modo SIGUE_LINEA
{
  //calculo de erros
  float error_v_referencia = v_referencia - v_real;
  float error_v_otra = v_otra - v_real;

  //calculo da saida
  float rpm=0;
  rpm= v_referencia + error_v_referencia * kp1 + error_v_otra * kp2 + aceleracion *2 ;//A aceleracion multiplicase *2 para anular o efecto de error_v_otra
  return rpm; 
}


float calculo_v(int contadorZ, int nZ, float tiempoZ) //CALCULO DA VELOCIDADE
{
  float v_real=0;
  //calculo da velocidade en rpm
  v_real=1000000*60*contadorZ/nZ;// voltas/micros
  v_real=v_real/(micros()-tiempoZ);

  return v_real;// Devolve a velocidade en rpm

}

void giro() //FUNCION QUE ENTREGA OS PULSOS A XIRAR EN FUNCION DO ANGULO DESEXADO
{
  pulsos_a_girar=((grados_a_girar*8.7*pi)/360)/(6*pi/24);
}

void I_interrupt() //rutina de interrupcion
{
  contadorI++;//constante utilizada para calcular a velocidade
  contadorI2++;//constante utilizada para ver si esta parado
  contadorI3++;
}

void D_interrupt() //rutina de interrupcion
{
  contadorD++;
  contadorD2++;
}
