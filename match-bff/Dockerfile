FROM golang:1.22

ARG SOURCE_DIRECTORY

COPY /protocols /tmp/protocols

WORKDIR /app

COPY ./${SOURCE_DIRECTORY} .

RUN make setup
RUN make build

EXPOSE 8080

CMD ["./bin/match-bff"]
