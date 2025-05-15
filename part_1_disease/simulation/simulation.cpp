#include "simulation.h"

#include <iostream>
#include "INIReader.h"

Simulation::Simulation(std::string in_file) { input_file = in_file; }

void Simulation::start() { std::cout << "Starting simulation..." << std::endl; 

    INIReader reader("../configExample/disease_in.ini");

    if (reader.ParseError() < 0) {
        std::cerr << "Can't load 'disease_in.ini'\n";
        return;
    }

    // Global section
    std::string simulation_name = reader.Get("global", "simulation_name", "");
    int num_populations = reader.GetInteger("global", "num_populations", 0);
    int simulation_runs = reader.GetInteger("global", "simulation_runs", 0);

    // Disease section
    std::string disease_name = reader.Get("disease", "name", "");
    int duration = reader.GetInteger("disease", "duration", 0);
    float transmissibility = reader.GetFloat("disease", "transmissibility", 0.0f);

    // Population section
    std::string location_name = reader.Get("population_1", "name", "");
    int population_size = reader.GetInteger("population_1", "size", 0);
    float vaccination_rate = reader.GetFloat("population_1", "vaccination_rate", 0.0f);
    bool patient_0 = reader.GetBoolean("population_1", "patient_0", false);

    run(simulation_runs, population_size, disease_name, duration, transmissibility, location_name, vaccination_rate, patient_0);
}

void Simulation::run(int sim_runs, int population_size, std::string disease_name, int duration, float trans, std::string location_name, float vac_rate, bool patient_0) {
    
    while(sim_runs-->0) {
        Population populace(population_size);
        Disease disease(duration, trans);
        populace.random_infection(1, disease);
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
