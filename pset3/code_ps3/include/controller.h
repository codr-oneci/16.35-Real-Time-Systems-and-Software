#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

// Include files.
#include "vehicle.h"

// Define the control structure.
typedef struct t_control {
    double speed; // Note: units not included in provided code since not directly specified - if units are known, it's good to name it someting like speed_m_s
    double angular_velocity_rad_s; // rad/s
} control;

// Define a structure for storing target and current waypoints.
typedef struct {
    int num_waypoints;
    double** target_waypoints; // List of x, y points to drive to; each element is [x, y] coordinates.  Length is num_waypoints.
    double* current_waypoint;  // The current x, y point you're aiming for.
    int current_waypoint_idx;  // Index into target_waypoints to retrieve current_waypoint.
} waypoint_control_data;

// Define a structure for the follower control paradigm.
typedef struct {
    vehicle* leader;
} follower_control_data;

/// YOUR CODE HERE AS NEEDED

// Declare possible controller functions here.
// Each one should return a control structure given a vehicle;
// it should compute the desired speed and angular velocity of the vehicle for the next timestep.
control get_proportional_waypoint_control(struct t_vehicle* v);
control get_follower_control(struct t_vehicle* v);

// Declare functions for control data initialization.
waypoint_control_data* create_waypoint_control_data(int n_waypoints, double* starting_position, double** offset_waypoints);
follower_control_data* create_follower_control_data(vehicle* leader);

#endif // __CONTROLLER_H__
