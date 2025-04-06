FROM robocin/ssl-core-go-base:latest AS devcontainer

FROM devcontainer AS build

COPY protocols /protocols
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

# THIRD PARTY ADDRESSES
ENV VISION_ADDRESS ${VISION_ADDRESS}
ENV TRACKED_ADDRESS ${TRACKED_ADDRESS}
ENV GC_ADDRESS ${GC_ADDRESS}

# TIME MEASUREMENT CONFIG
ENV INTERCEPT ${INTERCEPT}
ENV WAIT_PACKAGES ${WAIT_PACKAGES}
ENV MEASURE_PACKAGES ${MEASURE_PACKAGES}

ENTRYPOINT ["./gateway"]
