#include "uhand_servo.h"

void HW_ACTION_CTL::action_set(int num){
  action_num = num;
}

static int HW_ACTION_CTL::action_state_get(void){
  return action_num;
}

void HW_ACTION_CTL::action_task(void){
  static uint32_t last_tick = 0;
  static uint8_t step = 0;
  static uint8_t num = 0 , delay_count = 0;
  if(action_num != 0 && action_num <= action_count)
  {
    //(interval)
    if (millis() - last_tick < 100) {
      return;
    }
    last_tick = millis();
    
    switch(step)
    {
      case 0: //(run action)
        if(action[action_num-1][num][0] != 0)
        {
          extended_func_angles[0] = action[action_num-1][num][1];
          extended_func_angles[1] = action[action_num-1][num][2];
          extended_func_angles[2] = action[action_num-1][num][3];
          extended_func_angles[3] = action[action_num-1][num][4];
          extended_func_angles[4] = action[action_num-1][num][5];
          extended_func_angles[5] = action[action_num-1][num][6];

          step = 1;

        }else{ //(if the running is finished)
          num = 0;
          // (clear the action group variable)
          action_num = 0;
          
        }
        break;
      case 1: //(wait for action running)
        delay_count++;
        if(delay_count > 2)
        {
          num++;
          delay_count = 0;
          step = 0;
        }
        break;
      default:
        step = 0;
        break;
    }
  }
}