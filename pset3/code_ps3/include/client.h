#ifndef __CLIENT_H__
#define __CLIENT_H__

// Include files.
#include "vehicle.h"

// Declare functions to send data to Python.
void send_vehicles(int n_vehicles, vehicle* vehicles); // will use send_vehicle_data
void send_vehicle_data(int n_vehicles, double* pos_x, double* pos_y, double* theta);
void send_vehicle_waypoints(int vehicle_index, int num_waypoints, double** waypoints);
void send_data(char* data); // generic data transmission, used by the other functions

// Declare functions to open/close connection to Python.
void open_server(char* ip, int port);
void close_server();

// Declare function to start/stop the display server process.
void launch_display_server();
void close_display_server();

#endif // __CLIENT_H__
