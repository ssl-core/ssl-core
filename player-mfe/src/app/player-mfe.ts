import SocketHandler from "../lib/socket/socket-handler";
import Playback from "../entities/playback";
import { provide } from "./services/global-provider";
import { html } from "../utils/literals";

import styles from "./styles/main.css?inline";

import "./components/player-mfe-slider";
import "./components/player-mfe-controls";
import "./components/player-mfe-info";
import "./components/player-mfe-actions";

class PlayerMFE extends HTMLElement {
  private root: ShadowRoot;
  private socketHandler: SocketHandler;
  private playback: Playback;

  constructor() {
    super();
    this.root = this.attachShadow({ mode: "open" });
    this.socketHandler = new SocketHandler("player-bff");
    this.playback = new Playback(this.socketHandler);
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
        <player-mfe-slider></player-mfe-slider>
        <div class="container__controls">
          <player-mfe-info></player-mfe-info>
          <player-mfe-controls></player-mfe-controls>
          <player-mfe-actions></player-mfe-actions>
        </div>
      </div>
      <style>
        ${styles}
      </style>
    `;
  }

  private listenToEvents() {
    this.socketHandler.connect();
  }

  private provideDependencies() {
    provide("playback", this.playback);
  }
}

export default PlayerMFE;
