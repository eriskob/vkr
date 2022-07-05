#ifndef __CS__H_
#define __CS__H_

#include "automaton.h"
#include "common.h"

namespace CS {

class CS:public Automaton {
	CS *automaton;
public:
	CoreSchedulerData* data;





	std::vector <Channel*> &wakeup;
	std::vector <Channel*> &sleep;




	Var current_window;
	Var cycle_num;
	Var current_partition;

	Timer time;


	CS(std::string _name, Network *n, int p, bool t, CoreSchedulerData* _data, std::vector <Channel*> &_wakeup, std::vector <Channel*> &_sleep);






void get_current_partition()
{
    automaton->current_partition = (*(automaton->data)).windows[automaton->current_window].partitionId;
}

int start_time()
{
    return (*(automaton->data)).windows[automaton->current_window].start + automaton->cycle_num * (*(automaton->data)).majorFrame;
}

int stop_time()
{
    return (*(automaton->data)).windows[automaton->current_window].stop + automaton->cycle_num * (*(automaton->data)).majorFrame;
}

void get_next_window()
{
    if (automaton->current_window == (*(automaton->data)).numOfWindows -1) {
        automaton->current_window = 0;
        automaton->cycle_num = automaton->cycle_num + 1;
    }
    else
        automaton->current_window = automaton->current_window + 1;
}

};


class GetPartition_STOP_0: public Transition {
	CS* automaton;
	

public:
	GetPartition_STOP_0 (Network *_net, Location *_next, CS *_aut): Transition(_net, _next), automaton(_aut) {}

	virtual bool guard()
	{
		return automaton->time >= DUMMY_NUM;
	}





};

class Active_GetPartition_1: public Transition {
	CS* automaton;
	

public:
	Active_GetPartition_1 (Network *_net, Location *_next, CS *_aut): Transition(_net, _next), automaton(_aut)
	{
		is_sender = true;
	}

	virtual bool guard()
	{
		return automaton->time == automaton->stop_time();
	}



	virtual void action()
	{
		automaton->get_next_window();
	}

	virtual bool synchronize()
	{
		channel = &(*automaton->sleep[automaton->current_partition]);
		return Transition::synchronize();
	}

	virtual bool needSync()
	{
		return true;
	}

};

class PreActive_Active_2: public Transition {
	CS* automaton;
	

public:
	PreActive_Active_2 (Network *_net, Location *_next, CS *_aut): Transition(_net, _next), automaton(_aut)
	{
		is_sender = true;
	}

	virtual bool guard()
	{
		return automaton->time == automaton->start_time();
	}




	virtual bool synchronize()
	{
		channel = &(*automaton->wakeup[automaton->current_partition]);
		return Transition::synchronize();
	}

	virtual bool needSync()
	{
		return true;
	}

};

class GetPartition_PreActive_3: public Transition {
	CS* automaton;
	

public:
	GetPartition_PreActive_3 (Network *_net, Location *_next, CS *_aut): Transition(_net, _next), automaton(_aut) {}

	virtual bool guard()
	{
		return (*(automaton->data)).majorFrame!=0
&& automaton->time < DUMMY_NUM;
	}



	virtual void action()
	{
		automaton->get_current_partition();
	}


};


class PreActive: public Location {
	CS* automaton;
public:
	PreActive (Network *n, std::string _name, bool init, bool committed, CS *a): Location (n, _name, init, committed), automaton(a) {}
};

class GetPartition: public Location {
	CS* automaton;
public:
	GetPartition (Network *n, std::string _name, bool init, bool committed, CS *a): Location (n, _name, init, committed), automaton(a) {}
};

class STOP: public Location {
	CS* automaton;
public:
	STOP (Network *n, std::string _name, bool init, bool committed, CS *a): Location (n, _name, init, committed), automaton(a) {}
};

class Active: public Location {
	CS* automaton;
public:
	Active (Network *n, std::string _name, bool init, bool committed, CS *a): Location (n, _name, init, committed), automaton(a) {}
};


}

#endif
