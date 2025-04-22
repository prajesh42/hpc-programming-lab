#include "simulation.h"
#include <iostream>

std::string Person::get_infected() {
    (state) = State::INFECTED;
    return person_status();
}

std::string Person::get_vaccinated() {
    (state) = State::VACCINATED;
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
        case State::SUSCEPTIBLE:    return "susceptible";
        case State::INFECTED:       return "sick";
        case State::VACCINATED:     return "recovered";
        default: return "susceptible";
    }
}

void Person::infect(Disease d) {
    float ran_num = Utility::gen_random_num();
    if (ran_num <= d.get_transfer_probability()) {
        get_infected();
        (days_to_recover) = d.get_duration();
    }
}

State Person::get_state() {
    return state;
}

int Person::get_days_to_recover() {
    return days_to_recover;
}
