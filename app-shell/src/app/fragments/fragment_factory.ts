import BaseFragment from "./base_fragment";
import IframeFragment from "./iframe_fragment";
import WebComponentFragment from "./web_component_fragment";

class FragmentFactory {
  createFragment(
    container: HTMLElement,
    routeFragment: RouteFragment
  ): BaseFragment {
    switch (routeFragment.type) {
      case "webcomponent":
        return new WebComponentFragment(
          routeFragment.url,
          container,
          routeFragment.metadata
        );
      case "iframe":
        return new IframeFragment(
          routeFragment.url,
          container,
          routeFragment.metadata
        );
    }

    throw new Error(`Unknown fragment type: ${routeFragment.type}`);
  }
}

export default FragmentFactory;
