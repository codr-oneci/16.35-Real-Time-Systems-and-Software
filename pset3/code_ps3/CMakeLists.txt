cmake_minimum_required(VERSION 2.6.0)

project(VehicleSimulator C)

include_directories(include/)

set (SimulatorFiles)
list(APPEND SimulatorFiles
	include/client.h
	include/simulator.h
	include/controller.h
	include/vehicle.h
	src/client.c
	src/simulator.c
	src/vehicle.c
	src/controller.c
)
add_definitions(-DPORTNUM=1635 -DIP="127.0.0.1")

# create the executable
add_executable(main src/main.c ${SimulatorFiles})
target_link_libraries(main m pthread)
