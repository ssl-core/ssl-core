import indexTemplate from "../templates/index.html?raw";
import shortcutsTemplate from "../templates/shortcuts.html?raw";
import settingsTemplate from "../templates/settings.html?raw";

const templates = {
  "/": indexTemplate,
  "/shortcuts": shortcutsTemplate,
  "/settings": settingsTemplate,
};

export default templates;
