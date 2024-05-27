import EventBus from "../lib/event-bus/event-bus";
import SocketHandler from "../lib/socket/socket-handler";
import { provide } from "./services/global-provider";
import { PlaybackUpdateEvent } from "../events/playback-update";
import { html } from "../utils/literals";
import { Frame } from "../entities/frame";

import styles from "./styles/main.css?inline";

import "./components/player-mfe-slider";
import "./components/player-mfe-controls";
import "./components/player-mfe-info";
import "./components/player-mfe-actions";

type PlayerMFEState = {
  isPlaying: boolean;
  currentTime: number;
  duration: number;
};

class PlayerMFE extends HTMLElement {
  private root: ShadowRoot;
  private eventBus: EventBus;
  private socketHandler: SocketHandler;
  private state: PlayerMFEState;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.eventBus = new EventBus();
    this.socketHandler = new SocketHandler("0.0.0.0", this.eventBus);
    this.state = {
      isPlaying: false,
      currentTime: 0,
      duration: 0,
    };
  }

  public async connectedCallback() {
    this.provideDependencies();
    this.render();
    this.listenToEvents();
  }

  public disconnectedCallback() {
    this.socketHandler.disconnect();
  }

  public render() {
    this.root.innerHTML = html`
      <div class="container">
        <player-mfe-info></player-mfe-info>
        <div class="container__controls">
          <player-mfe-controls></player-mfe-controls>
          <player-mfe-slider></player-mfe-slider>
        </div>
        <player-mfe-actions></player-mfe-actions>
      </div>
      <style>
        ${styles}
      </style>
    `;
  }

  private listenToEvents() {
    this.socketHandler.connect();

    this.eventBus.subscribe("frame", (frame: Frame) => {
      this.state.currentTime += 1;
      this.state.duration = 100;
      this.state.isPlaying = frame.is_playing;

      const event: PlaybackUpdateEvent = {
        isPlaying: this.state.isPlaying,
        currentTime: this.state.currentTime,
        duration: this.state.duration,
      };
      this.eventBus.emit("playback-update", event);
    });
  }

  private provideDependencies() {
    provide("eventBus", this.eventBus);
    provide("socketHandler", this.socketHandler);
  }
}

export default PlayerMFE;
