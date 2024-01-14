class ShortcutsHandler {
  private shortcuts: { [key: string]: string };

  constructor() {
    this.shortcuts = {
      s: "save",
      d: "delete",
      e: "edit",
      n: "new",
    };
  }

  initialize() {
    document.addEventListener("keydown", (e) => {
      if (e.ctrlKey && Object.keys(this.shortcuts).includes(e.key)) {
        e.preventDefault();
        const eventName = this.shortcuts[e.key];
        console.log(`Triggering event ${eventName}`);
      }
    });
  }

  registerShortcut(key: string, eventName: string) {
    this.shortcuts[key] = eventName;
  }
}

export default ShortcutsHandler;
