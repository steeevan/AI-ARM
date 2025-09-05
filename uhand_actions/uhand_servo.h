#ifndef _HW_ACTION_CTL_
#define _HW_ACTION_CTL_
#include "actions.h"



class HW_ACTION_CTL{
  public:
    uint8_t extended_func_angles[6] = { 0,0,0,0,0, 90 }; /* (angle values used for secondary development) */
    //(control to execute action group)
    void action_set(int num);
    int action_state_get(void);
    void action_task(void);
    
  private:
    //(action group control variable)
    int action_num = 0;
};

#endif //_HW_ACTION_CTL_
