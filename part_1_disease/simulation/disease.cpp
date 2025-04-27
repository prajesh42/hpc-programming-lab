#include "simulation.h"
#include<iostream>

Disease::Disease() {}

Disease::Disease(int dur, float trans_prob) {
  sick_duration = dur;
  trans_probability = trans_prob;
}

int& Disease::duration() { 
    return sick_duration; 
}

float& Disease::transfer_probability() { 
    return trans_probability; 
}
