#include "simulation.h"
#include <iostream>

std::string Person::get_infected() {
    (state) = State::INFECTED;
    return person_status();
}

std::string Person::get_vaccinated() {
    (state) = State::VACCINATED;
    (days_to_recover) = 0;
    return person_status();
}

void Person::progress_each_day() {
    if(days_to_recover == 1) {
        get_vaccinated();
    }else if(state == State::INFECTED){
        days_to_recover--;
    }
}

std::string Person::person_status() {
    switch(state) {
        case State::INFECTED:       return "sick ( " + std::to_string(days_to_recover) + " days to go )";
        case State::VACCINATED:     return "recovered";
        default: return "susceptiple";
    }
}

void Person::infect(Disease& d) {
    if(state == State::SUSCEPTIPLE) {
        float ran_num = Utility::gen_random_num();
        if (ran_num <= d.transfer_probability()) {
            direct_infection(d);
        }
    }
}

State Person::get_state() {
    return state;
}

int Person::get_days_to_recover() {
    return days_to_recover;
}

bool Person::is_recovered() { 
    return state == State::VACCINATED; 
}

Disease& Person::disease() {
  return dis;
}

void Person::touch(Person& person) {
    if(person.get_state() == State::INFECTED && (state) == State::SUSCEPTIPLE) {
        infect(person.disease());
    }else if(person.get_state() == State::SUSCEPTIPLE && (state) == State::INFECTED) {
        person.infect(disease());
    }
}

void Person::direct_infection(Disease& disease) {
    (dis) = disease;
    (days_to_recover) = disease.duration();
    get_infected();
}
