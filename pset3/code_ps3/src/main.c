#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <client.h>
#include <unistd.h>
#include "simulator.h"
#include "vehicle.h"

// NOTE: See simulator.h for preprocessor constants that can turn the display on or off.

////
//// YOUR CODE HERE AS NEEDED
//// (such as helper functions or global variables)
////


int main(int argc, char *argv[])
{
    // Kill any lingering instances of the display process.
    system("pkill -f DisplayServer.py");

    #if USE_DISPLAY
    printf("\nLaunching the display server\n");
    // Launch the Python display server (in a separate process).
    launch_display_server();
    // Add a pause for python to start up.
    // If you get error messages about sockets,
    // a good start would be to increase this time.
    sleep(1);
    #endif

    // Create the simulator.
    printf("\nCreating the simulator");
    simulator* sim = create_simulator();
    // Simulator configuration.
    sim->n_vehicles = 3;
    sim->max_time_s = 100;        // default 100
    sim->time_increment_s = 0.05; // default 0.05

    // Create the waypoint data.
    // offset_waypoints specifies waypoints relative to a vehicle's starting position.
    int num_waypoints = 5;
    double radius = 30;
    double** offset_waypoints = malloc((num_waypoints)*sizeof(double*));
    for(int i = 0; i < num_waypoints + 1; i++)
    {
        offset_waypoints[i] = malloc(2*sizeof(double));
        offset_waypoints[i][0] = radius * cos( i * 2 * M_PI / (num_waypoints));
        offset_waypoints[i][1] = radius * sin( i * 2 * M_PI / (num_waypoints));
    }

    // Create 3 vehicles and add them to the simulator.
    // Each vehicle will have its own thread created.
    sim->n_vehicles = 3;
    sim->vehicles = malloc(sim->n_vehicles * sizeof(vehicle));
    pthread_t thread_ids[sim->n_vehicles];
    for(int i = 0; i < sim->n_vehicles; i++)
    {
        double initial_position[3] = {i*10+30, i*10+30, 0};
        vehicle* v = create_vehicle(initial_position, sim);
        // Create a copy so we can free the original vehicle later.
        sim->vehicles[i] = *v;
        free(v);
        ////
        //// YOUR CODE HERE
        //// Adjust the below code as needed for your control.
        ////
        sim->vehicles[i].get_control = get_proportional_waypoint_control;
        sim->vehicles[i].control_data = (void *) create_waypoint_control_data(num_waypoints, initial_position, offset_waypoints);
        sim->vehicles[i].control_type = "waypoint_control"; // set this to "waypoint_control" or "follower_control" according to what type of struct control_data is now
        ////
        //// YOUR CODE HERE
        //// Create threads using thread_ids[i].
        ////
    }

    // Run the main simulator.
    sim->run(sim);
    // Join each vehicle thread.
    for(int i = 0; i < sim->n_vehicles; i++)
      pthread_join(thread_ids[i], NULL);

    // Cleanup - will free memory used by vehicles and simulator.
    cleanup_simulator(sim);
    free(sim);

    #if USE_DISPLAY
    // Wait for the user to quit the program.
    printf("\nPress 'q' to quit the program\n");
    while(getchar() != 'q')  sleep(0.1);
    // Close the display server
    close_display_server();
    #endif

    printf("\n\n");
}
