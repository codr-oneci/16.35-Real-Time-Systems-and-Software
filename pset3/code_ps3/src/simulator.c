#include "simulator.h"
#include "vehicle.h"
#include <stdlib.h>
#include "client.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// Initialize a simulator.
simulator* create_simulator()
{
    // Initialization.
    simulator* sim = malloc(sizeof(simulator));
    sim->run = &run;
    sim->n_vehicles = 0;
    sim->vehicles = NULL;

    // Simulator settings.
    sim->vehicle_update_rate_hz = 15; // Hz
    sim->print_update_rate_hz = 10; // Hz
    sim->max_time_s = 100.0;
    sim->current_time_s = 0.0;
    sim->time_increment_s = 0.05;

    // Initialize synchronization/threading variables.
    sim->vehicles_updated = 0;
    ////
    //// YOUR CODE HERE
    //// NOTE: Do not edit vehicles_updated or n_vehicles defined above.
    ////

    // Return.
    return sim;
}

// Run a simulator.
void run(struct t_simulator* sim)
{
    #if USE_DISPLAY
    // Start the display.
    printf("\nOpening the display server\n");
    open_server(IP, PORTNUM);
    sleep(2);
    // Send the vehicle waypoints.
    printf("\n");
    for(int vehicle_index = 0; vehicle_index < sim->n_vehicles; vehicle_index++)
    {
      if(strcmp(sim->vehicles[vehicle_index].control_type, "waypoint_control") == 0)
      {
        printf("\nSending waypoint data to the display for vehicle %d/%d", vehicle_index+1, sim->n_vehicles);
        fflush(stdout); // force printf statements to be printed to the screen immediately
        waypoint_control_data* waypoint_data = (waypoint_control_data*) sim->vehicles[vehicle_index].control_data;
        send_vehicle_waypoints(vehicle_index, waypoint_data->num_waypoints, waypoint_data->target_waypoints);
      }
      else
      {
        printf("\nNot sending waypoint data to the display for vehicle %d/%d since its control does not store waypoints", vehicle_index+1, sim->n_vehicles);
        fflush(stdout); // force printf statements to be printed to the screen immediately
      }
    }
    printf("\n");
    #endif

    // Step through the simulation.
    printf("\nStarting the simulation");
    #if USE_DISPLAY
    printf("\nPress 'q' to quit the program");
    #endif
    printf("\n\n");
    sim->current_time_s = 0.0;
    double time_since_last_vehicle_message_s = 0.0;
    double time_since_last_print_update_s = 0.0;
    while(sim->current_time_s < sim->max_time_s)
    {
        // Wait on condition variable here to know when the current timestep completes.
        ////
        //// YOUR CODE HERE
        ////

        // Increment the simulation time.
        sim->current_time_s += sim->time_increment_s;

        // Before waking the other threads,
        // update the display server and printouts if required.
        time_since_last_vehicle_message_s += sim->time_increment_s;
        time_since_last_print_update_s += sim->time_increment_s;
        if(time_since_last_vehicle_message_s > 1.0/sim->vehicle_update_rate_hz)
        {
            #if USE_DISPLAY
            send_vehicles(sim->n_vehicles, sim->vehicles);
            #endif
            time_since_last_vehicle_message_s = 0.0;
        }
        if(time_since_last_print_update_s > 1.0/sim->print_update_rate_hz)
        {
            #if PRINT_SIMULATION_TIME
            // Note that \r is a carriage return to send the cursor to the beginning of the line,
            // so the following print statement will overwrite text currently printed on that line of the terminal.
            printf("\rt = %7.3f", sim->current_time_s);
            fflush(stdout); // force printf statements to be printed to the screen immediately
            #endif
            time_since_last_print_update_s = 0.0;
        }

        // We're past the condition (everyone is updated)
        // so we can now broadcast to the waiting threads and wake them up.
        ////
        //// YOUR CODE HERE
        ////

        // Sleep for roughly the time increment so we get quasi-realtime behavior.
        usleep(sim->time_increment_s*1e6);
    }

    // Close the display process.
    #if USE_DISPLAY
    close_server();
    #endif
}

// Clean up when simulator is finished.
void cleanup_simulator(struct t_simulator* sim)
{
    // Clean up synchronization/threading variables.
    ////
    //// YOUR CODE HERE
    ////

    if(sim->vehicles != NULL)
    {
      // Free threading variables and control data stored in vehicles.
      for(int i = 0; i < sim->n_vehicles; i++)
      {
          if(sim->vehicles[i].control_data != NULL)
              free(sim->vehicles[i].control_data);
      }
      // Free the list of vehicles.
      free(sim->vehicles);
    }
}
