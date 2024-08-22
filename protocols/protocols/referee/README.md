# referee

This directory contains the output of the referee service.

## Table of Contents

- [referee](#referee)
  - [Table of Contents](#table-of-contents)
  - [`GameStatus`](#gamestatus)

## [`GameStatus`](game_status.proto)

A collection of messages that compose the game status.

- `GameStatus`: the overall status of the game, including metadata and state information.
  - `source_id`: Identifier of the source providing the game status.
  - `description`: (Optional) Description of the current game status.
  - `timestamp`: The time at which the game status was recorded.
  - `match_type`: The type of match being played.
  - `home_team`: Details of the home team.
  - `away_team`: Details of the away team.
  - `game_stage`: The current stage of the game.
  - `game_stage_time_left`: (Optional) Time remaining in the current game stage.
  - `total_commands_issued`: The total number of commands issued during the game.
  - `command_issued_timestamp`: The time when the last command was issued.
  - `command`: The current game command.
  - `next_command`: The next game command to be issued.
  - `game_events`: A list of game events that have occurred.
  - `game_events_proposals`: A list of proposed game events and their statuses.

- `Team`:  Details of a team participating in the match.

- `GameEventsProposal`: A proposal for a set of game events.
