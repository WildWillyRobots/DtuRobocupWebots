#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <webots/robot.h>
#include <webots/supervisor.h>
#include <webots/position_sensor.h>

#define TIME_STEP 64

#define NUM_BALLS 5
#define SQUARE_SIZE 0.7
#define SQUARE_OFFSET_X (SQUARE_SIZE / 2 + 0.1)
#define SQUARE_OFFSET_Y 0.0
#define MIN_DISTANCE 0.05 // 50mm minimum distance

int main(int argc, char **argv)
{
  wb_robot_init();

  WbDeviceTag position_sensor = wb_robot_get_device("position sensor");
  wb_position_sensor_enable(position_sensor, TIME_STEP);
  
  WbNodeRef proto_instance = wb_supervisor_node_get_self();

  WbFieldRef ball_translation[NUM_BALLS];
  char field_name[20];

  // Get all ball translation fields
  for (int i = 0; i < NUM_BALLS; i++)
  {
    sprintf(field_name, "ball%d_translation", i);
    ball_translation[i] = wb_supervisor_node_get_field(proto_instance, field_name);
  }

  // Initialize random seed
  srand(time(NULL));

  // Array to store ball positions for collision checking
  double ball_positions[NUM_BALLS][3];

  // Flag to track if positioning has been done
  bool positioning_done = false;

  while (wb_robot_step(TIME_STEP) != -1)
  {
    // Check if position sensor value > 2 and positioning not done yet
    if (wb_position_sensor_get_value(position_sensor) > 2.0 && !positioning_done)
    {
      // Randomly position all balls in a square with minimum 50mm distance
      for (int i = 0; i < NUM_BALLS; i++)
      {
        if (!ball_translation[i])
          continue;

        bool position_found = false;
        int attempts = 0;
        const int max_attempts = 1000;

        while (!position_found && attempts < max_attempts)
        {
          // Generate random position in square with offsets
          double x = ((double)rand() / RAND_MAX - 0.5) * SQUARE_SIZE + SQUARE_OFFSET_X;
          double y = ((double)rand() / RAND_MAX - 0.5) * SQUARE_SIZE + SQUARE_OFFSET_Y;
          double z = -0.085 + 0.021; // Keep z at ground level

          // Check distance from all previously placed balls
          bool valid_position = true;
          for (int j = 0; j < i; j++)
          {
            double dx = x - ball_positions[j][0];
            double dy = y - ball_positions[j][1];
            double distance = sqrt(dx * dx + dy * dy);

            if (distance < MIN_DISTANCE)
            {
              valid_position = false;
              break;
            }
          }

          if (valid_position)
          {
            // Store position
            ball_positions[i][0] = x;
            ball_positions[i][1] = y;
            ball_positions[i][2] = z;

            // Set ball position
            wb_supervisor_field_set_sf_vec3f(ball_translation[i], ball_positions[i]);

            printf("Ball %d positioned at [%.3f, %.3f, %.3f]\n", i, x, y, z);
            position_found = true;
          }

          attempts++;
        }

        if (!position_found)
        {
          printf("Warning: Could not find valid position for ball %d after %d attempts\n", i, max_attempts);
          // Use a fallback position with offsets
          ball_positions[i][0] = (i * 0.1) - 0.2 + SQUARE_OFFSET_X;
          ball_positions[i][1] = 0.0 + SQUARE_OFFSET_Y;
          ball_positions[i][2] = -0.085 + 0.021;
          wb_supervisor_field_set_sf_vec3f(ball_translation[i], ball_positions[i]);
        }
      }

      positioning_done = true;
      printf("Ball positioning completed!\n");
    }
  };

  wb_robot_cleanup();
  return 0;
}
