#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

// Declare global variables.
struct hostent *he;
struct sockaddr_in their_addr; /* connector's address information */
int sockfd, numbytes;

// Send vehicles to be updated on the display.
// Wrapper for send_vehicle_data for convenience.
void send_vehicles(int n_vehicles, vehicle* vehicles)
{
    // Create arrays of x/y/theta positions for all vehicles.
    double* pos_x = malloc(n_vehicles * sizeof(double));
    double* pos_y = malloc(n_vehicles * sizeof(double));
    double* theta= malloc(n_vehicles * sizeof(double));
    for(int vehicle_index = 0; vehicle_index < n_vehicles; vehicle_index++)
    {
      pos_x[vehicle_index] = (vehicles + vehicle_index)->position[0];
      pos_y[vehicle_index] = (vehicles + vehicle_index)->position[1];
      theta[vehicle_index] = (vehicles + vehicle_index)->position[2];
    }

    // Send the data.
    send_vehicle_data(n_vehicles, pos_x, pos_y, theta);

    // Clean up.
    free(pos_x);
    free(pos_y);
    free(theta);
}


// Send data about the current vehicle positions to the display.
void send_vehicle_data(int n_vehicles, double* pos_x, double* pos_y, double* theta)
{
    // Construct pickle datagram.
    // FYI: if you're reading this, this isn't normally how one would construct a datagram.
    //      this is because the display server is written in python and cpickle was used.
    //      we'll likely cover how to do this properly later in the course --ckf
    char data [1024];
    data[0] = '\0'; // null terminator for string
    // Add the number of vehicles
    sprintf(data,"(I%i\n",n_vehicles);
    // Add the x positions
    sprintf(data,"%s(lp1\n",data);
    sprintf(data,"%sF%f\n",data,pos_x[0]);
    for (int i = 1; i < n_vehicles;i++) sprintf(data,"%saF%f\n",data,pos_x[i]);
    // Add the y positions
    sprintf(data,"%sa(lp2\n",data);
    sprintf(data,"%sF%f\n",data,pos_y[0]);
    for (int i = 1; i < n_vehicles;i++) sprintf(data,"%saF%f\n",data,pos_y[i]);
    // Add the theta positions
    sprintf(data,"%sa(lp3\n",data);
    sprintf(data,"%sF%f\n",data,theta[0]);
    for (int i = 1; i < n_vehicles;i++) sprintf(data,"%saF%f\n",data,theta[i]);
    sprintf(data,"%satp4\n.",data);
    // Send the constructed message.
    send_data(data);
}

// Send data about the waypoints to display.
void send_vehicle_waypoints(int vehicle_index, int num_waypoints, double** waypoints)
{
    // Construct pickle datagram.
    // FYI: if you're reading this, this isn't normally how one would construct a datagram.
    //      this is because the display server is written in python and cpickle was used.
    //      we'll likely cover how to do this properly later in the course --ckf

    char data [1024];
    data[0] = '\0'; // null terminator for string
    // Add the vehicle index
    sprintf(data,"(I%i\n", vehicle_index);
    // Add the waypoint x positions
    sprintf(data,"%s(lp1\n", data);
    for(int waypoint_index = 0; waypoint_index < num_waypoints; waypoint_index++)
    {
      if(waypoint_index == 0)
        sprintf(data,"%sF%f\n", data, waypoints[waypoint_index][0]);
      else
        sprintf(data, "%saF%f\n", data, waypoints[waypoint_index][0]);
    }
    // Add the waypoint y positions
    sprintf(data,"%sa(lp2\n", data);
    for(int waypoint_index = 0; waypoint_index < num_waypoints; waypoint_index++)
    {
      if(waypoint_index == 0)
        sprintf(data,"%sF%f\n", data, waypoints[waypoint_index][1]);
      else
        sprintf(data, "%saF%f\n", data, waypoints[waypoint_index][1]);
    }
    sprintf(data,"%satp4\n.",data);
    // Send the data
    send_data(data);
    // Make sure it was received and processed
    sleep(1);
}

// Send data to the server
void send_data(char* data)
{
    if (send(sockfd, data, strlen(data), 0) == -1)
    {
        perror("send");
        exit(1);
    }
}

// Open the server for Python communiction
void open_server(char* ip, int port)
{
    if ((he=gethostbyname(ip)) == NULL)
    {  /* get the host info */
        herror("gethostbyname");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;      /* host byte order */
    their_addr.sin_port = htons(PORTNUM);    /* short, network byte order */
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

    if (connect(sockfd, (struct sockaddr *)&their_addr, \
                                          sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }
}

// Close the server for Python communication
void close_server()
{
    close(sockfd);
}


// Launch the Python display process
pid_t child;
void launch_display_server()
{
    int fd[2];
    pipe(fd); // for stdout
    // fork into a subprocess
    child = fork();
    if (child == (pid_t)-1)
    {
        // error! probably out of resources;
        exit(1);
    }
    if (!child)
    {
        execlp("/usr/bin/python2.7","python2.7","../python/DisplayServer.py",NULL);
        exit(-1);
    }
}

// Kill the Python display process.
void close_display_server()
{
    if(child) 
      kill(child,SIGKILL);
}
