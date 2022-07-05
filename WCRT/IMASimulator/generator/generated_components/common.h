#ifndef __COMMON__H_
#define __COMMON__H_

#include <vector>
#include <string>
#include "network.h"
#include "channel.h"
#include "timer.h"
#include "var.h"

#define DUMMY_NUM 100000000

typedef struct {
    int delay;
} LinkData;


typedef struct {
    int start;
    int stop;
    int partitionId;
} WindowData;

typedef struct {
std::vector <WindowData> windows;
    int numOfWindows;
    int majorFrame;
} CoreSchedulerData;

typedef struct {
    int id;
    int wcet;
    int prio;
    int offset;
    int period;
    int deadline;

    int numOfInputLinks;
} TaskData;

typedef struct {
    int numOfTasks;
} TaskSchedulerData;


void createCS (std::string type, std::string _name, Network *n, int p, bool t, CoreSchedulerData* _data, std::vector <Channel*> &_wakeup, std::vector <Channel*> &_sleep);
void createTS (std::string type, std::string _name, Network *n, int p, bool t, TaskSchedulerData* _data, std::vector <Var*> &_abs_deadline, std::vector <Var*> &_is_ready, std::vector <Var*> &_prio, Channel* _wakeup, Channel* _sleep, Channel* _ready, Channel* _finished, std::vector <Channel*> &_exec, std::vector <Channel*> &_preempt);
void createL (std::string type, std::string _name, Network *n, int p, bool t, LinkData* _data, Var* _is_data_ready, Channel* _send, Channel* _receive);
void createT (std::string type, std::string _name, Network *n, int p, bool t, TaskData* _data, Var* _abs_deadline, Var* _is_ready, Var* _is_failed, Var* _prio, std::vector <Var*> &_is_data_ready, Channel* _exec, Channel* _preempt, Channel* _ready, Channel* _finished, Channel* _send, Channel* _receive);

#endif
