#include "simulation.h"
#include <iostream>
#include <algorithm>

Population::Population(int size) {
    people.resize(size);
}

void Population::random_infection(int infected, Disease& disease) {
    std::vector<int> ran_indices = Utility::randomized_indices(people.size());
    
    for(int ind = 0; ind < infected && ind < people.size(); ind++) {
        people[ran_indices[ind]].infect(disease);
    }
}

void Population::random_vaccination(int vaccinated) {
    std::vector<int> ran_indices = Utility::randomized_indices(people.size());

    for(int ind = 0; ind < vaccinated && ind < people.size(); ind++) {
        people[ran_indices[ind]].get_vaccinated();
    }
}

int Population::count_infected() { 
    return std::count_if(people.begin(), people.end(), [](Person& per){return per.get_state() == State::INFECTED;});
}

int Population::count_vaccinated() { 
    return std::count_if(people.begin(), people.end(), [](Person& per){return per.get_state() == State::VACCINATED;}); 
}

void Population::one_more_day() {
    for(auto& person : people) {
        person.progress_each_day();
    }
}