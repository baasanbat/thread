#include <iostream>
#include <queue>
#include <condition_variable>
#include <ctime>
#include <cstdlib>
#include <mutex>
#include <thread>
using namespace std;

class log {
	string logg;
	mutex mut;
public:
	log() {
		logg = "";
	}
	
	void add(const string str) {
		lock_guard <mutex> lk(mut);
		logg += str;
	}

	string to_string() {
		lock_guard <mutex> lk(mut);
		return logg;
	}
};

mutex mut;
queue <int> data_queue;
condition_variable data_cond;
bool last = false;
int sum = 0;
log my_log;

void data_preparation_thread() {
	int total = rand()%4 + 1;
	my_log.add("total = " + to_string(total) + "\n");
	while (total--) {
		int data = rand()%10;
		my_log.add(to_string(data) + " ");
		lock_guard <mutex> lk(mut);
		data_queue.push(data);
		data_cond.notify_one();
	}
	last = 1;
}

void data_processing_thread() {
	while (1) {
		unique_lock <mutex> lk(mut);
		data_cond.wait(lk, [] {return !data_queue.empty();});
		int data = data_queue.front();
		data_queue.pop();
		lk.unlock();
		sum += data;
		if (data_queue.empty()) break;
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
//	data_queue.push(1);
	thread preparer(data_preparation_thread);
	thread preparer2(data_preparation_thread);
	thread processor(data_processing_thread);
	preparer.join();
	preparer2.join();
	processor.join();

	cout << my_log.to_string() << endl;
	cout << sum << endl;
	return 0;
}
