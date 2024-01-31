import FragmentFactory from "../fragments/fragment_factory";

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
  }

  renderCurrentPage() {
    const path = new URL(window.location.href).pathname;

    if (Object.keys(this.templates).includes(path)) {
      this.renderTemplate(path);
      return;
    }

    const route = this.routes.find((route) => route.path === path);
    if (route) {
      this.renderRoute(route);
      return;
    }

    history.pushState({}, "", "/");
    this.renderTemplate("/");
  }

  createTemplates() {
    Object.keys(this.templates).forEach((template) => {
      const page = document.querySelector<HTMLElement>(`[href="${template}"]`)!;
      page.onclick = (event) => {
        event.preventDefault();
        this.renderTemplate(template);
      };
    });
  }

  renderTemplate(template: string) {
    const app = document.querySelector<HTMLDivElement>("#app")!;
    app.innerHTML = "";

    window.history.pushState({}, "", template);

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
    const page = document.createElement("a");
    page.href = route.path;
    page.innerHTML = `
      <li>
        <i data-lucide="${route.icon}"></i>
        <span class="tooltip">${route.label}</span>
      </li>
    `;

    page.onclick = (event) => {
      event.preventDefault();
      this.renderRoute(route);
    };

    document.querySelector<HTMLDivElement>("#pages")!.appendChild(page);
  }

  renderRoute(route: Route) {
    const app = document.querySelector<HTMLDivElement>("#app")!;
    app.innerHTML = "";

    window.history.pushState({}, "", route.path);

    app.style.gridTemplateRows = route.rows.toString();
    app.style.gridTemplateColumns = route.cols.toString();

    Object.values(route.fragments).forEach((routeFragment) => {
      const section = document.createElement("section");
      section.style.gridArea = `${routeFragment.rows.start} / ${routeFragment.cols.start} / ${routeFragment.rows.end} / ${routeFragment.cols.end}`;
      app.appendChild(section);

      const fragmentFactory = new FragmentFactory();

      const fragment = fragmentFactory.createFragment(section, routeFragment);
      fragment.render();
    });
  }
}

export default Router;
