import EventBus from "../../lib/event-bus/event-bus";
import SocketHandler from "../../lib/socket/socket-handler";
import { html } from "../../utils/literals";
import {
  forwardIcon,
  pauseIcon,
  playIcon,
  rewindIcon,
} from "../../utils/icons";
import { inject } from "../services/global-provider";
import { PlaybackUpdateEvent } from "../../events/playback-update";

type PlayerMFEControlsElements = {
  playButton: HTMLButtonElement | null;
  rewindButton: HTMLButtonElement | null;
  forwardButton: HTMLButtonElement | null;
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
      </div>
    `;

    this.elements = {
      playButton: this.querySelector<HTMLButtonElement>("#play-button"),
      rewindButton: this.querySelector<HTMLButtonElement>("#rewind-button"),
      forwardButton: this.querySelector<HTMLButtonElement>("#forward-button"),
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
