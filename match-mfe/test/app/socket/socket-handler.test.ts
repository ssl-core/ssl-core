import { describe, expect, it } from "vitest";

import SocketHandler from "../../../src/app/socket/socket-handler";

describe("SocketHandler", () => {
  it("should define class", () => {
    expect(SocketHandler).toBeDefined();
  });
});
