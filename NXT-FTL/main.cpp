#include "brain.h"
#include "communication.h"
#include "movement_history.h"

using namespace std;
using namespace nxtftl;

int main()
{
    // Init
	movement_history movement_history{};
    communication communication{};
    brain brain { 10 };

	communication.connect(communication::BLUETOOF);

    auto leftMotor = communication.initializeMotor(communication::OUT_A);
    auto rightMotor = communication.initializeMotor(communication::OUT_C);

    auto touchSensor = touch_sensor_dto{};
    auto leftColorSensor = color_sensor_dto{};
    auto rightColorSensor = color_sensor_dto{};
    auto distanceSensor = distance_sensor_dto{};

    
    communication.initializeSensor(leftColorSensor, communication::IN_1);
    communication.initializeSensor(rightColorSensor, communication::IN_2);
    communication.initializeSensor(touchSensor, communication::IN_3);
    communication.initializeSensor(distanceSensor, communication::IN_4);

	cout << "Press touch sensor to begin..." << endl;
	while (!touchSensor.is_pressed)
	{
		communication.updateSensorValue(touchSensor);
	}
	// Because code executes too fast, brain thinks sensor is pressed otherwise
	while (touchSensor.is_pressed)
	{
		communication.updateSensorValue(touchSensor);
	}
	cout << "Starting line follow..." << endl;

    while (true)
    {
        // Read
        communication.updateSensorValue(touchSensor);
        communication.updateSensorValue(leftColorSensor);
        communication.updateSensorValue(rightColorSensor);
        communication.updateSensorValue(distanceSensor);

		long int left_motor_tacho_count = communication.get_tacho_count(leftMotor);
		long int right_motor_tacho_count = communication.get_tacho_count(rightMotor);
		movement_history.log_rotation(left_motor_tacho_count, right_motor_tacho_count);

        // Process
        tuple<int, bool> direction = brain.compute_direction(touchSensor, distanceSensor, leftColorSensor, rightColorSensor);

        // Send
		// TODO: Implement distance sensor pauses only. Resumes on clear path
        bool can_move = get<1>(direction);
        if (!can_move)
        {
            communication.stopMotor(leftMotor);
            communication.stopMotor(rightMotor);
            break;
        }
        
        int turn_factor = get<0>(direction);
        if (0 > turn_factor)
		{
			communication.startMotor(leftMotor, 10);
			communication.startMotor(rightMotor, 20);
		}
		else if (0 < turn_factor)
		{
			communication.startMotor(leftMotor, 20);
			communication.startMotor(rightMotor, 10);
		}
		else // turn_factor == 0
		{
			communication.startMotor(leftMotor, 10);
			communication.startMotor(rightMotor, 10);
		}
    }

	communication.disconnect();
}