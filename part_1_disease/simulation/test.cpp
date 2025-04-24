#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"

TEST_CASE("Test simulation class") { Simulation sim = Simulation(); }

TEST_CASE("Test person class") { 
    Person per;

    SUBCASE("get_state()") {
        DOCTEST_CHECK_EQ(per.person_status(), "susceptible");
        DOCTEST_CHECK_EQ(per.get_state(), State::SUSCEPTIBLE);
    }

    SUBCASE("get_infected()") {
        DOCTEST_CHECK_EQ(per.get_infected(), "sick");
        DOCTEST_CHECK_EQ(per.get_state(), State::INFECTED);
    }

    SUBCASE("get_vaccinated()") {
        DOCTEST_CHECK_EQ(per.get_vaccinated(), "recovered");
        DOCTEST_CHECK_EQ(per.get_state(), State::VACCINATED);
    }

    SUBCASE("infect()") {
        Disease dis = Disease(20, 0.9);
        int count = 0;
        for(int tag = 0; tag < 10; tag++){
            Person p;
            p.infect(dis);
            if(p.get_state() == State::INFECTED) {
                ++count;
            }
        }
        DOCTEST_CHECK(count >= 5);

        SUBCASE("get_days_to_recover()") {
            per.infect(dis);
            DOCTEST_CHECK_EQ(per.get_days_to_recover(), 20);
            
            SUBCASE("progress_each_day()") {
                per.progress_each_day();
                DOCTEST_CHECK_EQ(per.get_days_to_recover(), 19);
            }

            SUBCASE("progress_each_day() CASE::Recovery") {
                Disease easy_recv = Disease(1, 0.9);
                per.infect(easy_recv);
                per.progress_each_day();
                DOCTEST_CHECK_EQ(per.get_state(), State::VACCINATED);
            }
        }
    }
}
