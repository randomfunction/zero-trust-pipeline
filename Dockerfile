FROM ubuntu:22.04
RUN apt-get update && apt-get install -y g++
WORKDIR /app
COPY . .
RUN g++ -O3 -march=native -flto -I include -o engine src/*.cpp
USER 1001
CMD ["./engine"]