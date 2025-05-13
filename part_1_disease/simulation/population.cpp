#include "simulation.h"
#include <iostream>
#include <algorithm>

Population::Population(int size) {
    people.resize(size);
}

void Population::random_infection(int infected, Disease& disease) {
    std::vector<int> ran_indices = Utility::randomized_indices(people.size());
    int count = 0;
    for(int ind = 0; count < infected && ind < people.size(); ind++) {
        Person& person = people[ran_indices[ind]];
        person.infect(disease);
        dis = disease;
        if(person.get_state() == State::INFECTED) {
            count++;
        }
    }
}

void Population::random_vaccination(int vaccinated) {
    std::vector<int> ran_indices = Utility::randomized_indices(people.size());
    int count = 0;
    for(int ind = 0; count < vaccinated && ind < people.size(); ind++) {
        Person& person = people[ran_indices[ind]];
        person.get_vaccinated();
        if(person.get_state() == State::VACCINATED) {
            count++;
        }
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
    for(auto& person : people) {
        person.progress_each_day();
    }
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

void Population::infect_neighbors() {
    bool left_touched = false;
    bool right_touched = false;
    for(int index = 0; index < people.size(); ++index) {
        if(people[index].get_state() == State::INFECTED) {
            if(!left_touched && index > 0 && people[index - 1].get_state() == State::SUSCEPTIBLE) {
                people[index].touch(people[index - 1]);
                left_touched = true;
            }
            if(!right_touched && index + 1 < people.size() && people[index + 1].get_state() == State::SUSCEPTIBLE) {
                people[index].touch(people[index + 1]); 
                right_touched = true;
            }
        }
        if(left_touched && right_touched) { break;}
    }
}

std::vector<Person>& Population::get_people() {
    return people;
}

void Population::random_interactions(int people_size) {
    int infected = count_infected();
    while(infected-->0) {
        random_infection(people_size, dis);
    }
}
