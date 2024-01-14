import BaseFragment from "../fragments/base_fragment";
import IframeFragment from "../fragments/iframe_fragment";
import WebComponentFragment from "../fragments/web_component_fragment";

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

      Object.values(route.fragments).forEach((routeFragment) => {
        const section = document.createElement("section");
        section.style.gridArea = `${routeFragment.rows.start} / ${routeFragment.cols.start} / ${routeFragment.rows.end} / ${routeFragment.cols.end}`;
        app.appendChild(section);

        const fragment = this.buildFragment(routeFragment, section);
        fragment.render();
      });
    };

    document.querySelector<HTMLDivElement>("#pages")!.appendChild(page);
  }

  buildFragment(fragment: RouteFragment, section: HTMLElement): BaseFragment {
    if (fragment.type === "webcomponent") {
      return new WebComponentFragment(fragment.url, section, fragment.metadata);
    } else if (fragment.type === "iframe") {
      return new IframeFragment(fragment.url, section, fragment.metadata);
    }

    throw new Error(`Unknown fragment type: ${fragment.type}`);
  }
}

export default Router;
