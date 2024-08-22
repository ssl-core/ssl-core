# gateway

This directory contains the definition of the Gateway Service which facilitates communication and data exchange between different components of the system.

## Table of Contents

- [gateway](#gateway)
  - [Table of Contents](#table-of-contents)
  - [Service Definition](#service-definition)
  - [Messages](#messages)

## Service Definition

### `GatewayService`

The `GatewayService` defines the following remote procedure calls (RPCs) to handle livestream data, retrieve game events, and fetch replay chunks:

- `ReceiveLivestream(stream ReceiveLivestreamRequest) returns (stream ReceiveLivestreamResponse)`: Handles livestream data by receiving a stream of `ReceiveLivestreamRequest` messages and returning a stream of `ReceiveLivestreamResponse` messages.
- `GetGameEvents(GetGameEventsRequest) returns (GetGameEventsResponse)`: Retrieves game events.
- `GetReplayChunk(GetReplayChunkRequest) returns (GetReplayChunkResponse)`: Fetches a chunk of replay data based on the specified start timestamp.

## Messages

### `ReceiveLivestreamRequest`

An empty message used as a request for receiving livestream data.

### `ReceiveLivestreamResponse`

A message containing a playback sample.

- `sample`: A `playback.Sample` message containing the sample data.

### `GetGameEventsRequest`

An empty message used as a request to retrieve game events.

### `GetGameEventsResponse`

A message containing a list of game events.

- `game_events`: A repeated field of `common.GameEvent` messages representing the game events.

### `GetReplayChunkRequest`

A message used to request a chunk of replay data starting from a specified timestamp.

- `start_timestamp`: A `google.protobuf.Timestamp` message indicating the start time for the replay chunk.

### `GetReplayChunkResponse`

A message containing the replay chunk data.

- `last_timestamp`: A `google.protobuf.Timestamp` message indicating the last timestamp in the replay chunk.
- `samples`: A repeated field of `playback.Sample` messages representing the samples in the replay chunk.
