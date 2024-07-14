# viewer-mfe

This folder contains the source code for the `Viewer` microfrontend of SSL Core. It is one of the microfrontends that make up the project, developed, tested, and deployed independently of other microfrontends. It is composed at runtime in the browser through the [`app-shell`](https://github.com/robocin/ssl-core/app-shell).

## About

The `Viewer` microfrontend is responsible for showing the SSL match. It is built as a web component that is seamlessly injected into the `app-shell`. This component is in charge of rendering the game match and facilitating bidirectional communication with the server.

## Development

To run the development server, use the following commands:

```bash
yarn
yarn start
```

### Codemap

- [`public`](public): Static files for the microfrontend.
- [`src`](src): Source code for the microfrontend.
- [`src/app`](src/app): Core of application development.
- [`src/app/engines`](src/app/engines): Rendering engines to the game match.
- [`src/app/socket`](src/app/socket): Socket communication with the server.
- [`src/app/viewer-mfe.ts`](src/app/viewer-mfe.ts): Declaration of the web component.
- [`src/main.ts`](src/main.ts): Entry point of the microfrontend.
