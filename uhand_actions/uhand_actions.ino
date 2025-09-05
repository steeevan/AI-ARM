#include <Servo.h> //(import servo library)
#include "uhand_servo.h" //(import action group contorl library)

/* (define pins) */
const static uint8_t servoPins[6] = { 7, 6, 5, 4, 3, 2 };//(define servo pins)
int action_index=3;  //action（actions.h）(action number to be executed; for more details about relevant action data, view action array (actions.h))

//(action group control object)
HW_ACTION_CTL action_ctl;
//(servo control object)
Servo servos[6];

const uint8_t limt_angles[6][2] = {{0,82},{0,180},{0,180},{25,180},{0,180},{0,180}}; /* (angle limit for each joint) */
static float servo_angles[6] = { 0,0,0,0,0, 90 };  /* (the actual angle values controlled by servos) */

static void servo_control(void); /* (servo control) */
void user_task(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // (set the timeout for serial port reading data)
  Serial.setTimeout(500);
  
  // (bind servo IO pin)
  for (int i = 0; i < 6; ++i) {
    servos[i].attach(servoPins[i]);
  }

  delay(2000);
  Serial.println("start");
}

void loop() {
  // (user task)
  user_task();
  // Action group executing tasks
  action_ctl.action_task();
  //Servo control
  servo_control();
}

void user_task(void)
{
  static uint32_t last_tick = 0;
  if (millis() - last_tick < 100) {
    return;
  }
  last_tick = millis();

  static uint32_t step = 0;
  switch(step)
  {
    case 0:
      //Action control
      action_ctl.action_set(action_index);//Execute Action 
      Serial.print("action run.");
      step = 1;
      break;
    case 1:
      if(action_ctl.action_state_get() == 0)
      {
        Serial.println("");
        Serial.println("The action is running successfully!");
      }else{
        Serial.print(" .");
      }
      break;
  }
}

// (servo control task (no need to be modified))
void servo_control(void) {
  static uint32_t last_tick = 0;
  if (millis() - last_tick < 20) {
    return;
  }
  last_tick = millis();

  for (int i = 0; i < 6; ++i) {
    if(servo_angles[i] > action_ctl.extended_func_angles[i])
    {
      servo_angles[i] = servo_angles[i] * 0.9 + action_ctl.extended_func_angles[i] * 0.1;
      if(servo_angles[i] < action_ctl.extended_func_angles[i])
        servo_angles[i] = action_ctl.extended_func_angles[i];
    }else if(servo_angles[i] < action_ctl.extended_func_angles[i])
    {
      servo_angles[i] = servo_angles[i] * 0.9 + (action_ctl.extended_func_angles[i] * 0.1 + 1);
      if(servo_angles[i] > action_ctl.extended_func_angles[i])
        servo_angles[i] = action_ctl.extended_func_angles[i];
    }

    servo_angles[i] = servo_angles[i] < limt_angles[i][0] ? limt_angles[i][0] : servo_angles[i];
    servo_angles[i] = servo_angles[i] > limt_angles[i][1] ? limt_angles[i][1] : servo_angles[i];
    servos[i].write(i == 0 || i == 5 ? 180 - servo_angles[i] : servo_angles[i]);
  }
}
