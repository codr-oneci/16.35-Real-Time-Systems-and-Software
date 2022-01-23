#include <pthread.h>
#include "vehicle.h"
#include <stdlib.h>
#include "simulator.h"
#include "client.h"
#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
simulator * create_simulator(){
    // initialization
    simulator * sim = malloc(sizeof(simulator));
    sim->run = &run;
    sim->n_vehicles = 0;
    // update rate for display server
    sim->vehicle_update_rate = 25; // hz
    // Create "offset waypoints", a series of waypoints for the vehicles to follow.
    // Note that these waypoints are for the *relative* waypoints relative to the
    // starting point of the vehicle.
    sim->num_waypoints = 5;
    sim->radius = 30;
    sim->offset_waypoints = malloc((sim->num_waypoints + 1) * 2 * sizeof(double));
    for (int i = 0; i < sim->num_waypoints + 1; i++) {
        sim->offset_waypoints[i] = malloc(2 * sizeof(double));
        sim->offset_waypoints[i][0] = sim->radius * cos( i * 2 * M_PI / (sim->num_waypoints));
        sim->offset_waypoints[i][1] = sim->radius * sin( i * 2 * M_PI / (sim->num_waypoints));
    }
    // simulator settings
    sim->max_time = 100.0;
    sim->current_time = 0.0;
    sim->time_increment = 0.01;
    return sim;
}
void run(struct t_simulator * sim){
    open_server(IP,PORTNUM);
    sim->current_time = 0.0;
    double time_vehicle_message = 0.0;

double time_avg;
double n=0.0;
    while (sim->current_time < sim->max_time) {n+=1.0;
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	//printf("\rt = %f",sim->current_time);
        time_vehicle_message += sim->time_increment;
        sim->current_time += sim->time_increment;
        if (time_vehicle_message > 1.0/sim->vehicle_update_rate) {
            send_vehicles(sim->n_vehicles,sim->vehicles);
            time_vehicle_message = 0.0;
        }
        for (vehicle * v = sim->vehicles; v < sim->vehicles + sim->n_vehicles; v++){
            v->control_vehicle(v);
            v->update_state(v,sim->time_increment); // delta t
        }
        usleep(sim->time_increment*1e6); // sleep for roughly the time increment so we get quasi-realtime behavior
	gettimeofday(&stop, NULL);

	time_avg=(n-1.0)/n*time_avg+1.0/n*((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - 	start.tv_usec);
	printf("\rAverage iteration time was %f microseconds",time_avg);}
    close_server();
}


struct descr_thread {
vehicle * v;
double time;} ;//I need v and time_increment in the structure because function update_state
//that from vehicle.c is of the form (indicated in the specification) void update_state   (struct t_vehicle * v, double time)

void* thr_function(void * ar){
    struct descr_thread * dt = (struct descr_thread *) ar;
    dt->v->control_vehicle(dt->v);
    dt->v->update_state(dt->v,dt->time);}

//based on the existing run function, create a threaded version of it
void run_threaded(struct t_simulator * sim){
    	open_server(IP,PORTNUM);
    	sim->current_time = 0.0;
   	double time_vehicle_message = 0.0,time_avg,n=0.0;
	int cntr =0;
    	while (sim->current_time < sim->max_time) {
		n+=1.0;
        	struct timeval stop, start;
		gettimeofday(&start, NULL);
        	time_vehicle_message += sim->time_increment;
        	sim->current_time += sim->time_increment;
        	if (time_vehicle_message > 1.0/sim->vehicle_update_rate) {
            		send_vehicles(sim->n_vehicles,sim->vehicles);
            		time_vehicle_message = 0.0;
        	}
        	pthread_t ids[sim->n_vehicles];//create identifiers for the threads, one for each vehicle
        	struct descr_thread info [sim->n_vehicles];//each vehicle thread needs info about vehicle pointer and time increment
	//up to this point, run and run_threaded are almost identical
		cntr=-1;
        	for (vehicle * v = sim->vehicles; v < sim->vehicles + sim->n_vehicles; v++){cntr++;
		//here I implement threading modifications
            	info[cntr].time = sim->time_increment;
            	info[cntr].v = v;
            	pthread_create(&ids[cntr],NULL,&thr_function,&info[cntr]);
            
        	}
        	for (int i = 0; i < sim->n_vehicles; i++)
		{
		pthread_join(ids[i],NULL);
		}
        	usleep(sim->time_increment*1e6); // sleep for roughly the time increment so we get quasi-realtime behavior
		gettimeofday(&stop, NULL);
		time_avg=(n-1.0)/n*time_avg+1.0/n*((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - 	start.tv_usec);
		printf("\rAverage iteration time was %f microseconds",time_avg);
    		}
    	close_server();
	}



