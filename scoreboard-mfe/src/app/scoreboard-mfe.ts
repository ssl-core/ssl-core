import CommunicationHandler from "../communication/communication-handler";
import Channels from "../config/channels";
import { html } from "../utils/literals";
import {
  formatAvatar,
  formatGameStage,
  formatScore,
  formatTime,
} from "../utils/patterns";

import styles from "./styles/main.css?inline";

type ScoreboardMFEElements = {
  homeTeamName: HTMLSpanElement | null;
  homeTeamLogo: HTMLDivElement | null;
  awayTeamName: HTMLSpanElement | null;
  awayTeamLogo: HTMLDivElement | null;
  scorePeriod: HTMLSpanElement | null;
  scoreResult: HTMLSpanElement | null;
  scoreTime: HTMLSpanElement | null;
  eventList: HTMLLIElement | null;
};

class ScoreboardMFE extends HTMLElement {
  private root: ShadowRoot;
  private communicationHandler: CommunicationHandler | null;
  private appChannel: BroadcastChannel;
  private elements: ScoreboardMFEElements;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.communicationHandler = null;
    this.appChannel = new BroadcastChannel(Channels.App);
    this.elements = {
      awayTeamLogo: null,
      awayTeamName: null,
      eventList: null,
      homeTeamLogo: null,
      homeTeamName: null,
      scorePeriod: null,
      scoreResult: null,
      scoreTime: null,
    };
  }

  public connectedCallback() {
    this.communicationHandler = new CommunicationHandler();
    this.communicationHandler.initialize();

    this.appChannel.addEventListener("message", (event) =>
      this.handleMessage(event.data)
    );

    this.render();
  }

  public disconnectedCallback() {
    this.communicationHandler?.terminate();
  }

  public render() {
    this.shadowRoot!.innerHTML = html`
      <div class="scoreboard">
        <div class="scoreboard__header">
          <div class="scoreboard__team scoreboard__team--home">
            <span class="scoreboard__team-name">...</span>
            <div class="scoreboard__team-logo scoreboard__team-logo--home">
              ...
            </div>
          </div>
          <div class="scoreboard__score">
            <span class="scoreboard__score-period"></span>
            <span class="scoreboard__score-result">-:-</span>
            <span class="scoreboard__score-time"></span>
          </div>
          <div class="scoreboard__team scoreboard__team--away">
            <span class="scoreboard__team-name">...</span>
            <div class="scoreboard__team-logo scoreboard__team-logo--away">
              ...
            </div>
          </div>
        </div>
        <div class="scoreboard__events">
          <span class="scoreboard__events-title">Events</span>
          <ul class="scoreboard__events-list">
            <li class="scoreboard__event">
              <p class="scoreboard__event-blank">No event registered</p>
            </li>
          </ul>
        </div>
      </div>
      <style>
        ${styles}
      </style>
    `;

    this.elements.awayTeamLogo = this.root.querySelector(
      ".scoreboard__team-logo--away"
    )!;
    this.elements.awayTeamName = this.root.querySelector(
      ".scoreboard__team--away > .scoreboard__team-name"
    )!;
    this.elements.eventList = this.root.querySelector(
      ".scoreboard__events-list"
    )!;
    this.elements.homeTeamLogo = this.root.querySelector(
      ".scoreboard__team-logo--home"
    )!;
    this.elements.homeTeamName = this.root.querySelector(
      ".scoreboard__team--home > .scoreboard__team-name"
    )!;
    this.elements.scorePeriod = this.root.querySelector(
      ".scoreboard__score-period"
    )!;
    this.elements.scoreResult = this.root.querySelector(
      ".scoreboard__score-result"
    )!;
    this.elements.scoreTime = this.root.querySelector(
      ".scoreboard__score-time"
    )!;
  }

  private handleMessage(frame: Frame) {
    this.elements.awayTeamLogo!.textContent = formatAvatar(frame.awayTeam.name);
    this.elements.awayTeamName!.textContent = frame.awayTeam.name;
    this.elements.homeTeamLogo!.textContent = formatAvatar(frame.homeTeam.name);
    this.elements.homeTeamName!.textContent = frame.homeTeam.name;
    this.elements.scorePeriod!.textContent = formatGameStage(frame.gameStage);
    this.elements.scoreResult!.textContent = formatScore(
      frame.homeTeam.score,
      frame.awayTeam.score
    );
    this.elements.scoreTime!.textContent = formatTime(frame.gameStageTimeLeft);
  }
}

export default ScoreboardMFE;
