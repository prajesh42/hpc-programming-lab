#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"

void write_sample_ini(const std::string& filename) {
    std::ofstream file(filename);
    file << "[global]\n";
    file << "simulation_name = test_sim\n";
    file << "num_populations = 1\n";
    file << "simulation_runs = 1\n\n";

    file << "[disease]\n";
    file << "name = TestFlu\n";
    file << "duration = 2\n";
    file << "transmissibility = 0.5\n\n";

    file << "[population_1]\n";
    file << "name = Testville\n";
    file << "size = 50\n";
    file << "vaccination_rate = 0.2\n";
    file << "patient_0 = true\n";

    file.close();
}

TEST_CASE("Test simulation class") {
    
    SUBCASE("start_test()") {
        const std::string ini_file = "test_disease.ini";
        const std::string details_file = "disease_details.csv";
        const std::string stats_file = "disease_stats.csv";

        write_sample_ini(ini_file);

        Simulation sim(ini_file);
        sim.start();

        std::ifstream details(details_file);
        std::ifstream stats(stats_file);

        REQUIRE(details.is_open());
        REQUIRE(stats.is_open());

        std::string first_line;
        std::getline(details, first_line);
        CHECK(first_line == "name,infectious,recovered,susceptiple,vaccinated");

        std::getline(stats, first_line);
        CHECK(first_line == "key,value");

        std::string content_line;
        bool has_data = false;
        while (std::getline(stats, content_line)) {
            if (!content_line.empty() && content_line.find(',') != std::string::npos) {
                has_data = true;
                break;
            }
        }

        CHECK(has_data);
        details.close();
        stats.close();
        std::remove(ini_file.c_str());
        std::remove(details_file.c_str());
        std::remove(stats_file.c_str());
    }

    SUBCASE("simulation_missing_file_test()") {
        const std::string missing_file = "nonexistent.ini";
    
        std::stringstream err_buffer;
        std::streambuf* old_buf = std::cerr.rdbuf(err_buffer.rdbuf());
    
        Simulation sim(missing_file);
        sim.start();
    
        std::cerr.rdbuf(old_buf);
    
        std::string error_output = err_buffer.str();
        CHECK(error_output.find("Can't load 'disease_in.ini'") != std::string::npos);
    }

    SUBCASE("run_test()") {
        const std::string details_file = "disease_details.csv";
        const std::string stats_file = "disease_stats.csv";

        Simulation sim;
        sim.run(1, 10, "Corona", 4, 0.9, "Deggendorf", 0.1, true);

        std::ifstream details(details_file);
        std::ifstream stats(stats_file);

        REQUIRE(details.is_open());
        REQUIRE(stats.is_open());

        std::string first_line;
        std::getline(details, first_line);
        CHECK(first_line == "name,infectious,recovered,susceptiple,vaccinated");

        std::getline(stats, first_line);
        CHECK(first_line == "key,value");

        std::string content_line;
        bool has_data = false;
        while (std::getline(stats, content_line)) {
            if (!content_line.empty() && content_line.find(',') != std::string::npos) {
                has_data = true;
                break;
            }
        }

        CHECK(has_data);
        details.close();
        stats.close();
        std::remove(details_file.c_str());
        std::remove(stats_file.c_str());
    }
}

TEST_CASE("Test person class") { 
    Person per;

    SUBCASE("get_state_test()") {
        CHECK_EQ(per.person_status(), "susceptiple");
        CHECK_EQ(per.get_state(), State::SUSCEPTIPLE);
    }

    SUBCASE("get_infected_test()") {
        per.get_infected();
        CHECK_EQ(per.get_state(), State::INFECTED);
    }

    SUBCASE("get_vaccinated_test()") {
        per.get_vaccinated();
        CHECK_EQ(per.get_state(), State::VACCINATED);
        CHECK(per.is_recovered());
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
        CHECK(chance == 1.0f);
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
        CHECK(chance >= 0.45f);
        CHECK(chance <= 0.55f);
    }

    SUBCASE("infect_recovered_test()") {
        Disease dis = Disease(10, 1.0f);
        float count = 0;
        for(int tag = 0; tag < people; tag++){
            Person p;
            if(tag > 99) {
                p.get_vaccinated();
            }
            p.infect(dis);
            if(p.get_state() == State::INFECTED) {
                ++count;
            }
        }
        float chance = count/people;
        CHECK(chance == 0.1f);
    }

    SUBCASE("get_days_to_recover_test()") {
        Disease dis = Disease(20,1.0f);
        per.infect(dis);
        CHECK_EQ(per.get_days_to_recover(), 20);

        SUBCASE("progress_each_day_test()") {
            per.progress_each_day();
            CHECK_EQ(per.get_days_to_recover(), 19);
        }
    }

    SUBCASE("progress_each_day_recovery_test()") {
        Disease easy_recv = Disease(1, 1.0f);
        Person p;
        p.infect(easy_recv);
        p.progress_each_day();
        CHECK_EQ(p.get_state(), State::VACCINATED);
    }

    SUBCASE("touch_test()") {
        Disease flu;
        flu.duration() = 20;
        flu.transfer_probability() = 1.0f;
        
        Person infected, healthy1, healthy2;
        infected.infect(flu);
        healthy1.touch(infected);
        healthy1.touch(healthy2);
        CHECK_EQ(healthy1.get_state(), State::INFECTED);
        CHECK_EQ(healthy2.get_state(), State::INFECTED);
    }

    SUBCASE("touch_multiple_person_test()") {
        Disease flu(20, 0.7f);

        Person infected;
        infected.direct_infection(flu);
        
        float count = 0.0f;
        for(int per = 1; per < people; per++) {
            Person healthy;
            healthy.touch(infected);
            if(healthy.get_state() == State::INFECTED) {
                ++count;
            }
        }
        float chance = count/people;
        CHECK(chance >= 0.6);
        CHECK(chance <= 0.8);
    }

    SUBCASE("direct_infection_test()") {
        Person person;
        Disease flu(20, 0.7f);
        person.direct_infection(flu);
        CHECK(person.get_state() == State::INFECTED);
    }
}

