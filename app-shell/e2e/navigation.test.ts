import { test, expect } from "@playwright/test";

test.describe("Navigation", () => {
  test.beforeEach(async ({ page }) => {
    await page.goto("/");
  });

  test("should navigate to the home page", async ({ page }) => {
    await page.click("nav a[href='/']");
    await expect(page.locator("h1")).toHaveText(/Application Shell/);
  });

  test("should navigate to the shortcuts page", async ({ page }) => {
    await page.click("nav a[href='/shortcuts']");
    expect(page.url()).toContain("/shortcuts");
    await expect(page.locator("h1")).toHaveText(/Shortcuts/);
  });

  test("should navigate to the settings page", async ({ page }) => {
    await page.click("nav a[href='/settings']");
    expect(page.url()).toContain("/settings");
    await expect(page.locator("h1")).toHaveText(/Settings/);
  });
});
