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
    days_to_recover--;
}

std::string Person::person_status() {
    switch(state) {
        case State::SUSCEPTIBLE:    return "susceptible";
        case State::INFECTED:       return "sick";
        case State::VACCINATED:     return "recovered";
        default: return "susceptible";
    }
}

void Person::infect(Disease d) { }

State Person::get_state() {
    return state;
}

int Person::get_days_to_recover() {
    return days_to_recover;
}
