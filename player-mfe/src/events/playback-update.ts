import Frame from "../entities/frame";

export type PlaybackUpdateEvent = {
  isPlaying: boolean;
  isLive: boolean;
  currentTime: number;
  duration: number;
  frame: Frame | null;
};
