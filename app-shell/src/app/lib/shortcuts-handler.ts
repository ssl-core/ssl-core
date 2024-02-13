import EventBus from "./event-bus";

class ShortcutsHandler {
  private eventBus: EventBus;
  private shortcuts: Record<string, Shortcut>;

  constructor(eventBus: EventBus, shortcuts: Record<string, Shortcut>) {
    this.eventBus = eventBus;
    this.shortcuts = shortcuts;
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