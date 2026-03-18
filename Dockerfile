FROM ubuntu:22.04
RUN apt-get update && apt-get install -y g++
WORKDIR /app
COPY main.cpp .
RUN g++ -o engine main.cpp
USER 1001
CMD ["./engine"]