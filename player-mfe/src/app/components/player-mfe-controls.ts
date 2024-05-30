import EventBus from "../../lib/event-bus/event-bus";
import SocketHandler from "../../lib/socket/socket-handler";
import { html } from "../../utils/literals";
import {
  forward5SecIcon,
  forwardIcon,
  pauseIcon,
  playIcon,
  rewind5SecIcon,
  rewindIcon,
} from "../../utils/icons";
import { inject } from "../services/global-provider";
import { PlaybackUpdateEvent } from "../../events/playback-update";

type PlayerMFEControlsElements = {
  playButton: HTMLButtonElement | null;
  rewindButton: HTMLButtonElement | null;
  forwardButton: HTMLButtonElement | null;
  rewind5Button: HTMLButtonElement | null;
  forward5Button: HTMLButtonElement | null;
};

type PlayerMFEControlsState = {
  isPlaying: boolean;
};

class PlayerMFEControls extends HTMLElement {
  private eventBus: EventBus;
  private socketHandler: SocketHandler;
  private elements: PlayerMFEControlsElements;
  private state: PlayerMFEControlsState;

  constructor() {
    super();
    this.eventBus = inject<EventBus>("eventBus")!;
    this.socketHandler = inject<SocketHandler>("socketHandler")!;
    this.elements = {
      playButton: null,
      rewindButton: null,
      forwardButton: null,
      rewind5Button: null,
      forward5Button: null,
    };
    this.state = {
      isPlaying: false,
    };
  }

  public connectedCallback() {
    this.render();
    this.elements.playButton!.addEventListener("click", this.handlePlayClick);
    this.eventBus.subscribe("playback-update", this.handlePlaybackUpdate);
  }

  public disconnectedCallback() {}

  public render() {
    this.innerHTML = html`
      <div class="controls">
        <button
          id="rewind-5-button"
          class="controls__button"
          aria-label="Rewind 5 seconds"
          data-testid="rewind-5-button"
        >
          ${rewind5SecIcon}
        </button>
        <button
          id="rewind-button"
          class="controls__button"
          aria-label="Rewind"
          data-testid="rewind-button"
        >
          ${rewindIcon}
        </button>
        <button
          id="play-button"
          class="controls__play"
          aria-label="Play"
          data-testid="play-button"
        >
          ${playIcon}
        </button>
        <button
          id="forward-button"
          class="controls__button"
          aria-label="Forward"
          data-testid="forward-button"
        >
          ${forwardIcon}
        </button>
        <button
          id="forward-5-button"
          class="controls__button"
          aria-label="Forward 5 seconds"
          data-testid="forward-5-button"
        >
          ${forward5SecIcon}
        </button>
      </div>
    `;

    this.elements = {
      playButton: this.querySelector<HTMLButtonElement>("#play-button"),
      rewindButton: this.querySelector<HTMLButtonElement>("#rewind-button"),
      forwardButton: this.querySelector<HTMLButtonElement>("#forward-button"),
      rewind5Button: this.querySelector<HTMLButtonElement>("#rewind-5-button"),
      forward5Button:
        this.querySelector<HTMLButtonElement>("#forward-5-button"),
    };
  }

  public handlePlayClick = () => {
    if (this.state.isPlaying) {
      this.socketHandler.pauseLiveStream();
      this.state.isPlaying = false;
    } else {
      this.socketHandler.playLiveStream();
      this.state.isPlaying = true;
    }

    this.syncPlayButton();
  };

  public handlePlaybackUpdate = (event: PlaybackUpdateEvent) => {
    if (event.isPlaying === this.state.isPlaying) {
      return;
    }

    this.state.isPlaying = event.isPlaying;
    this.syncPlayButton();
  };

  private syncPlayButton() {
    const icon = this.state.isPlaying ? pauseIcon : playIcon;
    const ariaLabel = this.state.isPlaying ? "Pause" : "Play";
    this.elements.playButton!.innerHTML = icon;
    this.elements.playButton!.setAttribute("aria-label", ariaLabel);
  }
}

customElements.define("player-mfe-controls", PlayerMFEControls);

export default PlayerMFEControls;
