#include "controller.h"
#include "vehicle.h"
#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

vehicle* create_vehicle(double* starting_position, struct t_simulator* sim)
{
    vehicle* v = malloc(sizeof(vehicle));

    // Initialize synchronization/threading variables.
    ////
    //// YOUR CODE HERE AS NEEDED
    ////

    // Assign function pointers to defaults.
    v->set_position = &set_position;
    v->set_velocity = &set_velocity;
    v->control_vehicle = &control_vehicle;
    v->update_state = &update_state;
    v->run = &vehicle_run;
    v->get_control = NULL;

    // Set the initial position.
    set_position(v, starting_position);
    // Store simulator and initial control data.
    v->simulator = sim;
    v->control_data = NULL;
    v->control_type = "";

    return v;
}

// Set the vehicle position.
// Position is specified as [x, y, theta]
void set_position(struct t_vehicle* v, double* values)
{
    ////
    //// YOUR CODE HERE AS NEEDED
    ////

    // Set the vehicle positions.
    // Assumes the vehicle is properly initialized.
    memcpy(v->position, values, 3*sizeof(double));
    // Enforce position bounds.
    if(v->position[0] > 100.0) v->position[0] = 100.0;
    if(v->position[1] > 100.0) v->position[1] = 100.0;
    if(v->position[2] > M_PI)  v->position[2] = M_PI;
    if(v->position[0] < 0.0)   v->position[0] = 0.0;
    if(v->position[1] < 0.0)   v->position[1] = 0.0;
    if(v->position[2] < -M_PI) v->position[2] = -M_PI;

    ////
    //// YOUR CODE HERE AS NEEDED
    ////
}

// Set the vehicle velocity.
// Velocity is specified as [vx, vy].
void set_velocity(struct t_vehicle* v, double* values)
{
    // Set the vehicle velocities.
    // Assumes the vehicle is properly initialized.
    memcpy(v->velocity, values, 3*sizeof(double));

    // Enforce linear speed bounds.
    // Split velocities by components if bounds are exceeded.
    double heading = v->position[2];
    if(sqrt(pow(v->velocity[0],2) + pow(v->velocity[1],2)) > 10.0)
    {
        v->velocity[0] = 10*cos(heading);
        v->velocity[1] = 10*sin(heading);
    }
    else if(sqrt(pow(v->velocity[0],2)+pow(v->velocity[1],2)) < 5.0)
    {
        v->velocity[0] = 5*cos(heading);
        v->velocity[1] = 5*sin(heading);
    }
    // Enforce angular velocity bounds.
    if(v->velocity[2] > M_PI_4)
      v->velocity[2] = M_PI_4;
    if(v->velocity[2] < -M_PI_4)
      v->velocity[2] = -M_PI_4;
}

// Control the vehicle
void control_vehicle(struct t_vehicle* v)
{
    // Calculate what control to apply.
    control control_values = v->get_control(v);

    // Apply controls to the vehicle.
    double velocities[3] = {
      control_values.speed*cos(v->position[2]), // vx
      control_values.speed*sin(v->position[2]), // vy
      control_values.angular_velocity_rad_s // vtheta
    };
    v->set_velocity(v, velocities);
}

// Update the vehicle state based on current controls and dynamics.
void update_state(struct t_vehicle* v, double time_increment_s)
{
    // Extract vehicle position and velocity.
    double dx = v->velocity[0];
    double dy = v->velocity[1];
    double dtheta = v->velocity[2];
    double x = v->position[0];
    double y = v->position[1];
    double theta = v->position[2];

    // Calculate new positions.
    x += dx*time_increment_s;
    y += dy*time_increment_s;
    theta += dtheta*time_increment_s;

    // Enforce bounds on range of theta.
    if(theta > M_PI)
      theta -= 2.0*M_PI;
    if(theta < -M_PI)
      theta += 2.0*M_PI;

    // Update positions (enforcing bounds).
    double new_position[3] = {x, y, theta};
    v->set_position(v, new_position);
}

// Run the vehicle simulation.
void vehicle_run(struct t_vehicle* v)
{
  ////
  //// YOUR CODE HERE
  ////
}
