import { createIcons, icons } from "lucide";

import configRoutes from "./config/routes.yaml";

import App from "./app/app";

import "./styles/variables.css";
import "./styles/defaults.css";
import "./styles/sidebar.css";
import "./styles/app.css";

const routes = Object.values(configRoutes);

const app = new App(routes);
app.initialize();

createIcons({ icons });
