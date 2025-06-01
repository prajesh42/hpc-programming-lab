# HPC Disease Simulation

This is the template project for the HPC-05 Programming Lab project **"Disease Simulation"**.

## Requirements

- Docker must be installed on your system.

## Instructions to Run the Project

1. Build the Docker image:
    ```bash
    docker build -t <simulation_name> .
    ```

2. Go to the directory where your `disease_in.ini` file is located:
    ```bash
    cd /path/to/disease/config
    ```

3. Run the simulation using Docker:
    ```bash
    docker run -v .:/scratch <simulation_name>
    ```

## Notes

- Replace `<simulation_name>` with a name of your choice for the Docker image.
- Make sure the `disease_in.ini` configuration file is present in the current working directory when you run the container.
