#include "movement_history.h"

movement_history::movement_history()
{
	snapshots = std::vector<snapshot>();
	positions = std::vector<position>();

	using namespace std::chrono;
	milliseconds now = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	snapshots.push_back({ now, 0, 0 });
	positions.push_back({ 0, 0, 0 });
}

movement_history::position movement_history::calculate_new_position(position initial_position, snapshot initial_snapshot, snapshot destination_snapshot)
{
	float left_tacho_count_from_last_snapshot = destination_snapshot.left_motor_tacho_count - initial_snapshot.left_motor_tacho_count;
	float right_tacho_count_from_last_snapshot = destination_snapshot.right_motor_tacho_count - initial_snapshot.right_motor_tacho_count;

	float sl_distance_left_wheel = left_tacho_count_from_last_snapshot / 360 * WHEEL_CIRCUMFERENCE;
	float sr_distance_right_wheel = right_tacho_count_from_last_snapshot / 360 * WHEEL_CIRCUMFERENCE;

	float O_angle_of_turn_in_rad = (sl_distance_left_wheel - sr_distance_right_wheel) / DISTANCE_BETWEEN_WHEELS;
	float r_turn_radius_left_wheel = std::fmin(sr_distance_right_wheel, sl_distance_left_wheel) / std::abs(O_angle_of_turn_in_rad);

	float initial_axle_angle_rad = initial_position.direction_in_rad - (PI / 2);

	float distance_from_center_of_rotation_to_center_of_robot = (r_turn_radius_left_wheel + (DISTANCE_BETWEEN_WHEELS / 2));
	float center_of_rotation_x = initial_position.x + cos(initial_axle_angle_rad) * distance_from_center_of_rotation_to_center_of_robot;
	float center_of_rotation_y = initial_position.y - sin(initial_axle_angle_rad) * distance_from_center_of_rotation_to_center_of_robot;
	
	float final_axle_angle_rad = initial_axle_angle_rad - O_angle_of_turn_in_rad;

	position new_position;
	new_position.direction_in_rad = initial_position.direction_in_rad + O_angle_of_turn_in_rad;
	new_position.x = center_of_rotation_x - sin(final_axle_angle_rad) * distance_from_center_of_rotation_to_center_of_robot;
	new_position.y = center_of_rotation_y + cos(final_axle_angle_rad) * distance_from_center_of_rotation_to_center_of_robot;

	return new_position;
}

void movement_history::log_rotation(long int left_motor_tacho_count, long int right_motor_tacho_count)
{
	using namespace std::chrono;
	milliseconds now = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
	);

	snapshots.push_back({ now, left_motor_tacho_count, right_motor_tacho_count });
	position new_position = calculate_new_position(positions[positions.size() - 1], snapshots[snapshots.size() - 2], snapshots[snapshots.size() - 1]);
	positions.push_back(new_position);
}

movement_history::position movement_history::get_current_position()
{
	return positions[positions.size() - 1];
}

std::vector<movement_history::position> movement_history::get_positions()
{
	return positions;
}