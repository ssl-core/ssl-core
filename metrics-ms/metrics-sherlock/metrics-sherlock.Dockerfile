FROM robocin/ssl-core-go-base:latest AS devcontainer

COPY /protocols /tmp/protocols

FROM devcontainer AS build

COPY common/golang /common/golang
COPY metrics-ms/metrics-sherlock /metrics-ms/metrics-sherlock

WORKDIR /metrics-ms/metrics-sherlock

RUN make setup
RUN make build-linux

RUN mkdir /prod && \
    ldd bin/metrics-ms | awk 'NF == 4 {print $3}; NF == 2 {print $1}' | grep '^/' | xargs -I {} cp --parents {} /prod

FROM gcr.io/distroless/static-debian12 AS prod

COPY --from=build /prod /
COPY --from=build /metrics-ms/metrics-sherlock/bin/metrics .

ENTRYPOINT ["./metrics"]
