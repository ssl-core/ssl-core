import EventBus from "./event-bus";

class ShortcutsHandler {
  private eventBus: EventBus;
  private shortcuts: Record<string, Shortcut>;

  constructor(eventBus: EventBus, shortcuts: Record<string, Shortcut>) {
    this.eventBus = eventBus;
    this.shortcuts = shortcuts;
  }

  initialize() {
    document.addEventListener("keydown", (event) => {
      if (event.ctrlKey && Object.keys(this.shortcuts).includes(event.key)) {
        event.preventDefault();
        const { eventName, data } = this.shortcuts[event.key];
        this.eventBus.publish(eventName, data);
      }
    });
  }

  registerShortcut(key: string, eventName: string, data: object) {
    this.shortcuts[key] = { eventName, data };
  }
}

export default ShortcutsHandler;
