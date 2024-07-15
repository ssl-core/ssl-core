# playback

This directory contains the output of the playback service.

## Table of Contents

- [playback](#playback)
  - [Table of Contents](#table-of-contents)
  - [`detection`](#detection)
  - [`field`](#field)
  - [`game_status`](#game_status)
  - [`raw_detection`](#raw_detection) 
  - [`sample`](#sample)
  - [`tracked_detection`](#tracked_detection) 

## [`detection`](detection.proto)

A collection of messages that compose the detection.

- `Detection`: the detection representation for playback service. It contains properties of its generation &#8211; such as unique id and generated time &#8211;, ball and robots &#8211; the main entities of the game.

- `Ball`: a message containing the ball's kinematic properties.
- `Robot`: a message containing the robot's identification and kinematic properties.

## [`field`](field.proto)

A collection of messages that compose the field.

- `Field`: the dimensions of the field used in the game.

## [`game_status`](game_status.proto)

A collection of messages that compose the game status.

- `GameStatus`: the status of the game, including team information, game stage, and game events.

- `Team`: the details of a team participating in the match.

- `GameEventsProposal`: a proposal for a set of game events.

## [`raw_detection`](raw_detection.proto)

A collection of messages that compose the raw detection.

- `RawDetection`: the raw detection messages received from cameras.
- `Camera`: a camera detection, with related balls and robots.
- `Ball`: the ball's kinematic properties.
- `Robot`: the robot's identification and kinematic properties.

## [`sample`](sample.proto)

A collection of messages that compose a playback sample.

- `Sample`: the playback output, which provides a game sample. It contains properties to indicate time &#8211; such as the first timestamp, to indicate where the game has started, and the current timestamp &#8211;, field, detection and referee infomations &#8211; the main data of the game to be rendered &#8211;.

## [`tracked_detection`](tracked_detection.proto)

A collection of messages that compose the tracked detection.

- `TrackedDetection`: the tracked detection mapped from tracked vision data. It contains properties of its generation &#8211; such as unique id, generated time and source &#8211;, ball and robots &#8211; the main entities of the game &#8211;.

- `Ball`: a message containing the ball's kinematic properties.
- `KickInformation`: a message containing the kicked ball's information.
- `Robot`: a message containing the robot's identification and kinematic properties.
