class Router {
  private routes: Route[];

  constructor(routes: Route[]) {
    this.routes = routes;
  }

  initialize() {
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
      const app = document.querySelector<HTMLDivElement>("#app")!;
      app.innerHTML = "";

      Object.values(route.components).forEach((component) => {
        const section = document.createElement("section");
        section.setAttribute(
          "rows",
          `${component.rows.start}-${component.rows.end}`
        );
        section.setAttribute(
          "cols",
          `${component.cols.start}-${component.cols.end}`
        );
        app.appendChild(section);

        if (component.type === "webcomponent") {
          const script = document.createElement("script");
          script.type = "module";
          script.src = component.url;
          script.onload = () => {
            const element = document.createElement(component.metadata.tag);
            section.appendChild(element);
          };

          document.body.appendChild(script);
        }
      });
    };

    document.querySelector<HTMLDivElement>("#pages")!.appendChild(page);
  }
}

export default Router;
