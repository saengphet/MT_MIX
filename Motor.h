#define MOTOR_FRPIN 10
#define MOTOR_RRPIN 9
#define MOTOR_FLPIN 3
#define MOTOR_RLPIN 11
//-------------------------------------------------------------------//
void motor_command_all() {
  for (int j = 0 ; j <= 50 ; j++)
  {
  analogWrite(MOTOR_FRPIN, 125);
  analogWrite(MOTOR_RRPIN, 125);
  analogWrite(MOTOR_FLPIN,125);
  analogWrite(MOTOR_RLPIN,125);
  
  delay(20);
}
}
//-------------------------------------------------------------------//
void motor_initialize() 
{
 pinMode(MOTOR_FRPIN,OUTPUT);
 pinMode(MOTOR_RRPIN,OUTPUT);
 pinMode(MOTOR_FLPIN,OUTPUT);
 pinMode(MOTOR_RLPIN,OUTPUT);  
  motor_command_all();
}
//-------------------------------------------------------------------//

void motor_equal(){
/*  
if ((abs(motor_FR_Sig-motor_RL_Sig)<10) && (abs(motor_FL_Sig-motor_RR_Sig)<10)){
  motor_FR_Sig=motor_RR_Sig;
  motor_FL_Sig=motor_RR_Sig;
  motor_RL_Sig=motor_RR_Sig;
 }
*/
 
 if ( (motor_FR_Sig && motor_FL_Sig) < (motor_RL_Sig&&motor_RR_Sig)) {
  motor_FR_Sig=motor_FL_Sig;
  motor_RR_Sig=motor_RL_Sig;
  }
  
else if ((motor_FR_Sig && motor_FL_Sig) > (motor_RL_Sig&&motor_RR_Sig)){
  motor_FR_Sig=motor_FL_Sig;
  motor_RR_Sig=motor_RL_Sig;
}

else if ((motor_FR_Sig && motor_RR_Sig) < (motor_FL_Sig&&motor_RL_Sig)){
  motor_FR_Sig=motor_RR_Sig;
  motor_FL_Sig=motor_RL_Sig;
}

else if ((motor_FR_Sig && motor_RR_Sig) > (motor_FL_Sig&&motor_RL_Sig)){
  motor_FR_Sig=motor_RR_Sig;
  motor_FL_Sig=motor_RL_Sig;
}
}
//-------------------------------------------------------------------//
void motor_command() { 

  if (throttle<1050) {
         motor_FL_Sig = 1000;
         motor_FR_Sig = 1000;
         motor_RL_Sig = 1000;
         motor_RR_Sig = 1000;
    
  }
         
 analogWrite(MOTOR_FRPIN,(motor_FR_Sig/8)); //No.1
 analogWrite(MOTOR_RLPIN,(motor_RL_Sig/8)); //No.2
 analogWrite(MOTOR_FLPIN,(motor_FL_Sig/8)); //No.3
 analogWrite(MOTOR_RRPIN,(motor_RR_Sig/8)); //No.4

  delay(10); 
}  //end void motor_command() 
//-------------------------------------------------------------------//
void motor_mixed() 
{ 
       motor_FR_Sig =  throttleC + pitchC - rollC + yawC ;//Front R //No.1
       motor_RL_Sig = throttleC - pitchC +rollC +  yawC  ;////Back L //No.2     
       motor_FL_Sig = throttleC + pitchC + rollC - yawC  ;//Front L //No.3  , cos45 = 0.7071
       motor_RR_Sig = throttleC - pitchC - rollC - yawC  ;////Back R //No.4
}

 /********************************************************************/
 /****           ESCs calibration                                 ****/
 /********************************************************************/
void ESC_calibration () {
  int ESC_calibra=1;
delay(1500);
    for (int i = 0; i < 5; i++)
  {
  //CheckTransmitter();
  computeRC();
  
  if(throttle > 1600)
  {
    ESC_calibra = 1; 
  }
  else
  {
    ESC_calibra = 0;
  }
  delay(20);
  }
  
  while(ESC_calibra == 1){
     //CheckTransmitter();
  computeRC();
   
   motor_FR_Sig = (throttle - 500)*1.5;
   motor_RL_Sig = (throttle - 500)*1.5;
   motor_FL_Sig = (throttle - 500)*1.5;
   motor_RR_Sig = (throttle - 500)*1.5;
   
   motor_FR_Sig =  constrain(motor_FR_Sig,MINTHROTTLE,MAXTHROTTLE);
   motor_RL_Sig =  constrain(motor_RL_Sig,MINTHROTTLE,MAXTHROTTLE);   
   motor_FL_Sig =  constrain(motor_FL_Sig,MINTHROTTLE,MAXTHROTTLE);
   motor_RR_Sig =  constrain(motor_RR_Sig,MINTHROTTLE,MAXTHROTTLE);
   
    motor_FR_Sig =  constrain(motor_FR_Sig ,MINTHROTTLE,MAXTHROTTLE);
   motor_RL_Sig =  constrain(motor_RL_Sig ,MINTHROTTLE,MAXTHROTTLE);   
   motor_FL_Sig =  constrain(motor_FL_Sig ,MINTHROTTLE,MAXTHROTTLE);
   motor_RR_Sig =  constrain(motor_RR_Sig ,MINTHROTTLE,MAXTHROTTLE);
   
 analogWrite(MOTOR_FRPIN,(motor_FR_Sig/8)); //No.1
 analogWrite(MOTOR_RLPIN,(motor_RL_Sig/8)); //No.2
 analogWrite(MOTOR_FLPIN,(motor_FL_Sig/8)); //No.3
 analogWrite(MOTOR_RRPIN,(motor_RR_Sig/8)); //No.4
   
   Serial.print( motor_FR_Sig,DEC);Serial.print("\t");
   Serial.print( motor_FL_Sig,DEC);Serial.print("\t");    
   Serial.print( motor_RR_Sig,DEC);Serial.print("\t");     
   Serial.print( motor_RL_Sig,DEC);Serial.print("\t");
   Serial.print( ESC_calibra,DEC);Serial.println("\t");
   //Serial.print( ESC_calibra,DEC);Serial.println("\t");
   //digitalWrite(13, LOW);
   //delay(50);
   digitalWrite(13, HIGH);
   delay(50);
   
  if (throttle < 1100) {
    ESC_calibra = 0;
    digitalWrite(13, LOW);
  }
    else {
    ESC_calibra = 1;
    digitalWrite(13, LOW);
    }
     
  }
   Serial.print( ESC_calibra,DEC);Serial.println("\t");
}
