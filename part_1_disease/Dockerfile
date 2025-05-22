# Base image: Ubuntu 24.04 Noble Numbat
FROM debian:bookworm-slim AS build

# Updates and install all the necessary tools for builing the app 
# This will makes sure that re-installation doesn't happen as long as base image doesn't change
RUN apt-get update && apt-get install -y build-essential cmake && apt-get clean

WORKDIR /src 
COPY CMakeLists.txt /src
COPY simulation /src/simulation
COPY include /src/include 

WORKDIR /src/build 
RUN cmake .. && make

# Second stage for deployment, without any build dependencies or source files
FROM debian:bookworm-slim AS run

WORKDIR /app
COPY --from=build /src/build/hpc_disease_simulation /app
COPY --from=build /src/build/test /app

# Run test to make sure everything works well
RUN /app/test

# Change to a working directory (for temporary files)
WORKDIR /scratch

# Execute the simulation on image start
ENTRYPOINT ["/app/hpc_disease_simulation"]
