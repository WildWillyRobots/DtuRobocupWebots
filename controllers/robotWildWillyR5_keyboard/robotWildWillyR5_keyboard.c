#include <stdio.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/keyboard.h>
#include <webots/camera.h>

#define TIME_STEP 64

int main(int argc, char **argv)
{
   wb_robot_init();

   WbDeviceTag wheel1_a, wheel1_w;
   WbDeviceTag wheel2_a, wheel2_w;
   WbDeviceTag wheel3_a, wheel3_w;

   wheel1_w = wb_robot_get_device("swerve_motor_angular_speed_0");
   wb_motor_set_position(wheel1_w, INFINITY);
   wb_motor_set_velocity(wheel1_w, 0);

   wheel2_w = wb_robot_get_device("swerve_motor_angular_speed_1");
   wb_motor_set_position(wheel2_w, INFINITY);
   wb_motor_set_velocity(wheel2_w, 0);

   wheel3_w = wb_robot_get_device("swerve_motor_angular_speed_2");
   wb_motor_set_position(wheel3_w, INFINITY);
   wb_motor_set_velocity(wheel3_w, 0);

   wheel1_a = wb_robot_get_device("swerve_motor_angle_0");
   wb_motor_set_position(wheel1_a, 0.0 / 180.0 * 3.14);
   wb_motor_set_velocity(wheel1_a, 7);

   wheel2_a = wb_robot_get_device("swerve_motor_angle_1");
   wb_motor_set_position(wheel2_a, 0.0 / 180.0 * 3.14);
   wb_motor_set_velocity(wheel2_a, 7);

   wheel3_a = wb_robot_get_device("swerve_motor_angle_2");
   wb_motor_set_position(wheel3_a, 0.0 / 180.0 * 3.14);
   wb_motor_set_velocity(wheel3_a, 7);

   WbDeviceTag servo = wb_robot_get_device("arm_motor_angle");
   wb_motor_set_position(servo, 0 / 180.0 * M_PI);
   wb_motor_set_velocity(servo, 10);

   WbDeviceTag camera = wb_robot_get_device("camera_rgb");
   wb_camera_enable(camera, TIME_STEP);

   wb_keyboard_enable(TIME_STEP);

   // Print keyboard commands
   printf("Keyboard commands:\n");
   printf("  Arrow keys - Move the robot\n");
   printf("  Keys 1-9   - Set speed\n");
   printf("  Key Q      - Robot-oriented control\n");
   printf("  Key W      - Field-oriented control\n");
   printf("  Key A      - Set arm to neutral position\n");
   printf("  Key S      - Set arm to up position\n");
   printf("  Key F      - Set arm to half-down position\n");
   printf("  Key D      - Set arm to down position\n");

   double speed[3];
   double angle[3];
   int keys = 0;
   const int k_up = 0x01;
   const int k_down = 0x02;
   const int k_left = 0x04;
   const int k_right = 0x08;
   double fwd = 10;
   int mode = 0;
   while (wb_robot_step(TIME_STEP) != -1)
   {
      keys = 0;
      int key = wb_keyboard_get_key();
      while (key != -1)
      {
         if (key == WB_KEYBOARD_UP)
         {
            keys |= k_up;
         }
         if (key == WB_KEYBOARD_DOWN)
         {
            keys |= k_down;
         }
         if (key == WB_KEYBOARD_LEFT)
         {
            keys |= k_left;
         }
         if (key == WB_KEYBOARD_RIGHT)
         {
            keys |= k_right;
         }
         if (key == '1')
            fwd = 2;
         if (key == '2')
            fwd = 4;
         if (key == '3')
            fwd = 6;
         if (key == '4')
            fwd = 8;
         if (key == '5')
            fwd = 10;
         if (key == '6')
            fwd = 12;
         if (key == '7')
            fwd = 14;
         if (key == '8')
            fwd = 16;
         if (key == '9')
            fwd = 18;
         if (key == 'Q')
            mode = 0;
         if (key == 'W')
            mode = 1;
         if (key == 'A')
            wb_motor_set_position(servo, 0.0 / 180.0 * M_PI); // neutral
         if (key == 'S')
            wb_motor_set_position(servo, 85.0 / 180.0 * M_PI); // up
         if (key == 'D')
            wb_motor_set_position(servo, 120.0 / 180.0 * M_PI); // half-down
         if (key == 'F')
            wb_motor_set_position(servo, 175.0 / 180.0 * M_PI); // down

         key = wb_keyboard_get_key();
      }

      speed[0] = 0;
      speed[1] = 0;
      speed[2] = 0;
      angle[0] = 0;
      angle[1] = 0;
      angle[2] = 0;
      if (mode == 1)
      {
         switch (keys)
         {
         case k_up | k_left:
            angle[0] = -45;
            angle[1] = -45;
            angle[2] = -45;
            speed[0] = fwd;
            speed[1] = fwd;
            speed[2] = fwd;
            break;
         case k_up:
            angle[0] = 0;
            angle[1] = 0;
            angle[2] = 0;
            speed[0] = fwd;
            speed[1] = fwd;
            speed[2] = fwd;
            break;
         case k_up | k_right:
            angle[0] = 45;
            angle[1] = 45;
            angle[2] = 45;
            speed[0] = fwd;
            speed[1] = fwd;
            speed[2] = fwd;
            break;
         case k_right:
            angle[0] = 90;
            angle[1] = 90;
            angle[2] = 90;
            speed[0] = fwd;
            speed[1] = fwd;
            speed[2] = fwd;
            break;
         case k_right | k_down:
            angle[0] = -45;
            angle[1] = -45;
            angle[2] = -45;
            speed[0] = -fwd;
            speed[1] = -fwd;
            speed[2] = -fwd;
            break;
         case k_down:
            angle[0] = 0;
            angle[1] = 0;
            angle[2] = 0;
            speed[0] = -fwd;
            speed[1] = -fwd;
            speed[2] = -fwd;
            break;
         case k_down | k_left:
            angle[0] = 45;
            angle[1] = 45;
            angle[2] = 45;
            speed[0] = -fwd;
            speed[1] = -fwd;
            speed[2] = -fwd;
            break;
         case k_left:
            angle[0] = -90;
            angle[1] = -90;
            angle[2] = -90;
            speed[0] = fwd;
            speed[1] = fwd;
            speed[2] = fwd;
            break;
         }
      }

      if (mode == 0)
      {
         switch (keys)
         {
         case k_up | k_left:
            angle[0] = -23;
            angle[1] = -9;
            angle[2] = 26;
            speed[0] = 61.0 / 100.0 * fwd;
            speed[1] = 145.0 / 100.0 * fwd;
            speed[2] = 111.0 / 100.0 * fwd;
            break;
         case k_up:
            angle[0] = 0;
            angle[1] = 0;
            angle[2] = 0;
            speed[0] = 100.0 / 100.0 * fwd;
            speed[1] = 100.0 / 100.0 * fwd;
            speed[2] = 100.0 / 100.0 * fwd;
            break;
         case k_up | k_right:
            angle[0] = 10;
            angle[1] = 24;
            angle[2] = -27;
            speed[0] = 145.0 / 100.0 * fwd;
            speed[1] = 61.0 / 100.0 * fwd;
            speed[2] = 111.0 / 100.0 * fwd;
            break;
         case k_right:
            angle[0] = 30;
            angle[1] = -29;
            angle[2] = 90;
            speed[0] = 50.0 / 100.0 * fwd;
            speed[1] = -50.0 / 100.0 * fwd;
            speed[2] = -49.0 / 100.0 * fwd;
            break;
         case k_right | k_down:
            angle[0] = 10;
            angle[1] = 24;
            angle[2] = -27;
            speed[0] = -145.0 / 100.0 * fwd;
            speed[1] = -61.0 / 100.0 * fwd;
            speed[2] = -111.0 / 100.0 * fwd;
            break;
         case k_down:
            angle[0] = 0;
            angle[1] = 0;
            angle[2] = 0;
            speed[0] = -100.0 / 100.0 * fwd;
            speed[1] = -100.0 / 100.0 * fwd;
            speed[2] = -100.0 / 100.0 * fwd;
            break;
         case k_down | k_left:
            angle[0] = -23;
            angle[1] = -9;
            angle[2] = 26;
            speed[0] = -61.0 / 100.0 * fwd;
            speed[1] = -145.0 / 100.0 * fwd;
            speed[2] = -111.0 / 100.0 * fwd;
            break;
         case k_left:
            angle[0] = 30;
            angle[1] = -29;
            angle[2] = 90;
            speed[0] = -50.0 / 100.0 * fwd;
            speed[1] = 50.0 / 100.0 * fwd;
            speed[2] = 49.0 / 100.0 * fwd;
            break;
         }
      }

      wb_motor_set_velocity(wheel1_w, speed[0]);
      wb_motor_set_velocity(wheel2_w, speed[1]);
      wb_motor_set_velocity(wheel3_w, speed[2]);

      wb_motor_set_position(wheel1_a, angle[0] / 180.0 * 3.14);
      wb_motor_set_position(wheel2_a, angle[1] / 180.0 * 3.14);
      wb_motor_set_position(wheel3_a, angle[2] / 180.0 * 3.14);
   };

   wb_robot_cleanup();
   return 0;
}
