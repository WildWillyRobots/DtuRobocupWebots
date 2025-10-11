#include <webots/robot.h>
#include <webots/motor.h>

#define TIME_STEP 64

int main(int argc, char **argv)
{
  wb_robot_init();

  WbDeviceTag axe = wb_robot_get_device("motor_axe");
  wb_motor_set_position(axe, INFINITY);
  wb_motor_set_velocity(axe, 0.4);

  while (wb_robot_step(TIME_STEP) != -1)
  {
  };

  wb_robot_cleanup();

  return 0;
}
