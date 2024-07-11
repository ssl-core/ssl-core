import FragmentFactory from "../fragments/fragment-factory";

class Router {
  private routes: Route[];
  private templates: Record<string, Template>;

  constructor(routes: Route[], templates: Record<string, Template>) {
    this.routes = routes;
    this.templates = templates;
  }

  initialize() {
    this.renderCurrentPage();
    this.createTemplates();
    this.createRoutes();
    this.addPopStateListener();
  }

  renderCurrentPage(pushState = true) {
    const path = new URL(window.location.href).pathname;

    if (Object.keys(this.templates).includes(path)) {
      this.renderTemplate(path, pushState);
      return;
    }

    const route = this.routes.find((route) => route.path === path);
    if (route) {
      this.renderRoute(route, pushState);
      return;
    }

    this.renderTemplate("/", pushState);
  }

  createTemplates() {
    Object.keys(this.templates).forEach((template) => {
      const anchor = document.querySelector<HTMLElement>(
        `[href="${template}"]`
      )!;
      anchor.onclick = (event) => {
        event.preventDefault();
        this.renderTemplate(template);
      };
    });
  }

  renderTemplate(template: string, pushState = true) {
    const app = document.getElementById("app")!;
    app.innerHTML = "";

    if (pushState) {
      window.history.pushState({}, "", template);
    }

    app.style.gridTemplateRows = "1";
    app.style.gridTemplateColumns = "1";

    const section = document.createElement("section");
    section.style.gridArea = "1 / 1 / 2 / 2";
    section.innerHTML = this.templates[template];
    app.appendChild(section);
  }

  createRoutes() {
    this.routes.forEach((route) => this.createRoute(route));
  }

  createRoute(route: Route) {
    const item = document.createElement("li");

    const anchor = document.createElement("a");
    anchor.href = route.path;
    anchor.ariaLabel = route.label;
    anchor.onclick = (event) => {
      event.preventDefault();
      this.renderRoute(route);
    };

    const icon = document.createElement("i");
    icon.dataset.icon = route.icon;

    const tooltip = document.createElement("span");
    tooltip.innerHTML = route.label;
    tooltip.className = "tooltip";

    item.appendChild(anchor);
    anchor.appendChild(icon);
    anchor.appendChild(tooltip);

    document.querySelector("#routes > ul")!.appendChild(item);
  }

  renderRoute(route: Route, pushState = true) {
    const app = document.getElementById("app")!;
    app.innerHTML = "";

    if (pushState) {
      window.history.pushState({}, "", route.path);
    }

    app.style.gridTemplateRows = `repeat(${route.rows}, 1fr)`;
    app.style.gridTemplateColumns = `repeat(${route.cols}, 1fr)`;

    Object.values(route.fragments).forEach((routeFragment) => {
      const section = document.createElement("section");
      section.style.gridArea = `${routeFragment.rows.start} / ${routeFragment.cols.start} / ${routeFragment.rows.end} / ${routeFragment.cols.end}`;
      app.appendChild(section);

      const fragmentFactory = new FragmentFactory();

      const fragment = fragmentFactory.createFragment(section, routeFragment);
      fragment.render();
    });
  }

  addPopStateListener() {
    window.addEventListener("popstate", () => {
      this.renderCurrentPage(false);
    });
  }
}

export default Router;