TEST_CASE("Test Disease class") {
    Disease dis;
    dis.duration() = 20;
    dis.transfer_probability() = 0.5f;

    SUBCASE("duration_test()") {
        CHECK_EQ(20, dis.duration());
    }

    SUBCASE("transfer_probability_test()") {
        CHECK_EQ(0.5f, dis.transfer_probability());
    }

    SUBCASE("parameterized_constructor_test()") {
        Disease disease = Disease(10, 0.7f);
        CHECK_EQ(10, disease.duration());
        CHECK_EQ(0.7f, disease.transfer_probability());
    }

    SUBCASE("disease_name_test()") {
        dis.disease_name() = "Corona";
        CHECK_EQ("Corona", dis.disease_name());
    }
}

TEST_CASE("Test Population class") {
    Disease disease(10, 1.0f); 

    SUBCASE("constructor_test()") {
        Population pop(100);
        CHECK_EQ(0, pop.count_infected());
        CHECK_EQ(0, pop.count_vaccinated());
    }

    SUBCASE("random_infection_test()") {
        Population pop(50);
        pop.random_infection(10, disease);
        CHECK_EQ(10, pop.count_infected());
    }

    SUBCASE("random_infection_limit_test()") {
        Population pop(5);
        pop.random_infection(10, disease); 
        CHECK_EQ(5, pop.count_infected());
    }

    SUBCASE("random_vaccination_test()") {
        Population pop(50);
        pop.random_vaccination(20);
        CHECK_EQ(20, pop.count_vaccinated());
    }

    SUBCASE("random_vaccination_limit_test()") {
        Population pop(8);
        pop.random_vaccination(20);
        CHECK_EQ(8, pop.count_vaccinated());
    }

    SUBCASE("random_infection_test()") {
        Population pop(1000);
        pop.random_infection(30, disease);
        CHECK(30 == pop.count_infected());
    }

    SUBCASE("complete_vaccination_test()") {
        Population pop(1000);
        pop.random_vaccination(1000);
        CHECK(1000 == pop.count_vaccinated());
    }

    SUBCASE("one_more_day_test()") {
        Population pop(1000);
        pop.random_infection(1, disease);
        pop.one_more_day();
        CHECK(3 <= pop.count_infected());
    }

    SUBCASE("routine_test()") {
        Population pop(1);
        Disease dis(1, 1.0f);
        pop.one_more_day();
        CHECK(" ? " == pop.routine());
        pop.random_infection(1, dis);
        CHECK(" + " == pop.routine());
        pop.one_more_day();
        CHECK(" - " == pop.routine());
    }

    SUBCASE("infect_neighbors_next_day_middle_index_test()") {
        Population pop(20);
        Disease dis(5, 1.0f);
        pop.get_people()[10].infect(dis);
        pop.one_more_day();
        CHECK(3 <= pop.count_infected());
    }

    SUBCASE("infect_neighbors_next_day_initial_index_test()") {
        Population pop(20);
        Disease dis(5, 1.0f);
        pop.get_people()[0].infect(dis);
        pop.one_more_day();
        CHECK(2 <= pop.count_infected());
    }

    SUBCASE("infect_neighbors_next_day_last_index_test()") {
        Population pop(20);
        Disease dis(5, 1.0f);
        pop.get_people()[19].infect(dis);
        pop.one_more_day();
        CHECK(2 <= pop.count_infected());
    }

    SUBCASE("random_interactions_test()") {
        Population pop(50);
        Disease dis(5, 1.0f);
        pop.random_infection(1, dis);
        pop.one_more_day();
        pop.one_more_day();
        CHECK(25 <= pop.count_infected());
    }

    SUBCASE("count_healthy_test()") {
        Population pop(50);
        Disease dis(5, 1.0f);
        pop.random_infection(5, dis);
        CHECK(45 == pop.count_healthy());
    }

    SUBCASE("count_infected_test()") {
        Population pop(50);
        Disease dis(5, 1.0f);
        pop.random_infection(5, dis);
        CHECK(5 == pop.count_infected());
    }

    SUBCASE("count_vaccinated_test()") {
        Population pop(50);
        Disease dis(5, 1.0f);
        pop.random_vaccination(10);
        CHECK(10 == pop.count_vaccinated());
    }
}
