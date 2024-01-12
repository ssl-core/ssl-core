import Router from "./router";

class App {
  private router: Router;

  constructor(routes: Route[]) {
    this.router = new Router(routes);
  }

  initialize() {
    this.router.initialize();
  }
}

export default App;
