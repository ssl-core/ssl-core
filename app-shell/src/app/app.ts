import Router from "./lib/router";
import EventBus from "./lib/event-bus";
import ShortcutsHandler from "./lib/shortcuts-handler";
import IconsHandler from "./lib/icons-handler";

class App {
  private router: Router;
  private shorcutsHandler: ShortcutsHandler;
  private eventBus: EventBus;
  private iconsHandler: IconsHandler;

  constructor(
    routes: Route[],
    templates: Record<string, Template>,
    shortcuts: Record<string, Shortcut>,
    icons: Record<string, Icon>
  ) {
    this.router = new Router(routes, templates);
    this.eventBus = new EventBus();
    this.shorcutsHandler = new ShortcutsHandler(this.eventBus, shortcuts);
    this.iconsHandler = new IconsHandler(icons);
  }

  initialize() {
    this.router.initialize();
    this.eventBus.initialize();
    this.shorcutsHandler.initialize();
    this.iconsHandler.initialize();
  }
}

export default App;
