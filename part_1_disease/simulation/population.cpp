#include "simulation.h"
#include <iostream>
#include <algorithm>

Population::Population(int size) {
    people.resize(size);
}

void Population::random_infection(int infected, Disease& disease) {
    std::vector<int> indices(people.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), Utility::engine());

    int count = 0;
    for (int ind = 0; ind < indices.size() && count < infected; ++ind) {
        Person& person = people[indices[ind]];

        if (person.get_state() == State::SUSCEPTIPLE) {
            person.infect(disease);
            if (person.get_state() == State::INFECTED) {
                count++;
            }
        }
    }
}

void Population::random_vaccination(int vaccinated) {
    std::vector<int> indices(people.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), Utility::engine());
    
    int count = 0;
    for (int ind = 0; ind < indices.size() && count < vaccinated; ++ind) {
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
    int total_size = people.size();
    int count = 0;
    for(int ind = 0; count < people_size && ind < total_size; ++ind) {
        int rand_index = Utility::randomized_indices(total_size);
        Person& person = people[rand_index];
        if(&person == &infected_person) continue;
        infected_person.touch(person);
        count++;
    }
}
