import Router from "./lib/router";
import EventBus from "./lib/event_bus";
import ShortcutsHandler from "./lib/shortcuts_handler";

class App {
  private router: Router;
  private shorcutsHandler: ShortcutsHandler;
  private eventBus: EventBus;

  constructor(routes: Route[]) {
    this.router = new Router(routes);
    this.eventBus = new EventBus();
    this.shorcutsHandler = new ShortcutsHandler(this.eventBus);
  }

  initialize() {
    this.router.initialize();
    this.eventBus.initialize();
    this.shorcutsHandler.initialize();
  }
}

export default App;
