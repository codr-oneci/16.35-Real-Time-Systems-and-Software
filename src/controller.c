//Author: Codrin Oneci
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "controller.h"
#include "vehicle.h"
#include "simulator.h"

#define PI 3.14159265


control get_proportional_waypoint_control(struct t_vehicle * vehicle)

    {
	control c_law;//create control law object
	c_law.speed=5.1;//set a speed between 5 and 10
	//I chose a smaller cruise speed because it helps with increasing the space resolution of the forward Euler integration in time
	//In some situations, choosing the larger speed (close to 10) can result in less refined maneuvers
	double * position = vehicle->position, * target = vehicle->current_waypoint;
    	double err_theta = atan2((target[1] - position[1]), (target[0] -position[0]))-position[2];
	//angular velocity controlaccording : specification 2.6
	//however, verify that spec 2.7 is met
    	if (err_theta > PI)
	{
       		err_theta -= 2*PI;
   	 } 
	if (err_theta < -1 * PI)
	{
        	err_theta += 2*PI;
    	}

	//check also that spec 2.8 is met
    	if (err_theta > PI*0.25) 
	{
        	err_theta = PI*0.25;
   	} 
	if (err_theta < -1 * PI*0.25) 
	{
        	err_theta = -1 *PI*0.25;
   	 }
	c_law.angular_velocity=err_theta;
    	return c_law;


    }
