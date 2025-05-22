#include "simulation.h"

#include <iostream>
#include "INIReader.h"
#include <fstream>
#include <iomanip>

Simulation::Simulation(std::string in_file) { input_file = in_file; }

void Simulation::start() {

    INIReader reader(input_file);
    
    if (reader.ParseError() < 0) {
        std::cerr << "Can't load 'disease_in.ini'\n";
        return;
    }

    std::cout << "    Input file: " << input_file << std::endl;

    // Global section
    std::string simulation_name = reader.Get("global", "simulation_name", "");
    int simulation_runs = reader.GetInteger("global", "simulation_runs", 0);

    // Disease section
    std::string disease_name = reader.Get("disease", "name", "");
    int duration = reader.GetInteger("disease", "duration", 0);
    float transmissibility = reader.GetFloat("disease", "transmissibility", 0.0f);

    // Population section
    std::string location_name = reader.Get("population_1", "name", "");
    int population_size = reader.GetInteger("population_1", "size", 0);
    float vaccination_rate = reader.GetFloat("population_1", "vaccination_rate", 0.0f);

    std::cout << "    Simulation name: " << simulation_name << std::endl;
    std::cout << "    Disease name: " << disease_name << std::endl;
    std::cout << "    Total population: " << population_size << std::endl;
    std::cout << "Starting simulation " << simulation_name << std::endl; 

    run(simulation_name, simulation_runs, population_size, duration, transmissibility, location_name, vaccination_rate);
}

void Simulation::run(std::string sim_name, int sim_runs, int population_size, int duration, float trans, std::string location_name, float vac_rate) {
    std::string details_file = "disease_details_" + sim_name + ".csv";
    std::ofstream details(details_file);
    details << "run,name,infectious,recovered,susceptiple,vaccinated\n";
    
    std::vector<int> steps_values;
    std::vector<int> healthy_values;
    std::vector<int> recovered_values;
    std::vector<int> vaccinated_values;

    for(int run = 1; run <= sim_runs; ++run) {
        std::cout << "    Running... " << run << " of " << sim_runs << std::endl;
        Population populace(population_size);
        Disease disease(duration, trans);
        populace.random_infection(1, disease);
        int vaccinated = vac_rate * population_size;
        populace.random_vaccination(vaccinated);

        int step = 0;

        for (step = 0; ; ++step) {
            int infected_count = populace.count_infected();
            int healthy = populace.count_healthy();
            int recovered_combined = populace.count_vaccinated();
            int recovered = recovered_combined - vaccinated;

            details << run << "," << location_name << "," << infected_count << "," 
                    << recovered << "," << healthy
                    << "," << vaccinated << "\n";
            if (infected_count == 0) {
                break;
            }
            populace.one_more_day();
        }

        int healthy = populace.count_healthy();
        int recovered = populace.count_vaccinated() - vaccinated;
        
        steps_values.push_back(step);
        healthy_values.push_back(healthy);
        recovered_values.push_back(recovered);
        vaccinated_values.push_back(vaccinated);
    }

    details.close();

    calc_print(sim_name, sim_runs, steps_values, healthy_values, recovered_values, vaccinated_values);
}

void Simulation::calc_print(std::string sim_name, int sim_runs, std::vector<int>& steps_values, std::vector<int>& healthy_values,
                            std::vector<int>& recovered_values,std::vector<int>& vaccinated_values) {

    std::pair<double,double> steps_pair = Utility::calc_mean_stdev(steps_values);
    std::pair<double,double> healthy_pair = Utility::calc_mean_stdev(healthy_values);
    std::pair<double,double> recovered_pair = Utility::calc_mean_stdev(recovered_values);
    std::pair<double,double> vaccinated_pair = Utility::calc_mean_stdev(vaccinated_values);

    std::cout << std::endl;
    std::cout << "Total statistics for " << sim_runs << " runs:\n";
    std::cout << std::left << std::setw(16) << "    key" 
              << std::right << std::setw(12) << "mean" 
              << std::setw(6) << "std" << std::endl;

    auto print_row = [](const std::string& label, double mean, double stdev, std::ofstream& stats) {
        stats << label << "," << mean << "," << stdev << "\n";
        std::cout << std::left << std::setw(16) << label << ":"
                  << std::right << std::setw(10) << std::fixed << std::setprecision(1) << mean
                  << std::setw(6) << std::fixed << std::setprecision(1) << " ± " << stdev << std::endl;
    };

    std::string stats_file = "disease_stats_" + sim_name + ".csv";
    std::ofstream stats(stats_file);
    stats << std::fixed << std::setprecision(1);
    stats << "key,mean,std\n";

    print_row("avg(steps)", steps_pair.first, steps_pair.second, stats);
    print_row("avg(healthy)", healthy_pair.first, healthy_pair.second, stats);
    print_row("avg(recovered)", recovered_pair.first, recovered_pair.second, stats);
    print_row("avg(vaccinated)", vaccinated_pair.first, vaccinated_pair.second, stats);

    std::cout << std::endl;
    stats.close();
}
