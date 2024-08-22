import Channels from "../../../config/channels";
import BaseSocketManager from "../base-communication-manager";

class TestSocketManager extends BaseSocketManager {
  private socket: number | null;
  private engineChannel: BroadcastChannel;
  private lastFrame: Frame | null;
  private fps: number;

  constructor(fps: number) {
    super();
    this.socket = null;
    this.engineChannel = new BroadcastChannel(Channels.Engine);
    this.lastFrame = null;
    this.fps = fps;
  }

  public connect(_address: string) {
    this.createTestInterval();
  }

  public disconnect() {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    clearInterval(this.socket);
    this.socket = null;
  }

  public send(message: any) {
    if (!this.socket) {
      throw new Error("Socket not initialized");
    }

    console.log("Message received: ", message);
  }

  private createTestInterval() {
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

      this.handleFrame(this.lastFrame);
    }, 1000 / this.fps);
  }

  private createDefaultFrame(): Frame {
    const balls: Ball[] = [
      {
        confidence: 1,
        position: [0, 0, 0],
        velocity: [0, 0, 0],
      },
    ];

    const field: Field = {
      length: 12,
      width: 9,
      goalWidth: 1.8,
      goalDepth: 0.18,
      boundaryWidth: 0.3,
      goalCenterToPenaltyMark: 9,
      penaltyAreaWidth: 3,
      penaltyAreaDepth: 1.8,
    };

    const robots: Robot[] = [];

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
      serialId: 0,
      startTime: new Date(),
      endTime: new Date(),
      currentTime: new Date(),
      fps: this.fps,
      balls,
      field,
      robots,
    };
  }

  private createRobot(
    id: number,
    color: RobotColor,
    x: number,
    y: number
  ): Robot {
    return {
      angle: 0,
      angularVelocity: 0,
      confidence: 1,
      dribblerWidth: 0,
      height: 0.147,
      position: [x, y],
      radius: 0.09,
      robotColor: color,
      robotId: id,
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

  private handleFrame(frame: Frame) {
    this.engineChannel.postMessage(frame);
  }
}

export default TestSocketManager;
