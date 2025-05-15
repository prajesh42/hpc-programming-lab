#include "simulation.h"
#include <iostream>
#include <algorithm>

Population::Population(int size) {
    people.resize(size);
    ran_indices = Utility::randomized_indices(size);
}

void Population::random_infection(int infected, Disease& disease) {
    int count = 0;
    for(int ind = 0; count < infected && ind < people.size(); ind++) {
        Person& person = people[ran_indices[ind]];
        person.infect(disease);
        if(person.get_state() == State::INFECTED) {
            count++;
        }
    }
}

void Population::random_vaccination(int vaccinated) {
    int count = 0;
    for(int ind = 0; count < vaccinated && ind < people.size(); ind++) {
        Person& person = people[ran_indices[ind]];
        if(person.get_state() != State::SUSCEPTIBLE) { continue; }
        person.get_vaccinated();
        count++; 
    }
}

int Population::count_healthy() { 
    return std::count_if(people.begin(), people.end(), [](Person& per){return per.get_state() == State::SUSCEPTIBLE;});
}

int Population::count_infected() { 
    return std::count_if(people.begin(), people.end(), [](Person& per){return per.get_state() == State::INFECTED;});
}

int Population::count_vaccinated() { 
    return std::count_if(people.begin(), people.end(), [](Person& per){return per.get_state() == State::VACCINATED;}); 
}

void Population::one_more_day() {
    std::vector<int> infected_persons;
    for(int index = 0; index < people.size(); ++index) {
        Person& person = people[index];
        person.progress_each_day();
        if(person.get_state() == State::INFECTED) {
            infected_persons.push_back(index);
        }
    }
    infect_neighbors(infected_persons);
}

std::string Population::routine() {
    std::string status;
    for(auto& person : people) {
        switch (person.get_state()) {
            case State::INFECTED: 
                status += " + ";
                break;
            case State::VACCINATED: 
                status += " - ";
                break;
            default: 
                status += " ? ";
                break;
        }
    }
    return status;
}

void Population::infect_neighbors(std::vector<int>& infected_persons) {
    for(auto& index: infected_persons) {
        if(index > 0 ) {
            people[index].touch(people[index - 1]);
        }
        if(index + 1 < people.size()) {
            people[index].touch(people[index + 1]);
        }
        random_interactions(6, people[index]);
    }
}

std::vector<Person>& Population::get_people() {
    return people;
}

void Population::random_interactions(int people_size, Person& infected_person) {
    std::vector<int> interac_indices = Utility::random_fixed_elements(ran_indices, people_size);
    for(auto& index: interac_indices) {
        infected_person.touch(people[index]);
    }
}
