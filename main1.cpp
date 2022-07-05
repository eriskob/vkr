#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <charconv>
#include <algorithm>
#include <utility>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "task.h"
#include "solution.h"
#include <chrono>
using namespace rapidxml;
int find_num_of_proc(std::string inputfile){
    system_config system(inputfile);
    std::vector<task> tasks;
    for(int i = 0; i < system.get_number_of_tasks(); i++){
        tasks.push_back(task(i, inputfile.c_str()));
    }
    int t = 0;
    for(int i = 0; i < tasks.size(); i++){
        if(tasks[i].get_procnum() >= t){
            t = tasks[i].get_procnum();
        }
    }
    return t;
}
void create_datafile(std::string inputfile){
    system_config system(inputfile);
    std::vector<task> tasks;
    for(int i = 0; i < system.get_number_of_tasks(); i++){
        tasks.push_back(task(i, inputfile));
    }
    xml_document<char> doc;
    std::ifstream file(inputfile);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    xml_document<> doc1;
    std::ofstream file_stored("data.xml");
    xml_node<> *node_wr1 = doc1.allocate_node(node_element, "system");
    doc1.append_node(node_wr1);
    int proc = find_num_of_proc(inputfile);
    std::cout << "proc: " << proc << "\n";
    for(int i = 0; i <= proc; i++){
        xml_node<> *node_wr2 = doc1.allocate_node(node_element, "module");
        node_wr1->append_node(node_wr2);
        node_wr2->append_attribute(doc.allocate_attribute("major_frame", std::to_string(system.get_maj_fr()).c_str()));
        std::string *module = new std::string("Module");
        *module += std::to_string(i);
        char *node_name = doc.allocate_string(module->c_str());
        node_wr2->append_attribute(doc.allocate_attribute("name", node_name));
        delete module;
        xml_node<> *node_wr3 = doc1.allocate_node(node_element, "partition");
        node_wr2->append_node(node_wr3);
        node_wr3->append_attribute(doc.allocate_attribute("id", "0"));
        node_wr3->append_attribute(doc.allocate_attribute("name", "m0_part0"));
        node_wr3->append_attribute(doc.allocate_attribute("scheduler", "FPPS"));
        for(int j = 0; j < tasks.size(); j++){
            if(tasks[j].get_procnum() == i){
                xml_node<> *node_wr4 = doc1.allocate_node(node_element, "task");
                node_wr3->append_node(node_wr4);
                char *node_name4 = doc.allocate_string(std::to_string(tasks[j].get_taskindex()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("id", node_name4));
                std::string name5 = std::string("task");
                name5 += std::to_string(tasks[j].get_taskindex());
                char *node_name5 = doc.allocate_string(name5.c_str());
                node_wr4->append_attribute(doc.allocate_attribute("name", node_name5));
                char *node_name6 = doc.allocate_string(std::to_string(tasks[j].get_wcet()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("wcet", node_name6));
                char *node_name7 = doc.allocate_string(std::to_string(tasks[j].get_prio()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("prio", node_name7));
                node_wr4->append_attribute(doc.allocate_attribute("offset", "0"));
                char *node_name8 = doc.allocate_string(std::to_string(tasks[j].get_period()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("period", node_name8));
                char *node_name9 = doc.allocate_string(std::to_string(tasks[j].get_deadline()).c_str());
                node_wr4->append_attribute(doc.allocate_attribute("deadline", node_name9));
            }
        }
        xml_node<> *node_wr5 = doc1.allocate_node(node_element, "window");
        node_wr2->append_node(node_wr5);
        node_wr5->append_attribute(doc.allocate_attribute("start", "0"));
        char *node_name9 = doc.allocate_string(std::to_string(tasks[0].get_deadline()).c_str());
        node_wr5->append_attribute(doc.allocate_attribute("stop", node_name9));
        node_wr5->append_attribute(doc.allocate_attribute("partition", "0"));
    }
    for(int i = 0; i < system.get_number_of_tasks(); i++){
        node = node->next_sibling();
    }
    for(; node; node = node->next_sibling()){
        xml_node<> *node_wr6 = doc1.allocate_node(node_element, "link");
        node_wr1->append_node(node_wr6);
        node_wr6->append_attribute(doc.allocate_attribute("src", node->first_attribute("src")->value()));
        node_wr6->append_attribute(doc.allocate_attribute("dst", node->first_attribute("dist")->value()));
        node_wr6->append_attribute(doc.allocate_attribute("delay", node->first_attribute("delay")->value()));
    }
    file_stored << doc1;
    file_stored.close();
    doc1.clear();
}
void create_inputfile_ue(std::string path, int tasknum, int val){
    xml_document<char> doc;
    std::ifstream file(path);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    for(; node; node = node->next_sibling()){
        xml_node<> *node1 = node->first_node();
        for(; node1; node1 = node1->next_sibling()){
            xml_attribute<> *attr = node1->first_attribute();
            if(std::string(attr->name()) == "id"){
                if(atoi(attr->value()) == tasknum){
                    std::string s = std::to_string(val);
                    char * text = doc.allocate_string(s.c_str());
                    node1->first_attribute("wcet")->value(text);
                    node1->first_attribute("bcet")->value(text);
                }
            }
        }
    }
    std::string data;
    rapidxml::print(std::back_inserter(data), doc);
    std::ofstream file2;
    file2.open(path.c_str());
    file2 << data;
    file2.close();
}

void create_inputfile_le(std::string path, int tasknum, int val){
    xml_document<char> doc;
    std::ifstream file(path);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);


    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node1 = root_node->first_node("module");
    for(; node1; node1 = node1->next_sibling()){
        xml_node<> *node2 = node1->first_node("partition");
        for(; node2; node2 = node2->next_sibling()){
            xml_node<> *node3 = node2->first_node("task");
            for(; node3; node3 = node3->next_sibling()){
                xml_attribute<> *attr = node3->first_attribute("id");
                if(atoi(attr->value()) != tasknum){
                    continue;
                }
                else{
                    std::string s = std::to_string(val);
                    char * text = doc.allocate_string(s.c_str());
                    node3->first_attribute("wcet")->value(text);
                }
            }
        }
    }
    std::string data;
    rapidxml::print(std::back_inserter(data), doc);
    std::ofstream file2;
    file2.open(path.c_str());
    file2 << data;
    file2.close();
}

void rec(std::vector<task> & anom_tasks, int & opt, int & est, job tmp1, solution tmp2, std::string path1){
    std::cout << "next:\n";
    std::cout << "\n\n\nanomtasks vector:\n";
    for(int i = 0; i < anom_tasks.size(); i++){
        std::cout << anom_tasks[i].get_taskindex() << "\n";
    }
    std::cout << "anomtasks vector:\n\n\n";
    for(int j = anom_tasks[0].get_wcet(); j >= anom_tasks[0].get_bcet(); j--){
        std::cout << anom_tasks[0].get_taskindex() << " " << anom_tasks[0].get_wcet() << " " << anom_tasks[0].get_bcet() << " curr: " << j << " \n";
        create_inputfile_ue("input1.xml", anom_tasks[0].get_taskindex(), j);
        std::cout << "---1---\n";
        tmp2.get_upper_estimate("input1.xml", tmp1.get_taskindex());
        std::cout << "---2---\n";
        est = tmp2.get_ue();
        std::cout << "---3---\n";
        create_inputfile_le("data1.xml", anom_tasks[0].get_taskindex(), j);
        std::cout << "---4---\n";
        tmp2.get_lower_estimate("data1.xml", path1);
        int local_opt = tmp2.get_le();
        std::cout << "est: " << est << " opt: " << opt << std::endl;
        if(est > opt){
            if(local_opt > opt){
                opt = local_opt;
            }
            std::vector<task> anom_tasks_copy(anom_tasks);
            anom_tasks_copy.erase(anom_tasks_copy.begin());
            if(!anom_tasks_copy.empty()){
                rec(anom_tasks_copy, opt, est, tmp1, tmp2, path1);
            }
            else{
                std::cout << "end\n";
                continue;
            }
        }
        // else{
        //     std::cout << "end\n";
        //     return;
        // }
    }
    return;
}

void find_solution(job tmp1, solution tmp2, std::vector<task> & anom_tasks, std::vector<solution> anom_sol, int & WCRT, std::string path1, std::string path2){
    tmp2.get_lower_estimate(path2, path1);
    int opt = tmp2.get_le();
    tmp2.get_upper_estimate(path1, tmp1.get_taskindex());
    int est = tmp2.get_ue();
    std::cout << opt << " " << est << "\n";
    std::string path1_cp = "cp " + path1 + " input1.xml";
    std::string path2_cp = "cp " + path2 + " data1.xml";
    system(path1_cp.c_str());
    system(path2_cp.c_str());
    std::vector<int> wb;
    if(opt == est){
        WCRT = opt;
        return;
    }
    rec(anom_tasks, opt, est, tmp1, tmp2, path1);
    std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    
    WCRT = opt;
}

int find_max_prio(std::vector<task> tasks){
    int max_prio = -1;
    for(int i = 0; i < tasks.size(); i++){
        if(tasks[i].get_prio() > max_prio){
            max_prio = tasks[i].get_prio();
        }
    }
    return max_prio;
}


std::vector<std::vector<int>> window_ordered(std::vector<std::vector<int>> wnd_proc){
    std::vector<std::vector<int>> wnd_ord;
    int max = 0;
    for(auto i: wnd_proc){
        if(i[0] > max){
            max = i[0];
        }
    }
    int min = max;
    for(auto i: wnd_proc){
        if(i[0] < min){
            min = i[0];
        }
    }
    int nxt = min;
    for(int j = 0; j < wnd_proc.size(); j++){
        for(auto i: wnd_proc){
            if(i[0] == nxt){
                wnd_ord.push_back(i);
                nxt = i[1];
                break;
            }
        }
    }
    return wnd_ord;
}


std::string create_datafile_new(std::string inputfile){
    std::string datafile = "data.xml";
    system_config syst(inputfile);
    std::vector<task> tasks;
    for(int i = 0; i < syst.get_number_of_tasks(); i++){
        tasks.push_back(task(i, inputfile));
    }
    std::vector<message> messages;
    for(int i = 0; i < syst.get_number_of_messages(); i++){
        messages.push_back(message(i, inputfile));
    }
    std::vector<partition> partitions;
    for(int i = 0; i < syst.get_number_of_partitions(); i++){
        partitions.push_back(partition(i, inputfile));
    }

    // xml_document<char> doc;
    // std::ifstream file(inputfile);
    // std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    // buffer.push_back('\0');
    // doc.parse<0>(&buffer[0]);

    xml_document<char> doc1;
    std::ofstream file_stored(datafile);
    xml_node<> *node_wr1 = doc1.allocate_node(node_element, "system");
    doc1.append_node(node_wr1);
    int proc = find_num_of_proc(inputfile);
    for(int k = 0; k <= proc; k++){
        xml_node<> *node_wr2 = doc1.allocate_node(node_element, "module");
        node_wr1->append_node(node_wr2);
        node_wr2->append_attribute(doc1.allocate_attribute("major_frame", std::to_string(syst.get_maj_fr()).c_str()));
        std::string name1 = "Module";
        name1 += std::to_string(k);
        char *node_wr2_name1 = doc1.allocate_string(name1.c_str());
        node_wr2->append_attribute(doc1.allocate_attribute("name", node_wr2_name1));
        int partition_count = 0;
        for(int i = 0; i < partitions.size(); i++){
            if(partitions[i].get_procnum() == k){
                xml_node<> *node_wr3 = doc1.allocate_node(node_element, "partition");
                node_wr2->append_node(node_wr3);
                // partitions[i].get_part_id()
                char *node_wr3_name0 = doc1.allocate_string(std::to_string(partition_count).c_str());
                node_wr3->append_attribute(doc1.allocate_attribute("id", node_wr3_name0));
                std::string part_num = "m0_part";
                part_num += std::to_string(partition_count);
                char *node_wr3_name1 = doc1.allocate_string(part_num.c_str());
                node_wr3->append_attribute(doc1.allocate_attribute("name", node_wr3_name1));
                node_wr3->append_attribute(doc1.allocate_attribute("scheduler", "FPPS"));
                std::vector<task> tasks_part;
                for(auto m: partitions[i].get_tasks()){
                    tasks_part.push_back(task(m, inputfile));
                }
                for(int j = 0; j < tasks_part.size(); j++){
                    xml_node<> *node_wr4 = doc1.allocate_node(node_element, "task");
                    node_wr3->append_node(node_wr4);
                    char *node_wr4_name1 = doc1.allocate_string(std::to_string(tasks_part[j].get_taskindex()).c_str());
                    node_wr4->append_attribute(doc1.allocate_attribute("id", node_wr4_name1));
                    char *node_wr4_name2 = doc1.allocate_string((std::string("task") + std::to_string(tasks_part[j].get_taskindex())).c_str());
                    node_wr4->append_attribute(doc1.allocate_attribute("name", node_wr4_name2));
                    char *node_wr4_name3 = doc1.allocate_string(std::to_string(tasks_part[j].get_wcet()).c_str());
                    node_wr4->append_attribute(doc1.allocate_attribute("wcet", node_wr4_name3));
                    char *node_wr4_name4 = doc1.allocate_string(std::to_string(tasks_part[j].get_prio()).c_str());
                    node_wr4->append_attribute(doc1.allocate_attribute("prio", node_wr4_name4));
                    char *node_wr4_name5 = doc1.allocate_string(std::to_string(0).c_str());
                    node_wr4->append_attribute(doc1.allocate_attribute("offset", node_wr4_name5));
                    char *node_wr4_name6 = doc1.allocate_string(std::to_string(tasks_part[j].get_period()).c_str());
                    node_wr4->append_attribute(doc1.allocate_attribute("period", node_wr4_name6));
                    char *node_wr4_name7 = doc1.allocate_string(std::to_string(tasks_part[j].get_deadline()).c_str());
                    node_wr4->append_attribute(doc1.allocate_attribute("deadline", node_wr4_name7));
                }
                partition_count++;
            }
        }
        int partition_count1 = 0;
        std::vector<std::vector<int>> wnd_unord;
        for(int i = 0; i < partitions.size(); i++){
            if(partitions[i].get_procnum() == k){
                std::vector<std::vector<int>> wnd_part = partitions[i].get_window();
                for(auto m: wnd_part){
                    m.push_back(partition_count1);
                    wnd_unord.push_back(m);
                }
                partition_count1++;
            }
        }
        std::vector<std::vector<int>> wnd_ord = window_ordered(wnd_unord);
        for(int j = 0; j < wnd_ord.size(); j++){
            xml_node<> *node_wr6 = doc1.allocate_node(node_element, "window");
            node_wr2->append_node(node_wr6);
            char *node_wr6_name1 = doc1.allocate_string(std::to_string(wnd_ord[j][0]).c_str());
            node_wr6->append_attribute(doc1.allocate_attribute("start", node_wr6_name1));
            char *node_wr6_name2 = doc1.allocate_string(std::to_string(wnd_ord[j][1]).c_str());
            node_wr6->append_attribute(doc1.allocate_attribute("stop", node_wr6_name2));
            char *node_wr6_name3 = doc1.allocate_string(std::to_string(wnd_ord[j][2]).c_str());
            node_wr6->append_attribute(doc1.allocate_attribute("partition", node_wr6_name3));
        }
    }
    for(int i = 0; i < messages.size(); i++){
        xml_node<> *node_wr5 = doc1.allocate_node(node_element, "link");
        node_wr1->append_node(node_wr5);
        char *node_wr5_name1 = doc1.allocate_string(std::to_string(messages[i].get_src()).c_str());
        node_wr5->append_attribute(doc1.allocate_attribute("src", node_wr5_name1));
        char *node_wr5_name2 = doc1.allocate_string(std::to_string(messages[i].get_dist()).c_str());
        node_wr5->append_attribute(doc1.allocate_attribute("dst", node_wr5_name2));
        char *node_wr5_name3 = doc1.allocate_string(std::to_string(0).c_str());
        node_wr5->append_attribute(doc1.allocate_attribute("delay", node_wr5_name3));
    }
    file_stored << doc1;
    file_stored.close();
    doc1.clear();
    return datafile;
}

std::string change_to_job(job job_i, std::string inputfile, system_config syst){
    std::string input_job = "data_job.xml";
    std::string inputfile_cp = "cp " + inputfile + " " + input_job;
    system(inputfile_cp.c_str());

    xml_document<char> doc;
    std::ifstream file(input_job);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node1 = root_node->first_node("module");
    for(; node1; node1 = node1->next_sibling()){
        xml_node<> *node2 = node1->first_node("partition");
        for(; node2; node2 = node2->next_sibling()){
            xml_node<> *node3 = node2->first_node("task");
            for(; node3; node3 = node3->next_sibling()){
                xml_attribute<> *attr = node3->first_attribute("id");
                if(atoi(attr->value()) != job_i.get_taskindex()){
                    continue;
                }
                else{
                    std::string s1 = std::to_string(job_i.get_bl());
                    char * text1 = doc.allocate_string(s1.c_str());
                    node3->first_attribute("offset")->value(text1);

                    // std::string s2 = std::to_string(syst.get_maj_fr());
                    // char * text2 = doc.allocate_string(s2.c_str());
                    // node3->first_attribute("period")->value(text2);

                    std::string s3 = std::to_string(job_i.get_br());
                    char * text3 = doc.allocate_string(s3.c_str());
                    node3->first_attribute("deadline")->value(text3);
                }
            }
        }
    }
    std::string data;
    rapidxml::print(std::back_inserter(data), doc);
    std::ofstream file2;
    file2.open(input_job.c_str());
    file2 << data;
    file2.close();
    return input_job;
}

std::string modify_inputfile_with_mess(std::string inputfile){
    std::string input_mess = "input_mess.xml";
    system_config syst(inputfile);
    std::vector<task> tasks;
    for(int i = 0; i < syst.get_number_of_tasks(); i++){
        tasks.push_back(task(i, inputfile));
    }
    std::vector<message> messages;
    for(int i = 0; i < syst.get_number_of_messages(); i++){
        messages.push_back(message(i, inputfile));
    }


    std::vector<partition> partitions;
    for(int i = 0; i < syst.get_number_of_partitions(); i++){
        partitions.push_back(partition(i, inputfile));
    }


    std::string inputfile_cp = "cp " + inputfile + " " + input_mess;
    system(inputfile_cp.c_str());


    xml_document<char> doc;
    std::ifstream file(input_mess);
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);


    xml_node<> *root_node = doc.first_node("system");
    int tasks_numb = syst.get_number_of_tasks();
    int max_prio = syst.get_number_of_messages() + 1;
    int prio = 0;
    for(auto i: messages){
        int part_id = tasks[i.get_src()].get_partitionid();
        xml_node<> *node_copy = root_node->first_node("partition");
        for(; node_copy; node_copy = node_copy->next_sibling()){
            xml_attribute<> *attr = node_copy->first_attribute();
            if(std::string(attr->name()) != "src"){
                if(atoi(attr->value()) == part_id){
                    xml_node<> *node1 = doc.allocate_node(node_element, "task");
                    node_copy->prepend_node(node1);
                    std::string name1 = std::to_string(tasks_numb++);
                    char *node1_name1 = doc.allocate_string(name1.c_str());
                    node1->append_attribute(doc.allocate_attribute("id", node1_name1));
                    std::string name2 = std::to_string(prio++);
                    char *node1_name2 = doc.allocate_string(name2.c_str());
                    node1->append_attribute(doc.allocate_attribute("prio", node1_name2));
                    std::string name3 = std::to_string(i.get_bctt());
                    char *node1_name3 = doc.allocate_string(name3.c_str());
                    node1->append_attribute(doc.allocate_attribute("bcet", node1_name3));
                    std::string name4 = std::to_string(i.get_wctt());
                    char *node1_name4 = doc.allocate_string(name4.c_str());
                    node1->append_attribute(doc.allocate_attribute("wcet", node1_name4));
                    std::string name5 = std::to_string(tasks[i.get_src()].get_period());
                    char *node1_name5 = doc.allocate_string(name5.c_str());
                    node1->append_attribute(doc.allocate_attribute("period", node1_name5));
                    std::string name6 = std::to_string(tasks[i.get_src()].get_deadline());
                    char *node1_name6 = doc.allocate_string(name6.c_str());
                    node1->append_attribute(doc.allocate_attribute("deadline", node1_name6));
                }
            }
            else{
                if(atoi(attr->value()) == i.get_src()){
                    if(atoi(node_copy->first_attribute("dist")->value()) == i.get_dist()){
                        node_copy->remove_last_attribute();
                        node_copy->remove_last_attribute();
                        std::string name1 = "0";
                        char *node_copy_name1 = doc.allocate_string(name1.c_str());
                        node_copy->append_attribute(doc.allocate_attribute("bctt", node_copy_name1));
                        char *node_copy_name3 = doc.allocate_string(name1.c_str());
                        node_copy->append_attribute(doc.allocate_attribute("wctt", node_copy_name3));
                        std::string name2 = std::to_string(tasks_numb - 1);
                        char *node_copy_name2 = doc.allocate_string(name2.c_str());
                        node_copy->first_attribute("dist")->value(node_copy_name2);
                        xml_node<> *node2 = doc.allocate_node(node_element, "tlink");
                        root_node->append_node(node2);
                        char *node2_name1 = doc.allocate_string(name2.c_str());
                        node2->append_attribute(doc.allocate_attribute("src", node2_name1));
                        std::string name3 = std::to_string(i.get_dist());
                        char *node2_name2 = doc.allocate_string(name3.c_str());
                        node2->append_attribute(doc.allocate_attribute("dist", node2_name2));
                        char *node2_name3 = doc.allocate_string(name1.c_str());
                        node2->append_attribute(doc.allocate_attribute("bctt", node2_name3));
                        char *node2_name4 = doc.allocate_string(name1.c_str());
                        node2->append_attribute(doc.allocate_attribute("wctt", node2_name4));
                    }
                }
            }
        }
    }
    xml_node<> *node = root_node->first_node("partition");
    for(; node; node = node->next_sibling("partition")){
        xml_node<> *node1 = node->first_node("task");
        for(; node1; node1 = node1->next_sibling("task")){
            std::string s = std::to_string(atoi(node1->first_attribute("prio")->value()) + max_prio);
            char * text = doc.allocate_string(s.c_str());
            node1->first_attribute("prio")->value(text);
        }
    }


    std::string data;
    rapidxml::print(std::back_inserter(data), doc);
    std::ofstream file2;
    file2.open(input_mess.c_str());
    file2 << data;
    file2.close();
    return input_mess;
}



int main(){
    int WCRT = 0;
    std::string path1;
    std::cout << "enter input file name: ";
    std::cin >> path1;
    std::string path_an_ts = modify_inputfile_with_mess(path1);
    std::string path2 = create_datafile_new(path_an_ts);
    std::cout << "enter task number: ";
    int task_number;
    std::cin >> task_number;
    task tmp1(task_number, path1.c_str());
    solution tmp2(task_number);
    tmp2.get_lower_estimate(path2, path_an_ts);
    std::cout << "lower and upper: \n";
    tmp2.get_upper_estimate(path_an_ts, task_number);
    std::cout << "lower and upper: " << tmp2.get_le() << " " << tmp2.get_ue() << "\n";

    system_config sys(path1.c_str());
    std::cout << sys.get_number_of_partitions() << ", " << sys.get_number_of_messages() << std::endl;
    message mes(0, path1.c_str());
    std::cout << mes.get_dist() << std::endl;

    tmp1.find_anomaltasks(path2);

    std::vector<task> anom_tasks;
    if(tmp1.get_anomaltasks().size() != 0){
        std::vector<int> anom_t_ind = tmp1.get_anomaltasks();
        // std::cout << anom_t_ind[0] << "\n";
        for(int i = 0; i < anom_t_ind.size(); i++){
            anom_tasks.push_back(task(anom_t_ind[i], path_an_ts));
        }
    }
    double leng = 1;
    for(int i = 0; i < anom_tasks.size(); i++){
        int bcet = anom_tasks[i].get_bcet();
        // std::cout << anom_tasks[i].get_bcet() << std::endl;
        int wcet = anom_tasks[i].get_wcet();
        leng *= (wcet - bcet + 1);
        // std::cout << leng << " \n";
    }
    std::cout << "размерность пространства: " << leng << "\n";
    std::cout << "anom tasks size: " << tmp1.get_anomaltasks().size() << std::endl;
    std::vector<task> anom_tasks1;
    std::vector<solution> anom_sol;
    auto started = std::chrono::high_resolution_clock::now();
    if(tmp1.get_anomaltasks().size() != 0){
        std::vector<int> anom_t_ind = tmp1.get_anomaltasks();
        std::cout << anom_t_ind[0] << "\n";
        for(int i = 0; i < anom_t_ind.size(); i++){
            anom_tasks1.push_back(task(anom_t_ind[i], path_an_ts));
        }
        for(int i = 0; i < anom_t_ind.size(); i++){
            anom_sol.push_back(solution(anom_t_ind[i]));
        }
        std::cout << "here\n";

        std::vector<job> jobs_of_task;
        int border_left = 0;
        int border_right = tmp1.get_period();
        for(int i = 0; i < sys.get_maj_fr()/tmp1.get_period(); i++){
            jobs_of_task.push_back(job(tmp1.get_taskindex(), path_an_ts, border_left, border_right));
            border_left += tmp1.get_period();
            border_right += tmp1.get_period();
        }
        std::cout << "here1\n";
        int wcrt_job;
        for(int i = 0; i < jobs_of_task.size(); i++){
            std::string data_job = change_to_job(jobs_of_task[i], path2, sys);
            std::cout << "here2\n";
            find_solution(jobs_of_task[i], tmp2, anom_tasks1, anom_sol, wcrt_job, path_an_ts, data_job);
            if(wcrt_job > WCRT){
                WCRT = wcrt_job;
            }
            std::cout << "wcrt job " << i+1 << ": " << wcrt_job << "\n";
        }
    }
    else{
        WCRT = tmp2.get_le();
    }
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << "anom tasks size: " << tmp1.get_anomaltasks().size() << std::endl;
    std::cout << "lower and upper: " << tmp2.get_le() << " " << tmp2.get_ue() << "\n";
    std::cout << "WCRT: " << WCRT << std::endl;
    std::cout << "time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << " ms" << std::endl;
}