#include "simulation.h"
#include<iostream>

Disease::Disease(int dur, float trans_prob) {
    sick_duration = dur;
    transfer_probability = trans_prob;
}

int Disease::get_duration() { 
    return sick_duration; 
}

float Disease::get_transfer_probability() { 
    return transfer_probability; 
}
