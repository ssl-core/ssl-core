FROM robocin/ssl-core-go-base:latest AS devcontainer

FROM devcontainer AS build

COPY protocols /protocols
COPY common/golang /common/golang
COPY player-bff/player-sonson /player-bff/player-sonson

WORKDIR /player-bff/player-sonson

RUN make setup
RUN make build-linux

FROM scratch AS prod

COPY --from=build /player-bff/player-sonson/bin/player .

ENTRYPOINT ["./player"]
