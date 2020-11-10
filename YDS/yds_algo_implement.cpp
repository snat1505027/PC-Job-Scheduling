#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<utility>
#include<algorithm>
#include<vector>
using namespace std;

int loop = 0;

class task {
public:
	int id;
	float release, deadline, load;
	float offset;
	task() {
		id = release = deadline = load = offset = -1;
	}
	task(int id, float start, float end, float load) {
		this->id = id;
		this->release = start;
		this->deadline = end;
		this->load = load;
		offset = 0.0;
	}
	bool operator==(task ob);
	void reschedule(float start, float end) {
		float diff = end - start;
		if (release >= end) {
			deadline = deadline - diff;
			release = release - diff;
			offset += diff;
			return;
		}
		if (deadline <= start) {
			return;
		}
		if (release<start && deadline>end) {
			deadline -= diff;
			return;
		}
		if (deadline > start && release < start) {
			deadline = start;
			return;
		}
		if (release < end && deadline > end) {
			offset += diff;
			release = start;
			deadline -= diff;
			return;
		}
	}
};
bool task::operator==(task ob) {
	return (id == ob.id) && (release == ob.release) && (deadline == ob.deadline) && (load == ob.load) && (offset == ob.offset);
}
class Interval {
public:
	float start, end;
	int size;
	task *tasks;
	float intensity;
	Interval(float st,float e, task ts[],int len){
		start = st;
		end = e;
		tasks = new task[1000];
		size = len;
		float temp = 0.0;
		for (int i = 0; i < size; i++) {
			tasks[i] = ts[i];
			temp += ts[i].load;
		}
		intensity = temp / (end - start);
	}
};

class Execution {
public:
	float start, end;
	int id;
	float frequency;
	Execution() {
		start = end = id = frequency = -1;
	}
	Execution(int nr, float st,float ed,float freq) {
		id = nr;
		start = st;
		end = ed;
		frequency = freq;
	}
};
vector<Interval> get_intervals(task *ts,int len) {
	float releases[1000];
	float deads[1000];
	for (int i = 0; i < len; i++) {
		releases[i] = ts[i].release;
		deads[i] = ts[i].deadline;
	}

	vector <Interval> intervals;
	task *contained;
	contained = new task[1000];
	loop = 0;
	
	for (int start = 0; start < len;start++ ) {
		for (int end = 0; end < len;end++) {
			if (releases[start] < deads[end]) {
				int k = 0;
				for (int i = 0; i < len; i++) {
					if (ts[i].release >= releases[start] && ts[i].deadline <= deads[end]) {
						contained[k++] = ts[i];
					}
				}
				loop++;
				intervals.push_back(Interval(releases[start], deads[end], contained,k));
			}
		}
	}
	return intervals;
}

Interval max_intensity_interval(vector<Interval>& intervals) {
	int maxim = -1,ind=-1;
	
	for (int i = 0; i < intervals.size(); i++) {
		if (intervals[i].intensity > maxim) {
			maxim = intervals[i].intensity;
			ind = i;
		}
	}
	return intervals[ind];
}

bool edf_sort(task const& a, task const& b) {
	return a.deadline < b.deadline;
}
bool start_sort(Execution const& a, Execution const& b) {
	return a.start < b.start;
}
Execution* calc_scheduling(task *tasks,int len){
	Execution *scheduling;
	scheduling = new Execution[1000];
	int len_sq = 0;
	
	while (len > 0) {
		vector<Interval>intervals = get_intervals(tasks,len);
		Interval max_int = max_intensity_interval(intervals);
		Interval to_execute = max_intensity_interval(intervals);
		sort(to_execute.tasks, to_execute.tasks + to_execute.size,edf_sort);
		/*cout << "tasks in to_execute---------------" << endl;
		for (int i = 0; i < to_execute.size; i++) {
			cout << to_execute.tasks[i].id << " " << to_execute.tasks[i].release << " " <<
				to_execute.tasks[i].deadline << " " << to_execute.tasks[i].load << endl;
		}*/

		float int_length = float(max_int.end - max_int.start);
		float offset = float(max_int.start);
		float c_sum = to_execute.intensity*(to_execute.end - to_execute.start);
		
		for (int k = 0; k < to_execute.size; k++) {
			float task_length = (to_execute.tasks[k].load / c_sum) * int_length;
			Execution exec = Execution(to_execute.tasks[k].id, offset + to_execute.tasks[k].offset,
				offset + to_execute.tasks[k].offset + task_length, max_int.intensity);
			scheduling[len_sq++] = exec;
			offset += task_length;
			int idx;
			for (int pp = 0; pp < len; pp++) {
				if (tasks[pp] == to_execute.tasks[k]) {
					idx = pp;
					break;
				}
			}
			for (int pp = idx + 1; pp < len; pp++) {
				tasks[pp - 1] = tasks[pp];
			}
			len--;
		}
		
		for (int pp = 0; pp < len; pp++) {
			tasks[pp].reschedule(max_int.start,max_int.end);
		}
		cout << endl;
		cout << "rescheduled tasks----" << endl;
		
		for (int loo = 0; loo < len; loo++) {
			cout << tasks[loo].id << " " << tasks[loo].release << " " << tasks[loo].deadline << " " << tasks[loo].load << endl;
		}
	}
	
	sort(scheduling, scheduling+len_sq,start_sort);
	return scheduling;
}
int main() {
	freopen("input.txt", "r", stdin);
	int num_of_tasks;
	cin >> num_of_tasks;
	task tasks[10000];
	for (int i = 0; i < num_of_tasks; i++) {
		float start, end, load;
		cin >> start >> end >> load;
		tasks[i] = task(i + 1, start, end, load);
	}
	
	Execution *scheduling = calc_scheduling(tasks,8);
	cout << endl;
	cout << "Results----" << endl<<endl;
	
	for (int i = 0; i < 8; i++) {
		cout << "ID: " << scheduling[i].id << " ; START: " << scheduling[i].start << " ; FINISH: " << scheduling[i].end << " ; FREQUENCY: " << scheduling[i].frequency << endl;
	}
	
	return 0;
}