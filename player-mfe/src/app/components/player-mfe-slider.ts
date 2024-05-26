import EventBus from "../../lib/event-bus/event-bus";
import { PlaybackUpdateEvent } from "../../events/playback-update";
import { inject } from "../services/global-provider";
import { html } from "../../utils/literals";
import { formatTime } from "../../utils/time";

type PlayerMFESliderState = {
  currentTime: number;
  duration: number;
};

type PlayerMFESliderElements = {
  duration: HTMLSpanElement | null;
  currentTime: HTMLSpanElement | null;
  slider: HTMLInputElement | null;
};

class PlayerMFESlider extends HTMLElement {
  private state: PlayerMFESliderState;
  private elements: PlayerMFESliderElements;
  private eventBus: EventBus;

  constructor() {
    super();
    this.state = {
      currentTime: 0,
      duration: 0,
    };
    this.elements = {
      duration: null,
      currentTime: null,
      slider: null,
    };
    this.eventBus = inject<EventBus>("eventBus")!;
  }

  public connectedCallback() {
    this.render();
    this.initializeElements();
    this.eventBus.subscribe("playback-update", this.handlePlaybackUpdate);
  }

  public disconnectedCallback() {
    this.elements.slider!.removeEventListener("input", this.handleInput);
  }

  public render() {
    this.innerHTML = html`
      <div class="slider">
        <span class="slider__current-time">
          ${formatTime(this.state.currentTime)}
        </span>
        <input
          class="slider__seek-bar"
          type="range"
          value="${this.state.currentTime}"
        />
        <span class="slider__duration">${formatTime(this.state.duration)}</span>
      </div>
    `;

    this.elements.duration =
      this.querySelector<HTMLSpanElement>(".slider__duration");
    this.elements.currentTime = this.querySelector<HTMLSpanElement>(
      ".slider__current-time"
    );
    this.elements.slider =
      this.querySelector<HTMLInputElement>(".slider__seek-bar");
  }

  public setDuration(duration: number) {
    this.state.duration = duration;
    this.elements.slider!.setAttribute("max", this.state.duration.toString());
    this.elements.duration!.textContent = formatTime(this.state.duration);
  }

  public setCurrentTime(currentTime: number) {
    this.state.currentTime = currentTime;
    this.elements.slider!.value = this.state.currentTime.toString();
    this.elements.slider!.setAttribute(
      "aria-valuenow",
      this.state.currentTime.toString()
    );
    this.elements.slider!.style.backgroundSize = `${
      (this.state.currentTime / this.state.duration) * 100
    }% 100%`;
    this.elements.currentTime!.textContent = formatTime(this.state.currentTime);
  }

  private initializeElements() {
    this.elements.slider!.addEventListener("input", this.handleInput);
    this.elements.slider!.setAttribute("max", this.state.duration.toString());
    this.elements.currentTime!.textContent = formatTime(this.state.currentTime);
    this.elements.duration!.textContent = formatTime(this.state.duration);
  }

  private handlePlaybackUpdate = (event: PlaybackUpdateEvent) => {
    this.setCurrentTime(event.currentTime);
    this.setDuration(event.duration);
  };

  private handleInput(event: Event) {
    const target = event.target as HTMLInputElement;
    this.setCurrentTime(parseInt(target.value));
  }
}

customElements.define("player-mfe-slider", PlayerMFESlider);

export default PlayerMFESlider;
