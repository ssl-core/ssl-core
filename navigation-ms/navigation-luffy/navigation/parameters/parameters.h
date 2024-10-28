#ifndef NAVIGATION_PARAMETERS_PARAMETERS_H
#define NAVIGATION_PARAMETERS_PARAMETERS_H

#include <numbers>
#include <robocin/parameters/parameters.h>

namespace navigation {
// NOLINTBEGIN(*comment*)
constinit const auto pNavigationPollerTimeoutMs
    = ::robocin::parameters::View<1>::asInt32(10 /*ms ~= 100Hz*/);

inline const static double M_to_MM = 1e3;
inline const static bool USING_GLOBAL_VELOCITY = false;

inline const static double ANGLE_KP = 2.85; // 0 - 10

inline const static double ROTATE_IN_POINT_MIN_VEL_FACTOR = 0.18; // 0 - 1
inline const static double ROTATE_IN_POINT_APPROACH_KP = 2;       // 0 - 10
inline const static double ROTATE_IN_POINT_MAX_VELOCITY = 1.8;    // 0 - 5
inline const static double ROTATE_IN_POINT_ANGLE_KP = 5;          // 0 - 10

inline const static double ROTATE_IN_POINT_DIST_THRESHOLD = 220;
inline const static double ROTATE_IN_POINT_DIST_THRESHOLD_WITH_BALL = 145;
inline const static double ROTATE_IN_POINT_WITH_BALL_ADDITIONAL_DISTANCE = 30;

inline const static double ROBOT_MAX_ANGULAR_ACCELERATION = 14.4;
inline const static double ROBOT_MAX_ANGULAR_VELOCITY = 24.0;
inline const static double ROBOT_MIN_POSSIBLE_LINEAR_VELOCITY = 0.13;

inline const static double ANGLE_Y_MOVE_EXCLUSION_ZONE = std::numbers::pi / 4;
inline const static double DIST_TO_ADJUST_EXCLUSIVE_ZONE_MM = 400;

// larc 2023 parameters
inline const static double FACTOR_TO_MULTIPLY_BOUNDARY_WIDTH_WHEN_HITTING_WALLS = 0.6;

inline const static double CYCLE_STEP = 0.16;
inline const static double ROBOT_VEL_BREAK_DECAY_FACTOR = 3.1;
inline const static double ROBOT_VEL_FAVORABLE_DECAY_FACTOR = 0.003;

inline static constexpr double DEFAULT_TOLERANCE_TO_DESIRED_POSITION_M = 0.02;
inline static constexpr double SMALL_TOLERANCE_TO_DESIRED_POSITION_M = 0.01;
namespace moving_profile {

inline const static double STOP_MAX_VELOCITY = 1.350; // m/s

inline const static double ALLY_MAX_SPEED = 2.500;            // m/s
inline const static double ALLY_MIN_SPEED = 0.200;            // m/s
inline const static double MIN_DIST_TO_PROP_VELOCITY = 1.100; // m/s

inline const static double ALLY_MEDIAN_SPEED = 1.200; // m/s
inline const static double ALLY_SLOW_SPEED = 0.250;   // m/s

inline const static double ROBOT_HIGH_PRECISION_LINEAR_ACCELERATION = 1.100; // m/s^2
inline const static double ROBOT_DEFAULT_LINEAR_ACCELERATION = 4.500;        // m/s^2
inline const static double MAX_SPEED_IN_CARRY_BALL = 0.27;                   // m/s

inline const static double GOALKEEPER_MAX_SPEED = 3.400; // m/s
inline const static double ROBOT_LATERAL_CRITICAL_ACCELERATION = 5.400;

inline const static double APPROACH_BALL_MAX_SPEED = 0.400;     // m/s
inline const static double APPROACH_BALL_MIN_SPEED = 0.150;     // m/s
inline const static double APPROACH_BALL_PROP_DISTANCE = 1.000; // m/s

inline const static double KICK_BALL_MAX_SPEED = 0.340;                // m/s
inline const static double KICK_BALL_MAX_SPEED_IN_DIRECT_KICK = 0.47;  // m/s
inline const static double KICK_BALL_MIN_SPEED = 0.150;                // m/s
inline const static double KICK_BALL_MIN_SPEED_IN_DIRECT_KICK = 0.160; // m/s

inline const static double TOTOZINHO_MAX_SPEED = 1.5; // m/s
inline const static double TOTOZINHO_MIN_SPEED = 1;   // m/s

inline const static double KICK_BALL_SAFE_PROP_DISTANCE = 0.500;     // m/s
inline const static double KICK_BALL_BALANCED_PROP_DISTANCE = 0.600; // m/s

inline const static double KICK_BALL_WITH_ENEMY_MAX_SPEED = 2.0;  // m/s
inline const static double KICK_BALL_WITH_ENEMY_MIN_SPEED = 1.20; // m/s

inline const static double GOTO_BALL_WITH_ENEMY_MAX_SPEED = 3.0;    // m/s
inline const static double GOTO_BALL_WITH_ENEMY_ACCELERATION = 5.0; // m/s]

} // namespace moving_profile

// NOLINTEND(*comment*)
} // namespace navigation

#endif // NAVIGATION_PARAMETERS_PARAMETERS_H
