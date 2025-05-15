#ifndef HPC_DISEASE_SIMULATION_H_
#define HPC_DISEASE_SIMULATION_H_
#endif // HPC_DISEASE_SIMULATION_H_

#include <string>
#include <vector>

enum class State {
  SUSCEPTIBLE,
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
  State state = State::SUSCEPTIBLE;
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
    void infect_neighbors(std::vector<int>& infected_persons);
    std::vector<Person>& get_people();
    void random_interactions(int people_size, Person& person);

  private:
    std::vector<Person> people;
    std::vector<int> ran_indices;
};

class Simulation {
public:
  Simulation(std::string in_file = "disease_in.ini");
  void start();
  void run(int sim_runs, int num_pop, std::string disease_name, int duration, float trans, std::string location_name, float vac_rate, bool patient_0);

private:
  std::string input_file;
};

class Utility {
public:
  static float gen_random_num();
  static std::vector<int> randomized_indices(int elements_size);
  static std::vector<int> random_fixed_elements(std::vector<int> indices, int sample_size);

private:
};
