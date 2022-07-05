#include <iostream>
#include "rapidxml/rapidxml.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <charconv>
#include "task.h"
using namespace rapidxml;
task::task(int num, std::string inputfile){
    xml_document<char> doc;
    std::ifstream file(inputfile.c_str());
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
                if(atoi(attr->value()) == num){
                    this->taskindex = atoi(node1->first_attribute("id")->value());
                    this->wcet = atoi(node1->first_attribute("wcet")->value());
                    this->bcet = atoi(node1->first_attribute("bcet")->value());
                    this->prio = atoi(node1->first_attribute("prio")->value());
                    this->period = atoi(node1->first_attribute("period")->value());
                    this->procnum = atoi(node->first_attribute("proc")->value());
                    this->deadline = atoi(node1->first_attribute("deadline")->value());
                    this->partition_id = atoi(node->first_attribute("id")->value());
                }
            }
        }
    }
}
int task::get_taskindex(){
    return this->taskindex;
}
int task::get_deadline(){
    return this->deadline;
}
int task::get_wcet(){
    return this->wcet;
}
int task::get_bcet(){
    return this->bcet;
}
int task::get_prio(){
    return this->prio;
}
int task::get_period(){
    return this->period;
}
int task::get_procnum(){
    return this->procnum;
}
int task::get_partitionid(){
    return this->partition_id;
}
void task::find_anomaltasks(std::string inputfile){
    std::string create_input = std::string("python3 ../CourseworkPubl-master/datamaker/adapter.py ./") + inputfile;
    system(create_input.c_str());
    std::string path = std::string("python3 ../CourseworkPubl-master/mainBS.py ./input_anom.xml")/*+ std::string(std::to_string(this->taskindex))*/ + std::string(" >output.txt");
    system(path.c_str());
    std::string buffer, num, set, lil_num;
    std::ifstream requests("./output.txt");
    std::set<int> Nums;
    std::vector<int> graph;
    std::vector<std::vector<int> > meta_graph;
    std::vector<std::set<int> > set_meta_graph;
    int count = 0;
    while(std::getline(requests, buffer)){
        if(!(buffer.empty())){
            for(count = 0; buffer[count] != ' '; count++){
                num += buffer[count];
            }
            count++;
            if(buffer[count] == 's'){
                for( ; buffer[count] != ')'; count++){
                    set += buffer[count];
                }
                set += buffer[count];
                meta_graph.push_back(std::vector<int>());
            }
            else{
                if(buffer[count] == '{'){
                    for( ; buffer[count] != '}'; count++){
                        set += buffer[count];
                        if((buffer[count] <= '9') && (buffer[count] >= '0')){
                            lil_num += buffer[count];
                        }
                        else if((buffer[count] == ',') || (buffer[count] == '}')){
                            Nums.insert(std::stoi(lil_num));
                            graph.push_back(std::stoi(lil_num));
                            lil_num = "";
                        }
                    }
                    graph.push_back(std::stoi(lil_num));
                    set += buffer[count];
                    lil_num = "";
                }
                meta_graph.push_back(graph);
                set_meta_graph.push_back(Nums);
            }
            num = "";
            set = "";
            Nums.clear();
            graph.clear();
        }

    }
    auto i = meta_graph.begin();
    for( ; i != meta_graph.begin() + this->taskindex; i++);
        for(auto j = (*i).begin(); j != (*i).end(); j++){
            this->anomaltasks.push_back((*j));
        }
    for(int i = 0; i < anomaltasks.size(); i++){
        std::cout << anomaltasks[i] << " ";
    }
    std::cout << "\n";
}
std::vector<int> task::get_anomaltasks(){
    return this->anomaltasks;
}


job::job(int num, std::string inputfile, int border_left, int border_right): task(num, inputfile){
    this->border_left = border_left;
    this->border_right = border_right;
}
int job::get_bl(){
    return border_left;
}
int job::get_br(){
    return border_right;
}



system_config::system_config(std::string path){
    xml_document<char> doc;
    std::ifstream file(path.c_str());
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    this->partitions = 0;
    this->tasks = 0;
    this->messages = 0;
    for(int i = 0; node; i++){
        if(node->first_attribute()->name() != std::string("src")){
            this->partitions++;
        }
        xml_node<> *node1 = node->first_node();
        for(int i = 0; node1; i++){
            if(node1->first_attribute()->name() != std::string("start")){
                this->tasks++;
            }
            node1 = node1->next_sibling();
        }
        if(node->first_attribute()->name() == std::string("src")){
            this->messages++;
        }
        node = node->next_sibling();
        
    }
    xml_node<> *node2 = root_node->first_node();
    this->maj_fr = atoi(node2->first_attribute("maj_fr")->value());
}
int system_config::get_number_of_tasks(){
    return this->tasks;
}
int system_config::get_maj_fr(){
    return this->maj_fr;
}
int system_config::get_number_of_partitions(){
    return this->partitions;
}
int system_config::get_number_of_messages(){
    return this->messages;
}



partition::partition(int num, std::string inputfile){
    xml_document<char> doc;
    std::ifstream file(inputfile.c_str());
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    for(int i = 0; i < num; i++){
        node = node->next_sibling();
    }
    this->partition_id = atoi(node->first_attribute("id")->value());
    this->procnum = atoi(node->first_attribute("proc")->value());
    this->maj_fr = atoi(node->first_attribute("maj_fr")->value());
    xml_node<> *node_tasks = node->first_node();
    for(; node_tasks; node_tasks = node_tasks->next_sibling()){
        xml_attribute<> *attr = node_tasks->first_attribute();
        if(std::string(attr->name()) == "id"){
            this->tasks.push_back(atoi(attr->value()));
        }
        else{
            window.push_back(std::vector<int>{atoi(node_tasks->first_attribute("start")->value()),atoi(node_tasks->first_attribute("stop")->value())});
        }
    }
}
int partition::get_part_id(){
    return this->partition_id;
}
int partition::get_procnum(){
    return this->procnum;
}
int partition::get_maj_fr(){
    return this->maj_fr;
}
std::vector<int> partition::get_tasks(){
    return this->tasks;
}
std::vector<std::vector<int>> partition::get_window(){
    return this->window;
}


message::message(int num, std::string inputfile){
    xml_document<char> doc;
    std::ifstream file(inputfile.c_str());
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    xml_node<> *root_node = doc.first_node("system");
    xml_node<> *node = root_node->first_node();
    for(; node; node = node->next_sibling()){
        xml_attribute<> *attr = node->first_attribute();
        if(std::string(attr->name()) != "src"){
            continue;
        }
        else{
            break;
        }
    }
    for(int i = 0; i < num; i++){
        node = node->next_sibling();
    }
    this->index = num;
    this->src = atoi(node->first_attribute("src")->value());
    this->dist = atoi(node->first_attribute("dist")->value());
    this->wctt = atoi(node->first_attribute("wctt")->value());
    this->bctt = atoi(node->first_attribute("bctt")->value());
}
int message::get_src(){
    return this->src;
}
int message::get_dist(){
    return this->dist;
}
int message::get_bctt(){
    return this->bctt;
}
int message::get_wctt(){
    return this->wctt;
}
int message::get_index(){
    return this->index;
}


message_instance::message_instance(int num, std::string inputfile, int border_left, int border_right): message(num, inputfile){
    this->border_left = border_left;
    this->border_right = border_right;
}
int message_instance::get_bl(){
    return border_left;
}
int message_instance::get_br(){
    return border_right;
}