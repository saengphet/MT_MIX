#define THROTTLEPIN 2
#define ROLLPIN 4
#define PITCHPIN 5
#define YAWPIN 6
#define AUXPIN 7

uint16_t throttleRAW = 1000;
uint16_t rollRAW = 1500;
uint16_t pitchRAW = 1500;
uint16_t yawRAW = 1500;
uint16_t auxRAW= 1500;

uint16_t throttle = 1000;
uint16_t roll = 1500;
uint16_t pitch = 1500;
uint16_t yaw = 1500;
uint16_t aux= 1500;

//----------------------------------------------------
//Channel
//int tarremote=0.025;
float CH_AILf = 1500;
float CH_ELEf = 1500;
float CH_RUDf = 1500;

uint16_t CH_AIL_Cal = 1500;
uint16_t CH_ELE_Cal = 1500;
uint16_t CH_RUD_Cal = 1500;

#define ROLL       0
#define PITCH      1
#define THROTTLE   2
#define AUX        4
#define YAW        3
      
static uint8_t pinRcChannel[5] = {ROLL, PITCH, THROTTLE, YAW, AUX};
volatile uint16_t rcPinValue[5] = {1500,1500,1000,1500,1500};
static int16_t rcData[5] ;
static int16_t rcHysteresis[5] ;
static int16_t rcData4Values[5][4];

//---------------------------------------------------
void TransmitterDefined () {
  pinMode(THROTTLEPIN, INPUT);
  pinMode(ROLLPIN, INPUT);
  pinMode(PITCHPIN, INPUT);
  pinMode(YAWPIN, INPUT);
  pinMode(AUXPIN, INPUT);
}


void CheckTransmitter()
{
  throttleRAW = pulseIn(THROTTLEPIN, HIGH, PULSETIMEOUT);
  rollRAW = pulseIn(ROLLPIN, HIGH, PULSETIMEOUT);
  pitchRAW = pulseIn(PITCHPIN, HIGH, PULSETIMEOUT); 
  yawRAW = pulseIn(YAWPIN, HIGH, PULSETIMEOUT); 
  auxRAW = pulseIn(AUXPIN, HIGH, PULSETIMEOUT);  
  
rcPinValue[0] =  rollRAW;
rcPinValue[1] =  pitchRAW;
rcPinValue[2] =  throttleRAW;
rcPinValue[3] = yawRAW;
rcPinValue[4] = auxRAW;
  
}
  //control_u();
  
  /*
  Serial.print (throttleC, DEC);
  Serial.print ("  ");
  
  Serial.print (rollC, DEC );
 Serial.print ("  ");
 
  Serial.print (pitchC, DEC );
   Serial.print ("  ");
 
  Serial.print (yawC, DEC );
   Serial.print ("  ");
 
  Serial.print (auxC, DEC );
   Serial.print ("  ");    
  
  Serial.println ("");
 */ 
 /*
Serial.print (motor_FR_Sig, DEC);
  Serial.print ("  ");
  
  Serial.print (motor_FL_Sig, DEC );
 Serial.print ("  ");
 
  Serial.print (motor_RR_Sig, DEC );
   Serial.print ("  ");
 
  Serial.print (motor_RL_Sig, DEC );
   Serial.print ("  ");

   Serial.print (armed, DEC );
   Serial.print ("  ");
   
  Serial.println ("");
   delay(200);
   */
/*   
//Channel
int tarremote=0.025;
float CH_AILf = 1500;
float CH_ELEf = 1500;
float CH_RUDf = 1500;

uint16_t CH_AIL_Cal = 1500;
uint16_t CH_ELE_Cal = 1500;
uint16_t CH_RUD_Cal = 1500;

#define ROLL       0
#define PITCH      1
#define THROTTLE   2
#define AUX        4
#define YAW        3
      
static uint8_t pinRcChannel[5] = {ROLL, PITCH, THROTTLE, YAW, AUX};
volatile uint16_t rcPinValue[5] = {1500,1500,1000,1500,1500};
static int16_t rcData[5] ;
static int16_t rcHysteresis[5] ;
static int16_t rcData4Values[5][4];
*/
 ///////////////////////////////////////////////////////////////////////////////////////////
void configureReceiver() {

    for (uint8_t chan = 0; chan < 5; chan++){
      for (uint8_t a = 0; a < 4; a++){
        rcData4Values[chan][a] = 1500;
      }  
    }    
}


  uint16_t readRawRC(uint8_t chan) {
  uint16_t data;
  data = rcPinValue[pinRcChannel[chan]];
    return data; 
}


void computeRC() {
  static uint8_t rc4ValuesIndex = 0;
  uint8_t chan,a;
  rc4ValuesIndex++;
  for (chan = 0; chan < 4; chan++) {
    rcData4Values[chan][rc4ValuesIndex%4] = readRawRC(chan);
    rcData[chan] = 0;
    for (a = 0; a < 4; a++){
      rcData[chan] += rcData4Values[chan][a];
    }
    rcData[chan]= (rcData[chan]+2)/4;
    if ( rcData[chan] < rcHysteresis[chan] -3)  rcHysteresis[chan] = rcData[chan]+2;
    if ( rcData[chan] > rcHysteresis[chan] +3)  rcHysteresis[chan] = rcData[chan]-2;
  }
    throttle = rcHysteresis[THROTTLE];
    roll = rcHysteresis[ROLL];
    pitch = rcHysteresis[PITCH];
    yaw = rcHysteresis[YAW];
    aux = rcHysteresis[AUX];
    
    CH_AILf = CH_AILf + (roll - CH_AILf)*0.02/tarremote;
    CH_ELEf = CH_ELEf + (pitch - CH_ELEf)*0.02/tarremote;
    CH_RUDf = CH_RUDf + (yaw - CH_RUDf)*0.02/tarremote;
}

void RC_Calibrate(){
  Serial.print("RC_Calibrate");Serial.println("\t");
  for (int i = 0; i < 10; i++) {
    computeRC();
    delay(20);
  }
   CH_AIL_Cal = roll;
   CH_ELE_Cal = pitch;
   CH_RUD_Cal = yaw;
    Serial.print(CH_AIL_Cal);Serial.print("\t");//-0.13
    Serial.print(CH_ELE_Cal);Serial.print("\t");//-0.10
    Serial.print(CH_RUD_Cal);Serial.println("\t");//0.03 
}

/*
   CH_AIL_Cal = roll;
   CH_ELE_Cal = pitch;
   CH_RUD_Cal = yaw;
  /*
    Serial.print(CH_AIL_Cal);Serial.print("\t");//-0.13
    Serial.print(CH_ELE_Cal);Serial.print("\t");//-0.10
    Serial.print(CH_RUD_Cal);Serial.println("\t");//0.03 
*/
 

