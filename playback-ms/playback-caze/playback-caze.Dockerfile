FROM robocin/ssl-core-go-base:latest AS devcontainer

FROM devcontainer AS build

COPY protocols /protocols
COPY common/golang /common/golang
COPY playback-ms/playback-caze /playback-ms/playback-caze

WORKDIR /playback-ms/playback-caze

RUN make setup
RUN make build-linux

RUN mkdir /prod && \
    ldd bin/playback | awk 'NF == 4 {print $3}; NF == 2 {print $1}' | grep '^/' | xargs -I {} cp --parents {} /prod

FROM gcr.io/distroless/static-debian12 AS prod

COPY --from=build /prod /
COPY --from=build /playback-ms/playback-caze/bin/playback .

ENTRYPOINT ["./playback"]
