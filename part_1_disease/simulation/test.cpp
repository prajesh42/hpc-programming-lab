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

    int people = 1000;

    SUBCASE("infect_100%_transmittablility_test()") {
        Disease dis = Disease(20, 1.0f);
        float count = 0;
        for(int tag = 0; tag < people; tag++){
            Person p;
            p.infect(dis);
            if(p.get_state() == State::INFECTED) {
                ++count;
            }
        }
        float chance = count/people;
        DOCTEST_CHECK(chance == 1.0f);
    }

    SUBCASE("infect_50%_transmittablility_test()") {
        Disease dis = Disease(20, 0.5f);
        float count = 0;
        for(int tag = 0; tag < people; tag++){
            Person p;
            p.infect(dis);
            if(p.get_state() == State::INFECTED) {
                ++count;
            }
        }
        float chance = count/people;
        DOCTEST_CHECK(chance >= 0.45f);
        DOCTEST_CHECK(chance <= 0.55f);
    }

    SUBCASE("infect_recovered_test()") {
        Disease dis = Disease(10, 1.0f);
        float count = 0;
        for(int tag = 0; tag < 10; tag++){
            Person p;
            p.get_vaccinated();
            p.infect(dis);
            if(p.get_state() == State::INFECTED) {
                ++count;
            }
        }
        float chance = count/people;
        DOCTEST_CHECK(chance == 0.0f);
    }

    SUBCASE("get_days_to_recover()") {
        Disease dis = Disease(20,1.0f);
        per.infect(dis);
        DOCTEST_CHECK_EQ(per.get_days_to_recover(), 20);

        SUBCASE("progress_each_day()") {
            per.progress_each_day();
            DOCTEST_CHECK_EQ(per.get_days_to_recover(), 19);
        }
    }

    SUBCASE("progress_each_day() CASE::Recovery") {
        Disease easy_recv = Disease(1, 1.0f);
        Person p;
        p.infect(easy_recv);
        p.progress_each_day();
        DOCTEST_CHECK_EQ(p.get_state(), State::VACCINATED);
    }
}
