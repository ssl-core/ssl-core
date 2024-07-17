import CommunicationHandler from "../communication/communication-handler";
import Channels from "../config/channels";
import { html } from "../utils/literals";

import styles from "./styles/main.css?inline";

class ScoreboardMFE extends HTMLElement {
  private root: ShadowRoot;
  private communicationHandler: CommunicationHandler | null;
  private appChannel: BroadcastChannel;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.communicationHandler = null;
    this.appChannel = new BroadcastChannel(Channels.App);
  }

  public connectedCallback() {
    this.communicationHandler = new CommunicationHandler();
    this.communicationHandler.initialize();

    this.appChannel.addEventListener("message", (event) => {
      // console.log(event.data);
    });

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
            <span class="scoreboard__team-name">RobôCIn</span>
            <div class="scoreboard__team-logo scoreboard__team-logo--home">
              RC
            </div>
          </div>
          <div class="scoreboard__score">
            <span class="scoreboard__score-period">1st Half</span>
            <span class="scoreboard__score-result">3:1</span>
            <span class="scoreboard__score-time">04:17</span>
          </div>
          <div class="scoreboard__team scoreboard__team--away">
            <span class="scoreboard__team-name">Tigers</span>
            <div class="scoreboard__team-logo scoreboard__team-logo--away">
              TM
            </div>
          </div>
        </div>
        <div class="scoreboard__events">
          <span class="scoreboard__events-title">Events</span>
          <ul class="scoreboard__events-list">
            <li class="scoreboard__event">
              <p class="scoreboard__event-time">(00:31) Goal</p>
              <p class="scoreboard__event-description">
                Goal by
                <span class="scoreboard__player scoreboard__player--home">
                  RC
                </span>
                Robot 1
              </p>
            </li>
            <li class="scoreboard__event">
              <p class="scoreboard__event-time">(01:02) Foul</p>
              <p class="scoreboard__event-description">
                Foul by
                <span class="scoreboard__player scoreboard__player--home">
                  RC
                </span>
                Robot 1 on
                <span class="scoreboard__player scoreboard__player--away">
                  TM
                </span>
                Robot 2
              </p>
            </li>
          </ul>
        </div>
      </div>
      <style>
        ${styles}
      </style>
    `;
  }
}

export default ScoreboardMFE;
