#ifndef HPC_DISEASE_SIMULATION_H_
#define HPC_DISEASE_SIMULATION_H_
#endif // HPC_DISEASE_SIMULATION_H_

#include <string>

enum class State {
  SUSCEPTIBLE,
  INFECTED,
  VACCINATED,
};

class Disease;

class Person {
public:
  std::string get_infected();
  std::string get_vaccinated();
  void progress_each_day();
  std::string person_status();
  void infect(Disease& disease);
  State get_state();
  int get_days_to_recover();

private:
  State state = State::SUSCEPTIBLE;
  int days_to_recover;
};

class Disease {
public:
  Disease(int dur, float trans_prob);
  int get_duration();
  float get_transfer_probability();

private:
  int sick_duration;
  float transfer_probability;
};

class Population {};

class Simulation {
public:
  Simulation(std::string in_file = "disease_in.ini");
  void start();

private:
  std::string input_file;
};

class Utility {
public:
  static float gen_random_num();

private:
};
