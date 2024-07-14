FROM robocin/ssl-core-go-base:latest AS devcontainer

COPY /protocols /tmp/protocols

FROM devcontainer AS build

COPY common/golang /common/golang
COPY gateway/gateway-augusto /gateway/gateway-augusto

WORKDIR /gateway/gateway-augusto

RUN make setup
RUN make build-linux

RUN mkdir /prod && \
    ldd bin/gateway | awk 'NF == 4 {print $3}; NF == 2 {print $1}' | grep '^/' | xargs -I {} cp --parents {} /prod

FROM gcr.io/distroless/static-debian12 AS prod

COPY --from=build /prod /
COPY --from=build /gateway/gateway-augusto/bin/gateway .

ENTRYPOINT ["./gateway"]
