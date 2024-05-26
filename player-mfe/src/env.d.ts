/// <reference types="vite/client" />
/// <reference types="vite-awesome-svg-loader" />

type Vector2 = [number, number];
type Vector3 = [number, number, number];
type RobotColor = "yellow" | "blue";
type WebSocketMessage = {
  type: string;
  payload: any;
};
