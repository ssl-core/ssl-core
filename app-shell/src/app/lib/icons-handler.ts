class IconsHandler {
  private icons: Record<string, Icon>;

  constructor(icons: Record<string, Icon>) {
    this.icons = icons;
  }

  initialize() {
    const iconElements = document.querySelectorAll<HTMLElement>("[data-icon]");
    Array.from(iconElements).forEach((element) => this.replaceIcon(element));
  }

  replaceIcon(element: HTMLElement) {
    const iconName = element.getAttribute("data-icon");
    if (iconName && this.icons[iconName]) {
      element.outerHTML = this.icons[iconName];
    }
  }
}

export default IconsHandler;
