#ifndef SOLUTION_H
#define SOLUTION_H
#include <iostream>
#include <vector>
#include <utility>

class solution{
        int task_num;
        int lower_est;
        int upper_est;
    public:
        solution(int tasknum);
        int get_le();
        int get_ue();
        void get_lower_estimate(std::string path_file, std::string inputfile);
        void get_upper_estimate(std::string path_file, int taskindex);
};
#endif