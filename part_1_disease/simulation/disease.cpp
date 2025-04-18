#include "simulation.h"
#include<iostream>

Disease::Disease(int dur, float trans_prob) {
    sick_duration = dur;
    transfer_probability = trans_prob;
}
