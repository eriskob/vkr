#ifndef TASK_H
#define TASK_H
#include <vector>

class task{
        int taskindex;
        int bcet;
        int wcet;
        int partition_id;
        int prio;
        int period;
        int procnum;
        int deadline;
        std::vector<int> anomaltasks;
    public:
        task(int num, std::string inputfile);
        int get_taskindex();
        int get_deadline();
        int get_wcet();
        int get_bcet();
        int get_prio();
        int get_period();
        int get_procnum();
        int get_partitionid();
        void find_anomaltasks(std::string inpufile);
        std::vector<int> get_anomaltasks();
};
class job: public task{
        int border_left;
        int border_right;
    public:
        job(int num, std::string inputfile, int border_left, int border_right);
        int get_bl();
        int get_br();
};
class system_config{
        int tasks;
        int partitions;
        int maj_fr;
        int messages;
    public:
        system_config(std::string path);
        int get_number_of_tasks();
        int get_maj_fr();
        int get_number_of_partitions();
        int get_number_of_messages();
};
class partition{
        int partition_id;
        int procnum;
        int maj_fr;
        std::vector<int> tasks;
        std::vector<std::vector<int>> window;
    public:
        partition(int num, std::string inputfile);
        int get_part_id();
        int get_procnum();
        int get_maj_fr();
        std::vector<int> get_tasks();
        std::vector<std::vector<int>> get_window();
};
class message{
        int index;
        int src;
        int dist;
        int bctt;
        int wctt;
    public:
        message(int num, std::string inputfile);
        int get_src();
        int get_dist();
        int get_bctt();
        int get_wctt();
        int get_index();
};
class message_instance: public message{
        int border_left;
        int border_right;
    public:
        message_instance(int num, std::string inputfile, int border_left, int border_right);
        int get_bl();
        int get_br();
};
#endif