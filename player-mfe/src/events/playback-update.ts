import Frame from "../entities/frame";

export type PlaybackUpdateEvent = {
  isPlaying: boolean;
  currentTime: number;
  duration: number;
  frame: Frame | null;
};
