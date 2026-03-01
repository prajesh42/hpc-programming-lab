# HPC Programming Lab

This repository contains three main projects corresponding to the HPC programming lab assignments.

## 1. Disease Simulation (part_1_disease)

A containerised simulation of disease spread using configuration files. The project is located in `part_1_disease`.

### Requirements
- Docker must be installed on your system.

### Running the Simulation
1. Build the Docker image:
    ```bash
    docker build -t <simulation_name> .
    ```
2. Change to the directory containing your `disease_in.ini` configuration (e.g. `configExample` or one of the example subdirectories):
    ```bash
    cd /path/to/disease/config
    ```
3. Run the container, mounting the current directory as `/scratch`:
    ```bash
    docker run -v .:/scratch <simulation_name>
    ```

> Replace `<simulation_name>` with a name of your choice and ensure the `disease_in.ini` file is present in the working directory when the container is executed.


## 2. Game Of Life (part_2_parallel)

A parallel implementation of Conway's Game of Life featuring an SFML GUI and MPI support located in `part_2_parallel`.

### Prerequisites
- Linux with the following packages installed (see part_2_parallel/README.md for the full list):
  - SFML dependencies (`libxrandr-dev`, `libxcursor-dev` etc.)
  - `openmpi-bin` and `libopenmpi-dev`

- Docker (optional, for containerised runs).

### Build Instructions
```bash
cd part_2_parallel
mkdir build && cd build
cmake ..
make
```

To enable debug output:
```bash
cmake -DENABLE_DEBUG=on ..
make
```

> Debugging is verbose; use small grids (e.g. 16×16) when enabled.

### Running the Application
- Locally with GUI:
  ```bash
  ./game_of_life
  ```
  Controls: `Esc` or window close to exit, arrow keys to adjust frame rate.

- Without UI or with MPI:
  ```bash
  mpirun -n <procs> ./game_of_life [options]
  ```

#### Command-line Options
| Option | Description | Default / Notes |
|--------|-------------|-----------------|
| `-n`, `--no-ui` | Disable GUI | UI enabled by default |
| `-r`, `--global_rows <n>` | Number of grid rows (power of 2) | 124 |
| `-c`, `--global_cols <n>` | Number of grid cols (power of 2) | 124 |
| `-i`, `--iterations <n>` | Iteration count (`-1` for infinite) | -1 |
| `-f`, `--file <filename>` | Load initial state (dimensions must be specified) |
| `--show-grid` | Display MPI process boundaries in UI | disabled |

Examples:
```bash
mpirun -n 4 ./game_of_life -n
./game_of_life -r 256 -c 256
./game_of_life -i 30
./game_of_life -f initial_state.txt
```

#### File Format
Plain text: one line per row, `1` for live cells, `0` (or any other char) for dead cells.

#### Pattern Generation
A helper script `scripts/generate_patterns.py` creates standard patterns. See `part_2_parallel/scripts/README.md`.

### Docker Usage
Same pattern as part 1 with environment variables for configuration.

Build the image:
```bash
cd part_2_parallel
docker build -t <image_name> .
```
Run with volume mount and optional env vars:
```bash
docker run -v .:/scratch \
  -e INPUT_FILE="input.txt" \
  -e GOL_NUM_MPI_PROCS=2 \
  -e ROWS=32 \
  -e COLS=32 \
  -e LOOP=10 \
  <image_name>
```
Default env values:
- `GOL_NUM_MPI_PROCS=1`
- `ROWS=128`, `COLS=128`
- `LOOP=1000`