#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/touch_sensor.h>
#include <webots/led.h>

#define TIME_STEP 64

int main(int argc, char **argv)
{
  wb_robot_init();

  WbDeviceTag motor = wb_robot_get_device("motor_goal");
  wb_motor_set_position(motor, INFINITY);
  wb_motor_set_velocity(motor, 0);

  WbDeviceTag touch = wb_robot_get_device("touchsensor_goal");
  wb_touch_sensor_enable(touch, TIME_STEP);

  WbDeviceTag led = wb_robot_get_device("led_goal");
  wb_led_set(led, 0);

  double time = -1;
  while (wb_robot_step(TIME_STEP) != -1)
  {
    double touched = wb_touch_sensor_get_value(touch);

    if (touched > 0.5 && time < 0)
    {
      // Goal touched: start rotation and turn on LED
      wb_motor_set_velocity(motor, 6);
      wb_led_set(led, 1);
      time = wb_robot_get_time();
    }

    if (wb_robot_get_time() - time > 3.5)
    {
      // After 3.5 seconds, stop the rotation
      wb_motor_set_velocity(motor, 0);
    }

    if (wb_robot_get_time() - time > 4)
    {
      // After 4 seconds, turn off the LED and reset if the goal is no longer touched
      wb_led_set(led, 0);
      if (touched < 0.5)
      {
        time = -1;
      }
    }
  };

  wb_robot_cleanup();
  return 0;
}
