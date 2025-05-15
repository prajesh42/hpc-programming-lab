#include "simulation.h"

#include <iostream>
#include "INIReader.h"
#include <fstream>

Simulation::Simulation(std::string in_file) { input_file = in_file; }

void Simulation::start() { std::cout << "Starting simulation..." << std::endl; 

    INIReader reader(input_file);

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
    std::ofstream details("disease_details.csv");
    std::ofstream stats("disease_stats.csv");
    details << "name,infectious,recovered,susceptiple,vaccinated\n";
    stats << "key,value\n";
    
    while(sim_runs-->0) {
        Population populace(population_size);
        Disease disease(duration, trans);
        populace.random_infection(1, disease);
        int vaccinated = vac_rate * population_size;
        populace.random_vaccination(vaccinated);

        int total_infected = 0;
        int step = 0;

        std::cout << std::endl;
        std::cout << "--------disease_details (" << disease_name << ") --------" << std::endl;
        std::cout << "name,infectious,recovered,susceptiple,vaccinated" << std::endl;

        for (step = 0; ; ++step) {
            int infected_count = populace.count_infected();
            int healthy = populace.count_healthy();
            int recovered = population_size - infected_count - healthy - vaccinated;
            total_infected += infected_count;
            details << location_name << "," << infected_count << "," 
                 << recovered << "," << healthy
                 << "," << vaccinated << "\n";
            std::cout << location_name << "," << infected_count << "," 
                      << recovered << "," << healthy
                      << "," << vaccinated << std::endl;
            if (infected_count == 0) {
                break;
            }
            populace.one_more_day();
        }
        details << "\n";

        int healthy = populace.count_healthy();
        int recovered = population_size - healthy - vaccinated;
        std::cout << std::endl;
        std::cout << "......... disease_stats (" << disease_name << ") ........." << std::endl;
        std::cout << "key,value " << std::endl;
        stats << "total_steps," << step << "\n";
        std::cout << "total_steps," << step << std::endl;
        stats << "susceptible_persons," << healthy << "\n";
        std::cout << "susceptible_persons," << healthy << std::endl;
        stats << "recovered_persons," << recovered << "\n";
        std::cout << "recovered_persons," << recovered << std::endl;
        stats << "vaccinated_persons," << vaccinated << "\n\n";
        std::cout << "vaccinated_persons," << vaccinated << std::endl;
        std::cout << std::endl;
    }
    
    details.close();
    stats.close();
}
