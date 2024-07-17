import SocketClient from "./socket-client";
import {
  BallResponse,
  ChunkResponse,
  FieldResponse,
  FrameResponse,
  RobotResponse,
} from "../../../types/requests";

class TestSocketClient implements SocketClient {
  private socket: number | null;
  private lastFrame: FrameResponse | null;
  private fps: number;
  private playing: boolean;
  private livestreaming: boolean;
  private connected: boolean;
  private startTime: number;

  constructor(fps: number) {
    this.socket = null;
    this.lastFrame = null;
    this.fps = fps;
    this.playing = false;
    this.livestreaming = false;
    this.connected = false;
    this.startTime = Date.now();
  }

  public connect(_address: string) {
    if (this.connected) {
      return;
    }

    this.createFakeSocket();
    this.sendState("connect");
    this.connected = true;
  }

  public disconnect() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    if (!this.connected) {
      return;
    }

    clearInterval(this.socket);
    this.sendState("disconnect");
    this.socket = null;
    this.connected = false;
  }

  public isConnected() {
    return this.connected;
  }

  public live() {
    this.sendState("live");
    this.playing = true;
    this.livestreaming = true;
  }

  public play(timestamp: number) {
    this.sendState("play");
    this.playing = true;
    this.livestreaming = false;
    this.fetchReplay(timestamp);
  }

  public pause() {
    if (!this.playing) {
      return;
    }

    this.sendState("pause");
    this.playing = false;
    this.livestreaming = false;
  }

  public isPlaying() {
    return this.playing;
  }

  public send(message: any) {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    console.log("Message received: ", message);
  }

  private fetchReplay(timestamp: number) {
    if (!this.lastFrame) {
      this.lastFrame = this.createDefaultFrame();
    }

    const CHUNK_SIZE = 100;

    let frames: FrameResponse[] = [];
    let currentTimestamp = timestamp;

    for (let i = 0; i < CHUNK_SIZE; i++) {
      this.lastFrame.balls[0].position = this.generateRandomPosition(
        this.lastFrame.balls[0].position
      );

      for (const robot of this.lastFrame.robots) {
        robot.position = this.generateRandomPosition(robot.position);
      }

      frames = [
        ...frames,
        {
          ...this.lastFrame,
          current_time: new Date(currentTimestamp).toISOString(),
        },
      ];
      currentTimestamp += 16;
    }

    const chunk: ChunkResponse = {
      request_time: new Date(timestamp).toISOString(),
      end_time: new Date(currentTimestamp).toISOString(),
      frames,
    };

    this.handleChunk(chunk);
  }

  private createFakeSocket() {
    this.lastFrame = this.createDefaultFrame();
    this.handleFrame(this.lastFrame);

    this.socket = setInterval(() => {
      if (!this.lastFrame) {
        throw new Error("Last frame not initialized");
      }

      this.lastFrame.balls[0].position = this.generateRandomPosition(
        this.lastFrame.balls[0].position
      );

      for (const robot of this.lastFrame.robots) {
        robot.position = this.generateRandomPosition(robot.position);
      }

      this.lastFrame.current_time = new Date().toISOString();

      this.handleFrame(this.lastFrame);
    }, 1000 / this.fps);
  }

  private handleFrame(frame: FrameResponse) {
    if (!this.playing || !this.livestreaming) {
      self.postMessage({
        type: "duration",
        payload: {
          start_time: frame.start_time,
          end_time: frame.current_time,
        },
      });
      return;
    }

    self.postMessage({ type: "frame", payload: frame });
  }

  private handleChunk(chunk: ChunkResponse) {
    if (!this.playing || this.livestreaming) {
      self.postMessage({
        type: "duration",
        payload: {
          start_time: chunk.frames[0].start_time,
          end_time: chunk.end_time,
        },
      });
      return;
    }

    self.postMessage({ type: "chunk", payload: chunk });
  }

  private createDefaultFrame(): FrameResponse {
    const balls: BallResponse[] = [
      {
        confidence: 1,
        position: [0, 0, 0],
        velocity: [0, 0, 0],
      },
    ];

    const field: FieldResponse = {
      length: 12,
      width: 9,
      goal_width: 1.8,
      goal_depth: 0.18,
      boundary_width: 0.3,
      goal_center_to_penalty_mark: 9,
      penalty_area_width: 3,
      penalty_area_depth: 1.8,
    };

    const robots: RobotResponse[] = [];

    for (let i = 0; i < 11; i++) {
      let robot;
      if (i === 0) {
        robot = this.createRobot(i, "blue", -12 / 2, 0);
      } else if (i < 6) {
        robot = this.createRobot(i, "blue", -3.5, i - 3);
      } else {
        robot = this.createRobot(i, "blue", -1.5, i - 8);
      }

      robots.push(robot);
    }

    for (let i = 0; i < 11; i++) {
      let robot;
      if (i === 0) {
        robot = this.createRobot(i, "yellow", 12 / 2, 0);
      } else if (i < 6) {
        robot = this.createRobot(i, "yellow", 3.5, (i - 3) * 1.5);
      } else {
        robot = this.createRobot(i, "yellow", 1.5, i - 8);
      }

      robots.push(robot);
    }

    return {
      serial_id: 0,
      start_time: new Date(this.startTime).toISOString(),
      current_time: new Date().toISOString(),
      fps: this.fps,
      balls,
      field,
      robots,
      match_type: "unspecified",
      home_team: {
        name: "RobôCIn",
        score: 0,
        is_positive_half: true,
        robot_color: "blue",
        goalkeeper_id: 0,
        yellow_cards: 0,
        time_to_expire_active_yellow_cards: [],
        red_cards: 0,
        timeouts_left: 4,
        total_timeout_time_left: 300000000000,
        fouls_committed: 0,
        consecutive_ball_placement_failures: 0,
        is_ball_placement_enabled: true,
        has_ball_placement_failures_reached_maximum: false,
        maximum_allowed_robots: 11,
        is_robot_substitution_requested: false,
        is_robot_substitution_allowed: false,
        robot_substitutions_left: 0,
        robot_substitution_time_left: 0,
      },
      away_team: {
        name: "Other team",
        score: 0,
        is_positive_half: false,
        robot_color: "yellow",
        goalkeeper_id: 0,
        yellow_cards: 0,
        time_to_expire_active_yellow_cards: [],
        red_cards: 0,
        timeouts_left: 4,
        total_timeout_time_left: 300000000000,
        fouls_committed: 0,
        consecutive_ball_placement_failures: 0,
        is_ball_placement_enabled: true,
        has_ball_placement_failures_reached_maximum: false,
        maximum_allowed_robots: 11,
        is_robot_substitution_requested: false,
        is_robot_substitution_allowed: false,
        robot_substitutions_left: 0,
        robot_substitution_time_left: 0,
      },
      game_stage: "GAME_STAGE_NORMAL_FIRST_HALF_PRE",
      game_stage_time_left: 300000000000,
      total_commands: 0,
      command_time: "1970-01-01T00:00:00Z",
      command: {
        Command: {
          Halt: {},
        },
      },
      next_command: {
        Command: {
          Halt: {},
        },
      },
      game_events: null,
      game_events_proposals: null,
    };
  }

  private createRobot(
    id: number,
    color: RobotColor,
    x: number,
    y: number
  ): RobotResponse {
    return {
      angle: 0,
      angular_velocity: 0,
      confidence: 1,
      dribbler_width: 0,
      height: 0.147,
      position: [x, y],
      radius: 0.09,
      robot_color: color,
      robot_id: id,
      velocity: [0, 0],
    };
  }

  private generateRandomPosition<T extends Vector2 | Vector3>(
    oldPosition: T
  ): T {
    const newPosition = [...oldPosition];

    newPosition[0] += Math.random() * 0.02 - 0.01;
    newPosition[1] += Math.random() * 0.02 - 0.01;

    if (newPosition[0] > 12 / 2 || newPosition[0] < -12 / 2) {
      newPosition[0] = oldPosition[0];
    }

    if (newPosition[1] > 9 / 2 || newPosition[1] < -9 / 2) {
      newPosition[1] = oldPosition[1];
    }

    return newPosition as T;
  }

  private sendState(state: string) {
    self.postMessage({ type: "state", payload: state });
  }
}

export default TestSocketClient;
