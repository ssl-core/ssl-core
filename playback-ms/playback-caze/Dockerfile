FROM mcr.microsoft.com/devcontainers/go:1.22 AS devcontainer

RUN set -x && \
    apt update && apt upgrade -y && \
    \
    apt install libsodium-dev libczmq-dev protobuf-compiler -y && \
    \
    : # last line

RUN set -x && \
    go install google.golang.org/protobuf/cmd/protoc-gen-go@v1.28 && \
    go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@v1.2 && \
    \
    : # last line

COPY /protocols /tmp/protocols

FROM devcontainer AS build

WORKDIR /app

COPY playback-ms/playback-caze .

RUN make setup
RUN make build-linux

FROM scratch AS prod

COPY --from=build /usr/share/zoneinfo /usr/share/zoneinfo
COPY --from=build /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/
COPY --from=build /etc/passwd /etc/passwd
COPY --from=build /etc/group /etc/group

COPY --from=build /app/bin/playback-ms .

EXPOSE 4040

ENTRYPOINT ["./playback-ms"]
