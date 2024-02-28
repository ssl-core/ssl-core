import { beforeAll, describe, expect, it } from "vitest";

import IconsHandler from "../../../src/app/lib/icons-handler";

describe("IconsHandler", () => {
  let iconsHandler: IconsHandler;
  let icons: Record<string, Icon> = {
    home: "<svg></svg>",
  };

  beforeAll(() => {
    iconsHandler = new IconsHandler(icons);
  });

  it("should replace icons", () => {
    const icon = document.createElement("i");
    icon.dataset.icon = "home";
    document.body.appendChild(icon);

    iconsHandler.initialize();

    expect(document.querySelector("i")).toBeNull();
    expect(document.querySelector("svg")).not.toBeNull();
  });
});
