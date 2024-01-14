import EventBus from "./event_bus";

class ShortcutsHandler {
  private eventBus: EventBus;
  private shortcuts: { [key: string]: { eventName: string; data: object } };

  constructor(eventBus: EventBus) {
    this.eventBus = eventBus;
    this.shortcuts = {
      s: { eventName: "save", data: {} },
      d: { eventName: "delete", data: {} },
    };
  }

  initialize() {
    document.addEventListener("keydown", (e) => {
      if (e.ctrlKey && Object.keys(this.shortcuts).includes(e.key)) {
        e.preventDefault();
        const { eventName, data } = this.shortcuts[e.key];
        this.eventBus.publish(eventName, data);
      }
    });
  }

  registerShortcut(key: string, eventName: string, data: object) {
    this.shortcuts[key] = { eventName, data };
  }
}

export default ShortcutsHandler;
