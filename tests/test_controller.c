#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "vehicle.h"
#include "controller.h"
int main (int argc, char ** argv) {
    // test controller functionality
//do the first test if name matches T1
if(strcmp(argv[1],"T1")==0){
//initialize the first vehicle
double * starting_position=malloc(100*sizeof(double));
starting_position[0]=0.1;
starting_position[1]=1.5;
starting_position[2]=0.2;
int num_waypoints=1;
double ** offset_waypoints=malloc(100*sizeof(double));
for (int i=0;i<num_waypoints;i++) offset_waypoints[0]=malloc(100*sizeof(double));
offset_waypoints[0][0]=2.8;
offset_waypoints[0][1]=1.0;
vehicle *v = create_vehicle(starting_position,num_waypoints,offset_waypoints);
control_vehicle(v);
//initialize the second vehicle
double * starting_position1=malloc(100*sizeof(double));
starting_position1[0]=1.1;
starting_position1[1]=2.5;
starting_position1[2]=0.2;
int num_waypoints1=1;
double ** offset_waypoints1=malloc(100*sizeof(double));
for (int i=0;i<num_waypoints1;i++) offset_waypoints1[0]=malloc(100*sizeof(double));
offset_waypoints1[0][0]=3.8;
offset_waypoints1[0][1]=2.0;
vehicle *v1 = create_vehicle(starting_position1,num_waypoints1,offset_waypoints1);
control_vehicle(v1);
//initialize the third vehicle
double * starting_position2=malloc(100*sizeof(double));
starting_position2[0]=2.1;
starting_position2[1]=3.5;
starting_position2[2]=0.2;
int num_waypoints2=1;
double ** offset_waypoints2=malloc(100*sizeof(double));
for (int i=0;i<num_waypoints2;i++) offset_waypoints2[0]=malloc(100*sizeof(double));
offset_waypoints2[0][0]=4.8;
offset_waypoints2[0][1]=3.0;
vehicle *v2 = create_vehicle(starting_position2,num_waypoints2,offset_waypoints2);
control_vehicle(v2);

//test if velocities are consistent for checking translation symmetry of the problem is satisfied
if (v->velocity[0]!=v1->velocity[0]) return -1;
if (v->velocity[1]!=v1->velocity[1]) return -1; 
//if (v->velocity[2]!=v1->velocity[2]) return -1; 
if (v->velocity[0]!=v2->velocity[0]) return -1;
if (v->velocity[1]!=v2->velocity[1]) return -1; 
//if (v->velocity[2]!=v2->velocity[2]) return -1;
return 0; // consistency test passed
}

//or do the second test named T2
if (strcmp(argv[1],"T2")==0) 
{double * starting_position=malloc(100*sizeof(double));
starting_position[0]=0.1;
starting_position[1]=1.5;
starting_position[2]=1.1;
int num_waypoints=1;
double ** offset_waypoints=malloc(100*sizeof(double));
for (int i=0;i<num_waypoints;i++) offset_waypoints[0]=malloc(100*sizeof(double));
offset_waypoints[0][0]=2.7;
offset_waypoints[0][1]=1.0;
vehicle *v = create_vehicle(starting_position,num_waypoints,offset_waypoints);
control_vehicle(v);
//Check that position and velocity bounds are satisfied
if (v->position[0]<0||v->position[0]>=100) return -1;
if (v->position[1]<0||v->position[1]>=100) return -1;
if (v->position[2]<-3.14159265/2||v->position[2]>=3.14159265/2) return -1;
if ( sqrt( pow(v->velocity[0],2) + pow(v->velocity[1],2) )<5|| sqrt( pow(v->velocity[0],2) + pow(v->velocity[1],2) ) >10) return -1;
if (v->velocity[2]<-3.14159265/4||v->velocity[2]>3.14159265/4) return -1;
return 0;

}
}
