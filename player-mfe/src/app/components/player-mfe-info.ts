import Playback from "../../entities/playback";
import { PlaybackUpdateEvent } from "../../events/playback-update";
import { html } from "../../utils/literals";
import { inject } from "../services/global-provider";

type PlayerMFEInfoElements = {
  liveButton: HTMLButtonElement | null;
};

class PlayerMFEInfo extends HTMLElement {
  private playback: Playback;
  private elements: PlayerMFEInfoElements;

  constructor() {
    super();
    this.playback = inject<Playback>("playback")!;
    this.elements = {
      liveButton: null,
    };
  }

  public connectedCallback() {
    this.render();
    this.playback.addEventListener("update", this.handlePlaybackUpdate);
    this.elements.liveButton!.addEventListener("click", this.handleLiveClick);
  }

  public disconnectedCallback() {
    this.playback.removeEventListener("update", this.handlePlaybackUpdate);
    this.elements.liveButton!.removeEventListener(
      "click",
      this.handleLiveClick
    );
  }

  public render() {
    this.innerHTML = html`
      <div class="info">
        <button
          id="live-button"
          class="info__live-button"
          aria-label="Toggle live"
        >
          <div class="info__live-button__circle"></div>
          <span class="info__live-button__text">Live</span>
        </button>
      </div>
    `;

    this.elements.liveButton =
      this.querySelector<HTMLButtonElement>("#live-button")!;
  }

  private handleLiveClick = () => {
    this.playback.live();
  };

  private handlePlaybackUpdate = (event: PlaybackUpdateEvent) => {
    this.elements.liveButton!.classList.remove("live");
    this.elements.liveButton!.classList.remove("loading");

    if (event.isPlaying && event.isLive) {
      this.elements.liveButton!.classList.add("live");
    }

    if (event.isLoading) {
      this.elements.liveButton!.classList.add("loading");
    }
  };
}

customElements.define("player-mfe-info", PlayerMFEInfo);

export default PlayerMFEInfo;
