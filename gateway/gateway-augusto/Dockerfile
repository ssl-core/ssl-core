FROM robocin/ssl-core-go-base:latest AS devcontainer

COPY /protocols /tmp/protocols

FROM devcontainer AS build

WORKDIR /app

COPY gateway/gateway-augusto .

RUN make setup
RUN make build-linux

RUN mkdir /prod && \
    ldd bin/playback-ms | awk 'NF == 4 {print $3}; NF == 2 {print $1}' | grep '^/' | xargs -I {} cp --parents {} /prod

FROM scratch AS prod

COPY --from=build /prod /
COPY --from=build /app/bin/gateway .

ENTRYPOINT ["./gateway"]
