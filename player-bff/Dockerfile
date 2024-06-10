FROM golang:1.22 AS build

ARG SOURCE_DIRECTORY

COPY /protocols /tmp/protocols

WORKDIR /app

COPY ./${SOURCE_DIRECTORY} .

RUN make setup
RUN make build-linux

FROM scratch

COPY --from=build /usr/share/zoneinfo /usr/share/zoneinfo
COPY --from=build /etc/ssl/certs/ca-certificates.crt /etc/ssl/certs/
COPY --from=build /etc/passwd /etc/passwd
COPY --from=build /etc/group /etc/group

COPY --from=build /app/bin/player-bff .

EXPOSE 4040

CMD ["./player-bff"]
