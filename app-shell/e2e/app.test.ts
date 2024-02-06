import { test, expect } from "@playwright/test";

test.describe("App", () => {
  test.beforeEach(async ({ page }) => {
    await page.goto("/");
  });

  test("should display the correct title", async ({ page }) => {
    await expect(page).toHaveTitle(/RobôCIn-INES/);
  });

  test("should render root page", async ({ page }) => {
    await expect(page.locator("h1")).toHaveText(/Application Shell/);
  });

  test("should render the navigation", async ({ page }) => {
    await expect(page.locator("nav")).toBeVisible();
  });

  test("should render at least 3 navigation links", async ({ page }) => {
    const count = await page.locator("nav").locator("a").count();
    expect(count).toBeGreaterThan(2);
  });
});
