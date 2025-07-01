# Base image: Ubuntu 24.04 Noble Numbat
FROM ubuntu:24.04 AS build

# Updates and install all the necessary tools for builing the app 
# This will makes sure that re-installation doesn't happen as long as base image doesn't change
RUN apt update && apt install -y \
openmpi-bin \
libopenmpi-dev \
libxrandr-dev \
libxcursor-dev \
libxi-dev \
libudev-dev \
libfreetype-dev \
libflac-dev \
libvorbis-dev \
libgl1-mesa-dev \
libegl1-mesa-dev \
libfreetype-dev \
libeigen3-dev \
cmake \
git \
build-essential \
&& apt clean

WORKDIR /game 
COPY CMakeLists.txt /game
COPY src /game/src

WORKDIR /game/build 
RUN cmake .. && make

WORKDIR /app
RUN cp -r /game/build/game_of_life /app

# Change to a working directory (for temporary files)
WORKDIR /scratch

# Set environment defaults
ENV GOL_NUM_MPI_PROCS=1
ENV ROWS=128
ENV COLS=128
ENV LOOP=1000

# Reads env vars and runs game_of_life using mpirun
ENTRYPOINT ["sh", "-c", "mpirun --allow-run-as-root --oversubscribe -n $GOL_NUM_MPI_PROCS /app/game_of_life -n ${INPUT_FILE:+-f $INPUT_FILE} -r $ROWS -c $COLS -i $LOOP "]