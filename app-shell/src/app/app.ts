import Router from "./lib/router";
import ShortcutsHandler from "./lib/shortcuts_handler";

class App {
  private router: Router;
  private shorcutsHandler: ShortcutsHandler;

  constructor(routes: Route[]) {
    this.router = new Router(routes);
    this.shorcutsHandler = new ShortcutsHandler();
  }

  initialize() {
    this.router.initialize();
    this.shorcutsHandler.initialize();
  }
}

export default App;
