#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../include/doctest.h"
#include "simulation.h"

void write_sample_ini(const std::string& filename) {
    std::ofstream file(filename);
    file << "[global]\n";
    file << "simulation_name = test_sim\n";
    file << "num_populations = 1\n";
    file << "simulation_runs = 5\n\n";

    file << "[disease]\n";
    file << "name = TestFlu\n";
    file << "duration = 2\n";
    file << "transmissibility = 0.5\n\n";

    file << "[population_1]\n";
    file << "name = Testville\n";
    file << "size = 10\n";
    file << "vaccination_rate = 0.2\n";
    file << "patient_0 = true\n";

    file.close();
}

TEST_CASE("simulation_integration_test()") {
    const std::string ini_file = "test_disease.ini";
    const std::string details_file = "disease_details_test_sim.csv";
    const std::string stats_file = "disease_stats_test_sim.csv";

    write_sample_ini(ini_file);

    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    Simulation sim(ini_file);
    sim.start();

    std::cout.rdbuf(old);

    std::string out = buffer.str();
    CHECK(out.find("Input file: test_disease.ini") != std::string::npos);

    std::ifstream details(details_file);
    std::ifstream stats(stats_file);

    REQUIRE(details.is_open());
    REQUIRE(stats.is_open());

    std::string first_line;
    std::getline(details, first_line);
    CHECK(first_line == "run,name,infectious,recovered,susceptiple,vaccinated");

    std::getline(stats, first_line);
    CHECK(first_line == "key,mean,std");

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

TEST_CASE("Test simulation class") {

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
        const std::string details_file = "disease_details_test.csv";
        const std::string stats_file = "disease_stats_test.csv";

        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

        Simulation sim;
        sim.run("test", 5, 10, 4, 0.9, "Deggendorf", 0.1);

        std::cout.rdbuf(old);

        std::string out = buffer.str();
        CHECK(out.find("Running") != std::string::npos);

        std::ifstream details(details_file);
        std::ifstream stats(stats_file);

        REQUIRE(details.is_open());
        REQUIRE(stats.is_open());

        std::string first_line;
        std::getline(details, first_line);
        CHECK(first_line == "run,name,infectious,recovered,susceptiple,vaccinated");

        std::getline(stats, first_line);
        CHECK(first_line == "key,mean,std");

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

    SUBCASE("calc_print_test()") {
        std::vector<int> steps    = {50, 60, 70};
        std::vector<int> healthy  = {1000, 1005, 995};
        std::vector<int> recovered= {14000, 13950, 14050};
        std::vector<int> vaccinated = {1500, 1500, 1500};

        std::stringstream buffer;
        std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

        Simulation sim;
        sim.calc_print("test", 3, steps, healthy, recovered, vaccinated);

        std::cout.rdbuf(old);
        std::string out = buffer.str();
        CHECK(out.find("Total statistics for") != std::string::npos);

        std::ifstream file("disease_stats_test.csv");
        REQUIRE(file.is_open());

        std::string header;
        std::getline(file, header);
        CHECK(header == "key,mean,std");

        std::string line;
        int line_count = 0;
        while (std::getline(file, line)) {
            CHECK(line.find("avg(") != std::string::npos);
            ++line_count;
        }
        CHECK(line_count == 4);

        file.close();

        std::remove("disease_stats_test.csv");
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

TEST_CASE("Test Utility class") {

    SUBCASE("gen_random_num_test()") {
        float count = 0.0f;
        for (int i = 0; i < 100; ++i) {
            float r = Utility::gen_random_num();
            if( r < 0.5) {
                count++;
            }
        }
        float val = count/100;
        CHECK(val > 0.4);
        CHECK(val < 0.6);
    }

    SUBCASE("calc_mean_stdev_test()") {
        std::vector<int> data = {2, 4, 4, 4, 5, 5, 7, 9};
        std::pair<double,double> mean_std = Utility::calc_mean_stdev(data);
        CHECK(mean_std.first == 5.0);
        CHECK(mean_std.second == 2.0);
    }
}
