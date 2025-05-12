#include "simulation.h"

#include <iostream>

Simulation::Simulation(std::string in_file) { input_file = in_file; }

void Simulation::start() { std::cout << "Starting simulation..." << std::endl; 

    //[global]
    std::string simulation_name = "disease_simulation";
    int simulation_runs = 5;

    //[disease]
    std::string disease_name = "Corona";
    int disease_duration = 2;
    int transmissibility = 1.0f;

    //[population]
    int population_size = 20;
    float vaccination_rate = 0.9;
    bool patient_0 = true;

    int steps = run(population_size, disease_name, disease_duration, transmissibility);
    std::cout << "total_steps , " << steps << std::endl;
}

int Simulation::run(int num_pop, std::string disease_name, int duration, float trans) {
    Population populace(num_pop);
    Disease flu(duration, trans);
    flu.disease_name() = disease_name;
    populace.get_people()[0].infect(flu);
    for (int step = 1; ; ++step) {
        if (populace.count_infected() == 0) {
            return step;
        }
        populace.one_more_day();
        populace.infect_neighbors();
    }
    return 0;
}
