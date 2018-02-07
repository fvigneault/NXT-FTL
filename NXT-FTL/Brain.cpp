#include "Brain.h"

Brain::Brain(unsigned int stopDistance) : stopDistance(stopDistance)
{
}

Brain::~Brain()
{
}

std::tuple<int, bool> Brain::ComputeDirection(TouchSensorDto touchSensor, DistanceSensorDto distanceSensor, ColorSensorDto leftColorSensor, ColorSensorDto rightColorSensor)
{
	bool stopping = false;
	// The touch sensor stop the robot
	//if touchsenso is touch => stop
	if (touchSensor.isPressed)
	{
		stopping = true;
	}

	// The distance before the robot stop
	// if distanceSensor.distance < stopDistance => stop
	if (distanceSensor.Distance <= stopDistance)
	{
		stopping = true;
	}

	int direction{ 0 };
	// Calculate the direction of the robot
	if (!stopping)
	{
		auto totalLight = leftColorSensor.BlueValue + leftColorSensor.GreenValue + leftColorSensor.RedValue;
	}


	return std::tuple<int, bool>{direction, !stopping};
}