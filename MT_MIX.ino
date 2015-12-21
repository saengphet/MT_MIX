
#define MINTHROTTLE 1000 
#define MAXTHROTTLE 1900

#define MINCHK 1030 
#define MAXCHK 1870

#define BAUD 9600
#define PULSETIMEOUT 200000

int armed=0; 
int throttleC = 0;
int rollC = 0;
int pitchC = 0;
int yawC = 0;
int auxC= 0;

int motor_FL_Sig = 125;
int motor_FR_Sig = 125;
int motor_RL_Sig = 125;
int motor_RR_Sig = 125;



/*z
float motor_FL_Sig = 1000;
float motor_FR_Sig = 1000;
float motor_RL_Sig = 1000;
float motor_RR_Sig = 1000;
*/
#include "config.h"
#include "Transmitter.h"
#include "Motor.h"

void setup()
{
  Serial.begin(BAUD);
  
  TransmitterDefined();
  delay(20);
  configureReceiver();
  delay(20);
  motor_initialize();
  delay(20);
  ESC_calibration ();
  delay(20);
  RC_Calibrate();//"multi_rxPPM2560.h"
  //Serial.print("TK_Quadrotor_Run_Roop_100Hz");Serial.println("\t");
  float sensorPreviousTime = micros();
  float previousTime = micros();
  
 
 }

void control_u () {
throttleC = throttle;
rollC =  ((roll-1490)*0.25) ;
pitchC = ((pitch-1490)*0.25);
yawC =   ((yaw-1490)*0.25);
}

void Arm_Con () {
                         
 if (throttle < MINCHK)  {
 
    if (yaw > MAXCHK && armed == 0) {
                armed = 1;
                digitalWrite(13, HIGH);
                        }
    if (yaw  < MINCHK && armed == 1) {
                armed = 0;
                digitalWrite(13, LOW);
                        }
   }
  if(armed == 1) {
           motor_mixed();
           motor_equal();
         motor_FR_Sig =  constrain(motor_FR_Sig,MINTHROTTLE,MAXTHROTTLE);
         motor_RL_Sig =  constrain(motor_RL_Sig,MINTHROTTLE,MAXTHROTTLE);   
         motor_FL_Sig =  constrain(motor_FL_Sig,MINTHROTTLE,MAXTHROTTLE);
         motor_RR_Sig =  constrain(motor_RR_Sig,MINTHROTTLE,MAXTHROTTLE);
         motor_command();
                      }
        else  {
         motor_FL_Sig = 1000;
         motor_FR_Sig = 1000;
         motor_RL_Sig = 1000;
         motor_RR_Sig = 1000;
         motor_command();
        }
  
  }//end void Arm_Con ()

void loop() {

  CheckTransmitter();
  control_u();
  //motor_mixed();
  //motor_equal();
  Arm_Con();
  //motor_command();
 //end void loop
}
