FROM robocin/ssl-core-go-base:latest AS devcontainer

COPY /protocols /tmp/protocols

FROM devcontainer AS build

COPY common/golang /common/golang
COPY player-bff/player-sonson /player-bff/player-sonson

WORKDIR /player-bff/player-sonson

RUN make setup
RUN make build-linux

FROM scratch AS prod

COPY --from=build /player-bff/player-sonson/bin/player .

ENTRYPOINT ["./player"]
