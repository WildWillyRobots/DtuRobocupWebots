#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>

#define TIME_STEP 64

int main(int argc, char **argv)
{
  wb_robot_init();

  WbDeviceTag motor = wb_robot_get_device("motor_barrier");
  wb_motor_set_position(motor, 0);

  WbDeviceTag distanceSensor = wb_robot_get_device("distance_sensor_start");
  wb_distance_sensor_enable(distanceSensor, TIME_STEP);

  double openTime = -1;
  bool wasAtStart = false;

  while (wb_robot_step(TIME_STEP) != -1)
  {
    bool atStart = wb_distance_sensor_get_value(distanceSensor) > 0.1;

    if (wasAtStart && !atStart)
    {
      // Start countdown
      openTime = wb_robot_get_time();
      wasAtStart = false;
    }

    if (!wasAtStart && atStart)
    {
      // Robot entered the start area: open the barrier
      wb_motor_set_position(motor, 1.57);
      wasAtStart = true;
      openTime = -1;
    }

    if (openTime > 0 && wb_robot_get_time() - openTime > 15)
    {
      // 15 seconds passed since the robot left the start area: close the barrier
      wb_motor_set_position(motor, 0);
    }
  };

  wb_robot_cleanup();

  return 0;
}
