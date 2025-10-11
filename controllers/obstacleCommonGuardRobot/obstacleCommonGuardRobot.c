#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>

#define TIME_STEP 64

int main(int argc, char **argv)
{
  wb_robot_init();

  WbDeviceTag motor0 = wb_robot_get_device("motor0");
  wb_motor_set_position(motor0, INFINITY);
  wb_motor_set_velocity(motor0, 0);

  WbDeviceTag motor1 = wb_robot_get_device("motor1");
  wb_motor_set_position(motor1, INFINITY);
  wb_motor_set_velocity(motor1, 0);

  WbDeviceTag sensors[4];
  char sensor_names[4][8] = {
      "sensor0",
      "sensor1",
      "sensor2",
      "sensor3"};

  int i;
  for (i = 0; i < 4; i++)
  {
    sensors[i] = wb_robot_get_device(sensor_names[i]);
    wb_distance_sensor_enable(sensors[i], TIME_STEP);
  }

  float speed = 6;
  while (wb_robot_step(TIME_STEP) != -1)
  {
    for (i = 0; i < 4; i++)
    {
      double value = wb_distance_sensor_get_value(sensors[i]);

      // Naive line following algorithm. Find first sensor that sees the line and adjust the speed of the wheels accordingly.
      if (value < 700)
      {
        float reg = (i - 1) * 1.5;
        wb_motor_set_velocity(motor0, speed - reg);
        wb_motor_set_velocity(motor1, speed + reg);

        break;
      }
    }
  };

  wb_robot_cleanup();
  return 0;
}