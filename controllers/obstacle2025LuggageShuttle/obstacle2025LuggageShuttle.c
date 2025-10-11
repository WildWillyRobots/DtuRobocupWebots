#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/position_sensor.h>

#define TIME_STEP 64

int main(int argc, char **argv)
{
  wb_robot_init();

  WbDeviceTag positionSensor = wb_robot_get_device("position sensor");
  wb_position_sensor_enable(positionSensor, TIME_STEP);

  WbDeviceTag motor0 = wb_robot_get_device("linear motor");
  wb_motor_set_position(motor0, INFINITY);
  wb_motor_set_velocity(motor0, 0.2);

  while (wb_robot_step(TIME_STEP) != -1)
  {

    double pos = wb_position_sensor_get_value(positionSensor);
    if (pos > 2.5) // Will move a bit further because of inertia
    {
      wb_motor_set_velocity(motor0, -0.2);
    }

    if (pos < 0.10) // Will move a bit further because of inertia
    {
      wb_motor_set_velocity(motor0, 0.2);
    }
  };

  wb_robot_cleanup();
  return 0;
}
