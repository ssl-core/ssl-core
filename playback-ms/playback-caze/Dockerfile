FROM robocin/ssl-core-go-base:latest AS devcontainer

COPY /protocols /tmp/protocols

FROM devcontainer AS build

WORKDIR /app

COPY playback-ms/playback-caze .

RUN make setup
RUN make build-linux

RUN mkdir /prod && \
    ldd bin/playback-ms | awk 'NF == 4 {print $3}; NF == 2 {print $1}' | grep '^/' | xargs -I {} cp --parents {} /prod

FROM gcr.io/distroless/static-debian12 AS prod

COPY --from=build /prod /
COPY --from=build /app/bin/playback-ms .

ENTRYPOINT ["./playback-ms"]