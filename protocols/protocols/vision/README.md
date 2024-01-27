# vision

A collection of messages related to RobôCIn's vision system.

## Table of Contents

- [vision](#vision)
  - [Table of Contents](#table-of-contents)
  - [`frame`](#frame)

## [`frame`](frame.proto)

- `Frame`: a message containing the frame's timestamp, optionally the field's dimensions, and the robots and ball detected in the frame.
- `Ball`: a message containing the ball's position and velocity. Also contains the ball's kick status when it is
  detected.
- `Field`: a message containing the field's dimensions.
- `Robot`: a message containing the robot's id, color, position, orientation, and velocity.
