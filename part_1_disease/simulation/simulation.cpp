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
    std::string location_name = "Deggendorf";
    int population_size = 200;
    float vaccination_rate = 0.1;
    bool patient_0 = true;

    run(simulation_runs, population_size, disease_name, disease_duration, transmissibility, location_name, vaccination_rate, patient_0);
}

void Simulation::run(int sim_runs, int population_size, std::string disease_name, int duration, float trans, std::string location_name, float vac_rate, bool patient_0) {
    
    while(sim_runs-->0) {
        Population populace(population_size);
        Disease disease(duration, trans);

        if(patient_0) {
            populace.get_people()[0].direct_infection(disease);
        }else {
            populace.random_infection(1, disease);
        }

        int vaccinated = vac_rate * population_size;
        populace.random_vaccination(vaccinated);

        int total_infected = 0;
        int step = 0;

        for (step = 0; ; ++step) {
            int infected_count = populace.count_infected();
            if (infected_count == 0) {
                break;
            }
            populace.one_more_day();
        }

        int healthy = populace.count_healthy();
        int recovered = population_size - healthy - vaccinated;

        std::cout << std::endl;
        std::cout << "......... disease_stats (" << disease_name << ") ........." << std::endl;
        std::cout << "        key         , value " << std::endl;
        std::cout << "total_steps         , " << step << std::endl;
        std::cout << "susceptible_persons , " << healthy << std::endl;
        std::cout << "recovered_persons   , " << recovered << std::endl;
        std::cout << "vaccinated_persons  , " << vaccinated << std::endl;
    }
}
