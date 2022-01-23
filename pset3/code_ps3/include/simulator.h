#ifndef __SIMULATOR_H__
#define __SIMULATOR_H__

// Include files.
#include "controller.h"
#include "vehicle.h"
#include <pthread.h>

// Configure some of the display/printing output options.
#define USE_DISPLAY 1           // Set to 0 to disable the graphical display.
#define PRINT_SIMULATION_TIME 1 // Whether to print the current time (which will overwrite text on the current terminal line).

// Define the simulator structure.
typedef struct t_simulator {
    // Simulator variables
    int       n_vehicles;
    vehicle*  vehicles;
    double    current_time_s;
    double    max_time_s;
    double    time_increment_s;
    double    vehicle_update_rate_hz;
    double    print_update_rate_hz;

    // Synchronization/threading variables.
    int vehicles_updated; // condition is vehicles_updated == n_vehicles.
    ////
    //// YOUR CODE HERE
    //// NOTE: Do not edit vehicles_updated declared above or n_vehicles declared below.
    ////

    // Simulator run function
    void (*run)(struct t_simulator*);
} simulator;

// Declare functions to initialize and run a simulator.
simulator* create_simulator();
void run(struct t_simulator* sim);
void cleanup_simulator(struct t_simulator* sim);

#endif // __SIMULATOR_H__
