#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"

TEST_CASE("Test simulation class") { 
    Simulation sim = Simulation();
}

TEST_CASE("Test person class") { 
    Person per;

    SUBCASE("get_state_test()") {
        DOCTEST_CHECK_EQ(per.person_status(), "susceptible");
        DOCTEST_CHECK_EQ(per.get_state(), State::SUSCEPTIBLE);
    }

    SUBCASE("get_infected_test()") {
        per.get_infected();
        DOCTEST_CHECK_EQ(per.get_state(), State::INFECTED);
    }

    SUBCASE("get_vaccinated_test()") {
        per.get_vaccinated();
        DOCTEST_CHECK_EQ(per.get_state(), State::VACCINATED);
        DOCTEST_CHECK(per.is_recovered());
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
        for(int tag = 0; tag < people; tag++){
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

    SUBCASE("get_days_to_recover_test()") {
        Disease dis = Disease(20,1.0f);
        per.infect(dis);
        DOCTEST_CHECK_EQ(per.get_days_to_recover(), 20);

        SUBCASE("progress_each_day_test()") {
            per.progress_each_day();
            DOCTEST_CHECK_EQ(per.get_days_to_recover(), 19);
        }
    }

    SUBCASE("progress_each_day_recovery_test()") {
        Disease easy_recv = Disease(1, 1.0f);
        Person p;
        p.infect(easy_recv);
        p.progress_each_day();
        DOCTEST_CHECK_EQ(p.get_state(), State::VACCINATED);
    }

    SUBCASE("touch_test()") {
        Disease flu;
        flu.duration() = 20;
        flu.transfer_probability() = 1.0f;
        
        Person infected, healthy1, healthy2;
        infected.infect(flu);
        healthy1.touch(infected);
        healthy1.touch(healthy2);
        DOCTEST_CHECK_EQ(healthy1.get_state(), State::INFECTED);
        DOCTEST_CHECK_EQ(healthy2.get_state(), State::INFECTED);
    }

    SUBCASE("touch_multiple_person_test()") {
        Disease flu;
        flu.duration() = 20;
        flu.transfer_probability() = 1.0f; 

        Person infected;
        infected.infect(flu);
        infected.disease().transfer_probability() = 0.7f;
        
        float count = 0.0f;
        for(int per = 1; per < people; per++) {
            Person healthy;
            healthy.touch(infected);
            if(healthy.get_state() == State::INFECTED) {
                ++count;
            }
        }
        float chance = count/people;
        DOCTEST_CHECK(chance >= 0.6);
        DOCTEST_CHECK(chance <= 0.8);
    }
}

TEST_CASE("Test Disease class") {
    Disease dis;
    dis.duration() = 20;
    dis.transfer_probability() = 0.5f;

    SUBCASE("duration_test()") {
        DOCTEST_CHECK_EQ(20, dis.duration());
    }

    SUBCASE("transfer_probability_test()") {
        DOCTEST_CHECK_EQ(0.5f, dis.transfer_probability());
    }

    SUBCASE("parameterized_constructor_test()") {
        Disease disease = Disease(10, 0.7f);
        DOCTEST_CHECK_EQ(10, disease.duration());
        DOCTEST_CHECK_EQ(0.7f, disease.transfer_probability());
    }

    SUBCASE("disease_name_test()") {
        dis.disease_name() = "Corona";
        DOCTEST_CHECK_EQ("Corona", dis.disease_name());
    }
}

TEST_CASE("Test Population class") {
    Disease disease(10, 1.0f); 

    SUBCASE("constructor_test()") {
        Population pop(100);
        DOCTEST_CHECK_EQ(0, pop.count_infected());
        DOCTEST_CHECK_EQ(0, pop.count_vaccinated());
    }

    SUBCASE("random_infection_test()") {
        Population pop(50);
        pop.random_infection(10, disease);
        DOCTEST_CHECK_EQ(10, pop.count_infected());
    }

    SUBCASE("random_infection_limit_test()") {
        Population pop(5);
        pop.random_infection(10, disease); 
        DOCTEST_CHECK_EQ(5, pop.count_infected());
    }

    SUBCASE("random_vaccination_test()") {
        Population pop(50);
        pop.random_vaccination(20);
        DOCTEST_CHECK_EQ(20, pop.count_vaccinated());
    }

    SUBCASE("random_vaccination_limit_test()") {
        Population pop(8);
        pop.random_vaccination(20);
        DOCTEST_CHECK_EQ(8, pop.count_vaccinated());
    }

    SUBCASE("infection_with_complete_vaccination_test()") {
        Population pop(1000);
        pop.random_infection(30, disease);
        DOCTEST_CHECK(30 == pop.count_infected());
        pop.random_vaccination(1000);
        DOCTEST_CHECK(1000 == pop.count_vaccinated());
    }

    SUBCASE("one_more_day_test()") {
        Population pop(1000);
        pop.random_infection(1, disease);
        for(int days = 0; days < 10; ++days) {
            pop.one_more_day();
        }
        DOCTEST_CHECK(0 == pop.count_infected());
    }

    SUBCASE("one_more_day_infected_test()") {
        Population pop(1000);
        pop.random_infection(5, disease);
        for(int days = 0; days < 10 ; ++days) {
            int infected = pop.count_infected();
            int healthy = pop.count_healthy();
            DOCTEST_CHECK(5 == infected);
            DOCTEST_CHECK(1000 == (healthy + infected));
            pop.one_more_day();
        }
        DOCTEST_CHECK(0 == pop.count_infected());
    }

    SUBCASE("routine_test()") {
        Population pop(1);
        Disease dis(1, 1.0f);
        pop.one_more_day();
        DOCTEST_CHECK(" ? " == pop.routine());
        pop.random_infection(1, dis);
        DOCTEST_CHECK(" + " == pop.routine());
        pop.one_more_day();
        DOCTEST_CHECK(" - " == pop.routine());
    }

    SUBCASE("infect_neighbors_next_day_middle_index_test()") {
        Population pop(20);
        Disease dis(5, 1.0f);
        pop.get_people()[10].infect(dis);
        pop.one_more_day();
        pop.infect_neighbors();
        DOCTEST_CHECK(3 == pop.count_infected());
    }

    SUBCASE("infect_neighbors_next_day_initial_index_test()") {
        Population pop(20);
        Disease dis(5, 1.0f);
        pop.get_people()[0].infect(dis);
        pop.one_more_day();
        pop.infect_neighbors();
        DOCTEST_CHECK(2 == pop.count_infected());
    }

    SUBCASE("infect_neighbors_next_day_last_index_test()") {
        Population pop(20);
        Disease dis(5, 1.0f);
        pop.get_people()[19].infect(dis);
        pop.one_more_day();
        pop.infect_neighbors();
        DOCTEST_CHECK(2 == pop.count_infected());
    }

    SUBCASE("people_infected_days_test()") {
        Population pop(20);
        Disease dis(5, 1.0f);
        pop.get_people()[0].infect(dis);
        int days = 0;
        for(int step = 1; ; ++step) {
            if(pop.count_infected() == 0) {
                days = step;
                break;
            }
            pop.one_more_day();
            pop.infect_neighbors();
        }
        
        DOCTEST_CHECK(25 == days);
    }

    SUBCASE("random_interactions_test()") {
        Population pop(20);
        Disease dis(5, 1.0f);
        pop.random_infection(1, dis);
        pop.random_interactions(6);
        DOCTEST_CHECK(7 == pop.count_infected());
    }
}
