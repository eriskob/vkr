#include "CS.h"

using namespace std;

namespace CS {

CS::CS(string _name, Network *n, int p, bool __t, CoreSchedulerData* _data
, vector <Channel*> &_wakeup, vector <Channel*> &_sleep
): Automaton(n, _name, p, __t), data(_data), wakeup(_wakeup), sleep(_sleep), current_window(n, "CS_current_window"), cycle_num(n, "CS_cycle_num"), current_partition(n, "CS_current_partition"), time(n, "CS_time")


{
	PreActive* preactive = new PreActive (n, "PreActive", false, false, this);
	addLocation(preactive);
	GetPartition* getpartition = new GetPartition (n, "GetPartition", true, true, this);
	addLocation(getpartition);
	STOP* stop = new STOP (n, "STOP", false, true, this);
	addLocation(stop);
	Active* active = new Active (n, "Active", false, false, this);
	addLocation(active);

	GetPartition_STOP_0 *getpartition_stop_0 = new GetPartition_STOP_0 (n, stop, this);
	getpartition->addTransition(getpartition_stop_0);
	Active_GetPartition_1 *active_getpartition_1 = new Active_GetPartition_1 (n, getpartition, this);
	active->addTransition(active_getpartition_1);
	PreActive_Active_2 *preactive_active_2 = new PreActive_Active_2 (n, active, this);
	preactive->addTransition(preactive_active_2);
	GetPartition_PreActive_3 *getpartition_preactive_3 = new GetPartition_PreActive_3 (n, preactive, this);
	getpartition->addTransition(getpartition_preactive_3);

	automaton = this;
	automaton->timers.push_back(&time);


}

}

