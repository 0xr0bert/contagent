FROM alpine:3.20 AS builder

RUN apk add cmake boost1.84-dev glog-dev nlohmann-json ninja-build clang18 \
    pkgconfig zstd-dev ninja-is-really-ninja

COPY . /usr/src/contagent

RUN cd /usr/src/contagent; rm -rf build; CXX=/usr/bin/clang++-18 CC=/usr/bin/clang-18 cmake -S . -B build -G Ninja; cd build; ninja

FROM alpine:3.20 AS runtime

RUN apk add boost1.84 glog nlohmann-json zstd

COPY --from=builder /usr/src/contagent/build/contagent-bin /usr/local/bin/contagentsim

COPY --from=builder /usr/src/contagent/build/libcontagent.so /usr/local/lib/libcontagent.so

ENTRYPOINT ["contagentsim"]