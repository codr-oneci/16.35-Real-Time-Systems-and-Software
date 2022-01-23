// Author: Codrin Oneci
#define PI 3.14159265
#define SIMULATOR_TIME_INCREMENT 0.01 //I got this from simulator.c
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vehicle.h"
#include "controller.h"
#include "simulator.h"




// create vehicle
vehicle * create_vehicle(double * starting_position, int num_waypoints, double ** offset_waypoints)
	{

		vehicle * new_vehicle = (vehicle *) malloc(sizeof(vehicle));//dynamically alocate memory for a new vehicle object
		new_vehicle->position[0]=starting_position[0];
		new_vehicle->position[1]=starting_position[1];
		new_vehicle->position[2]=starting_position[2];
		new_vehicle->num_waypoints = num_waypoints;//store the number of waypoints data in the new structure
		new_vehicle->target_waypoints = malloc(num_waypoints * sizeof(double)+3);
		//allocate space for the waypoints, which are num_waypoints many, positive integer number
		//I know that a waypoint has x and y coordinate, so need 2*sizeof(double) for that
		//however I decided to allocate slightly more memory for safety (maybe adding +1 instead of +3 would also work well)
    		for (int i = 0; i < num_waypoints; i++) 
		{
        		new_vehicle->target_waypoints[i] = malloc(2 * sizeof(double)+3);
        		new_vehicle->target_waypoints[i][0] = starting_position[0] + offset_waypoints[i][0];//x
			//I simply build vector x and y components by adding the absolute position from starting_position
			//with the relative position stored in offset_waypoints
        		new_vehicle->target_waypoints[i][1] = starting_position[1] + offset_waypoints[i][1];//y
    		}

    		//the first target from the list must be the current target waypoint at the initial time 
    		new_vehicle->current_waypoint = new_vehicle->target_waypoints[0];
    		// the functions from the structure are given outside pointers towards function instructions
    		new_vehicle->set_position = &set_position;
    		new_vehicle->set_velocity = &set_velocity;
    		new_vehicle->control_vehicle = &control_vehicle;
    		new_vehicle->update_state = &update_state;
    		return new_vehicle;
	}


// standard vehicle methods
void set_position   (struct t_vehicle * v,double * values)
	{
		//values[0] shall be x, values[1] is y, values[2] is theta
    		if (values[0]<100 && values[0]>0)//if the x value is in (0, 100), according to spec 1.2
    			{
				v->position[0]=values[0];
			}// set x value
    		if (values[1]<100 && values[1]>0)//if the y value is in (0, 100), according to spec 1.2
    			{
				v->position[1]=values[1];
			}//set the y value
    		if (values[2]<PI && values[2]>-PI)//if the theta value is in (-PI, PI), according to spec 1.3
    			{
				v->position[2]=values[2];
			}//set theta value
		
		if (v->position[0] > 100.0)
			{
				v->position[0] = 100.0;
			}
		else if (v->position[0] < 0.0)  
			{
				v->position[0] = 0.0;
			}
		if (v->position[1] > 100.0)
			{
				v->position[1] = 100.0;
			}
		else if (v->position[1] < 0.0)
			{
				v->position[1] = 0.0;
			}
    		if (v->position[2] > PI)
			{
				v->position[2] = PI;//satisfies the specification boundaries and apparently gives good behavior when theta=PI (approximative), allowing for rotation afterwise
				//v->position[2] -= 2.0*PI; //causes a crash into the border

			}
    		else if (v->position[2] < -PI)
			{
				v->position[2] = -PI;
				//v->position[2] += 2.0*PI;//crashes into the border because it is not changing
			}
	}


void set_velocity   (struct t_vehicle * v,double * values)
	{//put the values from the values array to the vehicle velocity components array
    //memcpy(v->velocity,values,3*sizeof(double));//copy 3 values into the v->velocity array
		v->velocity[0]=values[0];
		v->velocity[1]=values[1];
		v->velocity[2]=values[2];
    // I limit the translational speed between 5 and 10 according to the technical specification 1.2 and 1.3
    //double heading = v->position[2];
		if (sqrt(pow(v->velocity[0],2)+pow(v->velocity[1],2)) < 5.0)
		{
        		v->velocity[0] = 5*cos(v->position[2]);
        		v->velocity[1] = 5*sin(v->position[2]);
		}
		if (sqrt(pow(v->velocity[0],2)+pow(v->velocity[1],2)) > 10.0)
		{
        		v->velocity[0] = 10*cos(v->position[2]);
        		v->velocity[1] = 10*sin(v->position[2]);
		}
		//the specification 1.5 requires angular velocity of magnitude less than 0.25*PI rad/s
    		if (v->velocity[2]>PI*0.25)  
		{
			v->velocity[2] = PI*0.25;
		}
    		if (v->velocity[2]<-PI*0.25)
		{
			v->velocity[2] = -PI*0.25;
		}
	}


void control_vehicle(struct t_vehicle * v)
	{
		double speed[3];
		control c_law = get_proportional_waypoint_control(v);//get velocity control law
		control * p = &c_law;//create pointer to the c_law adress
		speed[0]=p->speed*cos(v->position[2]);
		speed[1]=p->speed*sin(v->position[2]);
		speed[2]=p->angular_velocity;
    		v->set_velocity(v,speed);//update velocity
	}


void update_state   (struct t_vehicle * v, double time)
	{//in order to update the state, do Euler integartion first, and afterwise apply controls
	//another version could be to first apply controls, and afterwise do 1st order time 		integration
	double new_position [3],errx,erry;
	new_position[0]=v->position[0]+time*(v->velocity[0]);
	new_position[1]=v->position[1]+time*(v->velocity[1]);
	new_position[2]=v->position[2]+time*(v->velocity[2]);
    	// apply continuous corrections to the azimuth if it is outside of the boudaries
	//use the 2*PI periodicity of trig functions
    	if (new_position[2] > PI)
	{
		new_position[2] -= 2.0*PI;
	}
    	if (new_position[2] < -PI)
	{
		new_position[2] += 2.0*PI;
	}
    	v->set_position(v,new_position);//set the position array using new_position data
    	errx = v->current_waypoint[0] - v->position[0];
    	erry = v->current_waypoint[1] - v->position[1];
    	//if (sqrt(x_error * x_error + y_error * y_error) < 0.1)
	//calculate space epsilon: 2*max_speed*delta_time 
	if(pow(pow(errx,2)+pow(erry,2),0.5)<2*10*time)//aircraft is very close to current waypoint
	{
 		if (v->current_waypoint_idx !=v->num_waypoints-1)//if not at the end of the waypoint array
		{
			v->current_waypoint=v->target_waypoints[v->current_waypoint_idx+1];
			v->current_waypoint_idx+=1;}
		else // we reached the end of the waypoint array
		{
			//I chose to impose ciclicity as sugested by the problem statement, other versions may put the aircraft to rotate around the last waypoint or do a random walk 
			v->current_waypoint=v->target_waypoints[0];
			v->current_waypoint_idx=0;
		}
    }


}
