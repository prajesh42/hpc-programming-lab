#include "simulation.h"

#include <iostream>

Simulation::Simulation(std::string in_file) { input_file = in_file; }

void Simulation::start() { std::cout << "Starting simulation..." << std::endl; 

    //[global]
    std::string simulation_name = "disease_simulation";
    int simulation_runs = 5;

    //[disease]
    std::string disease_name = "Corona";
    int disease_duration = 5;
    int transmissibility = 1.0f;

    //[population]
    int population_size = 200;
    float vaccination_rate = 0.1;
    bool patient_0 = true;

    run(simulation_runs, population_size, disease_name, disease_duration, transmissibility, vaccination_rate);
}

void Simulation::run(int sim_runs, int num_pop, std::string disease_name, int duration, float trans, float vac_rate) {
    while(sim_runs-->0) {
        Population populace(num_pop);
        Disease flu(duration, trans);
        flu.disease_name() = disease_name;
        populace.random_infection(1, flu);
        int vaccinated = vac_rate * num_pop;
        int total_vaccinated = 0;
        for (int step = 1; ; ++step) {
            int infected_count = populace.count_infected();
            if (infected_count == 0) {
                std::cout << "total_steps , " << step << std::endl;
                break;
            }
            populace.one_more_day();
            populace.infect_neighbors();
            populace.random_interactions(6);
            populace.random_vaccination(vaccinated);

            total_vaccinated += vaccinated;
        }
        std::cout << "susceptible_persons, " << populace.count_healthy() << std::endl;
        std::cout << "recovered_persons, " << populace.count_vaccinated() << std::endl;
        std::cout << "vaccinated_persons, " << total_vaccinated << std::endl;
    }
}
