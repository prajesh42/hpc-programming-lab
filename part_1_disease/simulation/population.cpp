#include "simulation.h"
#include <iostream>

Population::Population(int size) {
    people.resize(size);
}

void Population::random_infection(int infected, Disease& disease) {
    int total_size = people.size();
    std::vector<int> indices = Utility::shuffled_indices(total_size);
    int count = 0;
    for (int ind = 0; ind < total_size && count < infected; ++ind) {
        people[indices[ind]].direct_infection(disease);
        count++;
    }
}

void Population::random_vaccination(int vaccinated) {
    int total_size = people.size();
    std::vector<int> indices = Utility::shuffled_indices(total_size);
    int count = 0;
    for (int ind = 0; ind < total_size && count < vaccinated; ++ind) {
        Person& person = people[indices[ind]];
        if (person.get_state() == State::SUSCEPTIPLE) {
            person.get_vaccinated();
            count++;
        }
    }
}

int Population::count_healthy() { 
    return std::count_if(people.begin(), people.end(), [](Person& per){return per.get_state() == State::SUSCEPTIPLE;});
}

int Population::count_infected() { 
    return std::count_if(people.begin(), people.end(), [](Person& per){return per.get_state() == State::INFECTED;});
}

int Population::count_vaccinated() { 
    return std::count_if(people.begin(), people.end(), [](Person& per){return per.get_state() == State::VACCINATED;}); 
}

void Population::one_more_day() {
    std::vector<Person> infected_persons;
    for(int index = 0; index < people.size(); ++index) {
        Person& person = people[index];
        if(person.get_state() == State::INFECTED) {
            infected_persons.push_back(person);
        }
        person.progress_each_day();
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

void Population::infect_neighbors(std::vector<Person>& infected_persons) {
    for(auto& person: infected_persons) {
        random_interactions(6, person);
    }
}

std::vector<Person>& Population::get_people() {
    return people;
}

void Population::random_interactions(int people_size, Person& infected_person) {
    std::vector<int> ran_indices = Utility::randomized_indices(people.size(), people_size);
    for(auto& ind: ran_indices) {
        Person& person = people[ind];
        if(&person == &infected_person) continue;
        infected_person.touch(person);
    }
}
