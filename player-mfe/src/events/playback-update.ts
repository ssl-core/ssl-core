import Frame from "../entities/frame";

export type PlaybackUpdateEvent = {
  isPlaying: boolean;
  isLive: boolean;
  isLoading: boolean;
  currentTime: number;
  duration: number;
  frame: Frame | null;
};
