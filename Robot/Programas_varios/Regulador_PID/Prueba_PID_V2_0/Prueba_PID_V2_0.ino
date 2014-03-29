/*
Alexandre Filgueira Lago --> http://botybot.wordpress.com/
*-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-**-*-*-*-*-*
*/

unsigned log lasTime;
double Input, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;
int SampleTime = 1000;// Tiempo para ejecutar PID

void Compute()
{
  unsigned long now = millis();
  int timeChange = (now - lasTime);
//Ver si ejecutar PID o no
  if(timeChange >= SampleTime)
{
  //Calculo de variables
  double error = Setpoint - Input;
  errSum = errSum + error;
  double dErr = (error - lastErr);
  //calculo funcion salida PID
  Output = kp*error + ki *errSum + kd * dErr;
  
 //Guardo variables
 lastErr = error;
 lasTime = now;
}
}

void SetTunings(double kp, double ki, double kd)
{
 double SampleTimeInSec = ((double)SampleTime)/1000;
kp=kp;
ki=ki*SampleTimeInSec;
kd = kd / SampleTimeInSec;
}

void SetSampleTime (int NewSampleTime)
{
 if(NewSampleTime > 0)
 {
  double ratio = (double)NewSampleTime / (double)SampleTime;
  ki*=ratio;
  kd/=ratio;
  SampleTime = (unsigned long) NewSampleTime;
 } 
}
