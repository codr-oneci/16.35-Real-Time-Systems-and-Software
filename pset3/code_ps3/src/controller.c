#include "controller.h"
#include "vehicle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Helper function to get the angle from the vehicle's position to a target waypoint.
// Will return angle in radians, between -pi and pi.
double get_angle_to_point(struct t_vehicle* v, double targ_x, double targ_y)
{
    double angle_to_point_rad = atan2((targ_y - v->position[1]), (targ_x - v->position[0]));
    double angle_error_rad = angle_to_point_rad -  v->position[2];
    if(angle_error_rad > M_PI) {
        angle_error_rad = angle_error_rad - 2*M_PI;
    } else if(angle_error_rad < -1*M_PI) {
        angle_error_rad = 2*M_PI + angle_error_rad;
    }
    return angle_error_rad;
}

// Get control values using a proportional controller.
control get_proportional_waypoint_control(struct t_vehicle* v)
{
    // Get the current waypoint.
    waypoint_control_data* waypoint_data = (waypoint_control_data*) v->control_data;
    double* current_waypoint = waypoint_data->current_waypoint;

    // Compute the angular velocity control.
    double angle_control_input = get_angle_to_point(v, current_waypoint[0], current_waypoint[1]);
    // Cap the max angular velocity.
    if(angle_control_input > M_PI_4)
        angle_control_input = M_PI_4;
    else if(angle_control_input < -1*M_PI_4)
        angle_control_input = -1*M_PI_4;

    // Create control structure to return.
    control control_values = {5.0, angle_control_input}; // {speed, angular velocity}

    // Update waypoint logic - advance to next waypoint when current target is reached.
    // Note that waypoint_data is a pointer, so updating the values here will
    // update the values used by the vehicle as well.
    double waypoint_proximity_threshold = 1;
    double x_error = waypoint_data->current_waypoint[0] - v->position[0];
    double y_error = waypoint_data->current_waypoint[1] - v->position[1];
    if(sqrt(x_error*x_error + y_error*y_error) < waypoint_proximity_threshold)
    {
        int next_waypoint_idx = waypoint_data->current_waypoint_idx + 1;
        next_waypoint_idx %= waypoint_data->num_waypoints; // Wrap around (aim for first waypoint after reaching the final one)
        waypoint_data->current_waypoint_idx = next_waypoint_idx;
        waypoint_data->current_waypoint = waypoint_data->target_waypoints[next_waypoint_idx];
    }

    // Return the computed control.
    return control_values;
}

// Get control values using a follow controller.
control get_follower_control(struct t_vehicle* v)
{
    ////
    //// YOUR CODE HERE
    ////
    control control_values = {0.0, 0.0};  // {speed, angular velocity}
    return control_values;
}


// Initialization for waypoint control data.
waypoint_control_data* create_waypoint_control_data(int n_waypoints, double* starting_position, double** offset_waypoints)
{
    waypoint_control_data* waypoint_data = malloc(sizeof(waypoint_control_data));
    waypoint_data->target_waypoints = malloc(n_waypoints*sizeof(double*));
    for (int i = 0; i < n_waypoints; i++) {
        waypoint_data->target_waypoints[i] = malloc(2*sizeof(double));
        waypoint_data->target_waypoints[i][0] = starting_position[0] + offset_waypoints[i][0];
        waypoint_data->target_waypoints[i][1] = starting_position[1] + offset_waypoints[i][1];
    }
    waypoint_data->num_waypoints = n_waypoints;
    waypoint_data->current_waypoint_idx = 0;
    waypoint_data->current_waypoint = waypoint_data->target_waypoints[waypoint_data->current_waypoint_idx];
    return waypoint_data;
}

// Initialization for follower control data.
follower_control_data* create_follower_control_data(vehicle* leader)
{
    ////
    //// YOUR CODE HERE
    ////
    return NULL;
}
