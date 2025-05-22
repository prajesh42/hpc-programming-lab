#ifndef HPC_DISEASE_SIMULATION_H_
#define HPC_DISEASE_SIMULATION_H_
#endif // HPC_DISEASE_SIMULATION_H_

#include <string>
#include <vector>
#include <random>
#include <algorithm>

enum class State {
  SUSCEPTIPLE,
  INFECTED,
  VACCINATED,
};

class Disease {
  public:
    Disease();
    Disease(int dur, float trans_prob);
    int& duration();
    float& transfer_probability();
    std::string& disease_name();
  
  private:
    int sick_duration;
    float trans_probability;
    std::string dises_name;
  };

class Person {
public:
  std::string get_infected();
  std::string get_vaccinated();
  void progress_each_day();
  std::string person_status();
  void infect(Disease& disease);
  State get_state();
  int get_days_to_recover();
  bool is_recovered();
  void touch(Person& person);
  void direct_infection(Disease& disease);

private:
  State state = State::SUSCEPTIPLE;
  int days_to_recover = 0;
  Disease dis;
  Disease& disease();
};

class Population {
  public:
    Population(int size);
    void random_infection(int infected, Disease& disease);
    void random_vaccination(int vaccinated);
    int count_healthy();
    int count_infected();
    int count_vaccinated();
    void one_more_day();
    std::string routine();
    void infect_neighbors(std::vector<Person>& infected_persons);
    std::vector<Person>& get_people();
    void random_interactions(int people_size, Person& person);

  private:
    std::vector<Person> people;
};

class Simulation {
public:
  Simulation(std::string in_file = "disease_in.ini");
  void start();
  void run(std::string sim_name, int sim_runs, int num_pop, int duration, 
           float trans, std::string location_name, float vac_rate);
  void calc_print(std::string sim_name, int sim_runs, std::vector<int>& steps_values, std::vector<int>& healthy_values,
                  std::vector<int>& recovered_values,std::vector<int>& vaccinated_values);

 private:
  std::string input_file;
};

class Utility {
public:
  static std::default_random_engine& engine();
  static float gen_random_num();
  static int randomized_indices(int elements_size);
  static std::vector<int> shuffled_indices(int elements_size);
  static std::pair<double, double> calc_mean_stdev(const std::vector<int>& elements);

private:
};
