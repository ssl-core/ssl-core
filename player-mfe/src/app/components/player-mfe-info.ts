import Playback from "../../entities/playback";
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
    this.elements.liveButton!.addEventListener("click", this.handleLiveClick);
  }

  public disconnectedCallback() {
    this.elements.liveButton!.removeEventListener(
      "click",
      this.handleLiveClick
    );
  }

  public render() {
    this.innerHTML = html`
      <div class="info">
        <button class="info__live-button" aria-label="Toggle live">
          <div class="info__live-button__circle"></div>
          <span class="info__live-button__text">Live</span>
        </button>
      </div>
    `;

    this.elements.liveButton =
      this.querySelector<HTMLButtonElement>(".info__live-button")!;
  }

  private handleLiveClick = () => {
    this.playback.live();
  };
}

customElements.define("player-mfe-info", PlayerMFEInfo);

export default PlayerMFEInfo;
