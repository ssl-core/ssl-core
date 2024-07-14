import Playback from "../../entities/playback";
import { PlaybackUpdateEvent } from "../../events/playback-update";
import { inject } from "../services/global-provider";
import { html } from "../../utils/literals";
import { formatTime } from "../../utils/time";

type PlayerMFESliderState = {
  currentTime: number;
  duration: number;
};

type PlayerMFESliderElements = {
  container: HTMLDivElement | null;
  duration: HTMLSpanElement | null;
  currentTime: HTMLSpanElement | null;
  slider: HTMLInputElement | null;
  tooltip: HTMLSpanElement | null;
};

class PlayerMFESlider extends HTMLElement {
  private state: PlayerMFESliderState;
  private elements: PlayerMFESliderElements;
  private playback: Playback;

  constructor() {
    super();
    this.state = {
      currentTime: 0,
      duration: 0,
    };
    this.elements = {
      container: null,
      duration: null,
      currentTime: null,
      slider: null,
      tooltip: null,
    };
    this.playback = inject<Playback>("playback")!;
  }

  public connectedCallback() {
    this.render();
    this.initializeElements();
    this.playback.addEventListener("update", this.handlePlaybackUpdate);
  }

  public disconnectedCallback() {
    this.elements.slider!.removeEventListener("input", this.handleInput);
    this.playback.removeEventListener("update", this.handlePlaybackUpdate);
  }

  public render() {
    this.innerHTML = html`
      <div class="slider">
        <span class="slider__current-time">
          ${formatTime(this.state.currentTime)}
        </span>
        <div class="slider__container">
          <input
            class="slider__seek-bar"
            type="range"
            value="${this.state.currentTime}"
          />
          <span class="slider__tooltip">00:00</span>
        </div>
        <span class="slider__duration">${formatTime(this.state.duration)}</span>
      </div>
    `;

    this.elements.container = this.querySelector<HTMLDivElement>(".slider");
    this.elements.duration =
      this.querySelector<HTMLSpanElement>(".slider__duration");
    this.elements.currentTime = this.querySelector<HTMLSpanElement>(
      ".slider__current-time"
    );
    this.elements.slider =
      this.querySelector<HTMLInputElement>(".slider__seek-bar");
    this.elements.tooltip =
      this.querySelector<HTMLSpanElement>(".slider__tooltip");
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
    this.elements.slider!.addEventListener("mousemove", this.handleMouseMove);
    this.elements.slider!.setAttribute("max", this.state.duration.toString());
    this.elements.currentTime!.textContent = formatTime(this.state.currentTime);
    this.elements.duration!.textContent = formatTime(this.state.duration);
  }

  private handlePlaybackUpdate = (event: PlaybackUpdateEvent) => {
    this.setDuration(event.duration);
    this.setCurrentTime(event.currentTime);
  };

  private handleInput = (event: Event) => {
    const target = event.target as HTMLInputElement;
    this.setCurrentTime(parseInt(target.value));
    this.playback.seek(parseInt(target.value));
  };

  private handleMouseMove = (event: MouseEvent) => {
    const rect = this.elements.slider!.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const width = rect.width;
    const percentage = x / width;

    this.elements.tooltip!.textContent = formatTime(
      this.state.duration * percentage
    );

    const offsetX = this.elements.container!.getBoundingClientRect().left;
    this.elements.tooltip!.style.left = `${event.clientX - offsetX}px`;
  };
}

customElements.define("player-mfe-slider", PlayerMFESlider);

export default PlayerMFESlider;
