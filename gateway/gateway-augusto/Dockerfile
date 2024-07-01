FROM robocin/ssl-core-go-base:latest AS devcontainer

COPY /protocols /tmp/protocols

FROM devcontainer AS build

WORKDIR /app

COPY gateway/gateway-augusto .

RUN make setup
RUN make build-linux

FROM scratch AS prod

COPY --from=build /usr/share/zoneinfo /usr/share/zoneinfo
COPY --from=build /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/
COPY --from=build /etc/passwd /etc/passwd
COPY --from=build /etc/group /etc/group

COPY --from=build /app/bin/gateway .

EXPOSE 4040

ENTRYPOINT ["./gateway"]
