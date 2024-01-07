# RoboCup's Small Size League Messages

This directory contains the libraries needed for a RoboCup Small Size League game.

## Protobufs

The `.proto` files in this directory are named differently from the original files. The mapping between the files
described here and the original files are shown below:

### detection

| File Name                                        | Corresponding SSL Vision Proto       |
| ------------------------------------------------ | ------------------------------------ |
| [geometry.proto](detection/geometry.proto)       | messages_robocup_ssl_geometry.proto  |
| [raw.proto](detection/raw.proto)                 | messages_robocup_ssl_detection.proto |
| [raw_wrapper.proto](detection/raw_wrapper.proto) | messages_robocup_ssl_wrapper.proto   |

Original files are in [`RoboCup-SSL/ssl-vision`](https://github.com/RoboCup-SSL/ssl-vision).

### game_controller

| File Name                                                      | Corresponding SSL Game Controller Proto |
| -------------------------------------------------------------- | --------------------------------------- |
| [common.proto](game_controller/common.proto)                   | ssl_gc_common.proto                     |
| [event.proto](game_controller/event.proto)                     | ssl_gc_game_event.proto                 |
| [geometry.proto](game_controller/geometry.proto)               | ssl_gc_geometry.proto                   |
| [referee.proto](game_controller/referee.proto)                 | ssl_gc_referee_message.proto            |
| [tracked.proto](game_controller/tracked.proto)                 | ssl_vision_tracked.proto                |
| [tracked_wrapper.proto](game_controller/tracked_wrapper.proto) | ssl_vision_detection_tracked.proto      |

Original files are in [`RoboCup-SSL/ssl-game-controller`](https://github.com/RoboCup-SSL/ssl-game-controller).

### simulation

| File Name                                                                       | Corresponding SSL Simulation Proto                     |
| ------------------------------------------------------------------------------- | ------------------------------------------------------ |
| [config.proto](simulation/config.proto)                                         | ssl_simulation_config.proto                            |
| [control.proto](simulation/control.proto)                                       | ssl_simulation_control.proto                           |
| [error.proto](simulation/error.proto)                                           | ssl_simulation_error.proto                             |
| [robot_control.proto](simulation/robot_control.proto)                           | ssl_simulation_robot_control.proto                     |
| [robot_feedback.proto](simulation/robot_feedback.proto)                         | ssl_simulation_robot_feedback.proto                    |
| [synchronous.proto](simulation/synchronous.proto)                               | ssl_simulation_synchronous.proto                       |
| [erforce/custom_realism.proto](simulation/erforce/custom_realism.proto)         | erforce/ssl_simulation_custom_erforce_realism.proto    |
| [erforce/custom_robot_specs.proto](simulation/erforce/custom_robot_specs.proto) | erforce/ssl_simulation_custom_erforce_robot_spec.proto |

Original files are in [`RoboCup-SSL/ssl-simulation-protocol`](https://github.com/RoboCup-SSL/ssl-simulation-protocol).

> **Note**: The `erforce` directory contains custom messages for the [ER-Force simulation](https://github.com/robotics-erlangen/framework#simulator-cli).
