#ifndef __VEHICLE_H__
#define __VEHICLE_H__

// Include files.
//// YOUR CODE HERE AS NEEDED

// Forward declare simulator and control structures so they can be visible during compilation.
struct t_simulator;
struct t_control;

// Define the vehicle structure.
typedef struct t_vehicle {
    ////
    //// YOUR CODE HERE AS NEEDED
    ////

    // Vehicle state.
    double position[3]; // x, y, theta
    double velocity[3]; // dx, dy, dtheta

    // Simulator and controller.
    struct t_simulator* simulator;
    void* control_data; // for use by relevant controller
    char* control_type; // indicates what controller is being used

    // Vehicle function calls
    void (*set_position)   (struct t_vehicle* v, double* values); // sets the vehicle position (after applying constraints)
    void (*set_velocity)   (struct t_vehicle* v, double* values); // sets the vehicle velocities (after applying constraints)
    void (*control_vehicle)(struct t_vehicle* v);                 // calculates and applies new vehicle control velocities
    void (*update_state)   (struct t_vehicle* v, double time_increment_s);    // updates vehicle states based on the dynamics (i.e. current velocity and position)
    struct t_control (*get_control)(struct t_vehicle* v);         // applies appropriate control rule
    void (*run) (struct t_vehicle* v);                            // runs the vehicle update loop; will use vehicle_run()
} vehicle;

// Declare vehicle creation function.
vehicle* create_vehicle(double* starting_position, struct t_simulator* sim);

// Declare standard vehicle methods (pointers to which are in the vehicle structure).
void set_position   (struct t_vehicle* v, double* values);
void set_velocity   (struct t_vehicle* v, double* values);
void control_vehicle(struct t_vehicle* v);
void update_state   (struct t_vehicle* v, double time);
void vehicle_run(struct t_vehicle* v); // will be used by run()

/// YOUR CODE HERE
// Declare any additional functions that you may want.

#endif // __VEHICLE_H__
