/*
* Baasanbat Purevjal
* baasanbatp@gmail.com
* thread safe stack
* 08/31/16
*/

/*
* Problems with queue on thread programs:
*	empty() and size() can't be relied on
*	this is because lets say one thread checks
*	if it is empty while an another thread putting a 
*	data into it, there is chance that the first thread
*	could be treating the queue as empty..
*
*	another problem is between top() and pop()
*	consider following threads
*	|   Thread A				 |   Thread B				|
*   | if (!s.empty())            |							|
*	|							 | if (!s.empty())			|
*	|	const value = s.top()    |							|
*	|							 |	const value = s.top()	|
*	|	s.pop();				 |							|
*	|	do_something(value)	     |  s.pop();				|
*	|							 | do_something(value);		|
*   as you can see, each thread could be using a same value
*/

#include <stack>
#include <memory>
#include <thread>
#include <mutex>
#include <stack>

struct empty_stack: std::exception {
	const char* what() const throw();
};

template<typename T>
class threadsafe_stack {
private:
	std::stack<T> data;
	mutable std::mutex m; //mutex must be mutable
public:
	threadsafe_stack(){}
	threadsafe_stack(const threadsafe_stack& other) {
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data;
	}
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;

	void push(T new_value) {
		std::lock_guard<std::mutex> lock(m);
		data.push(new_value);
	}
	std::shared_ptr<T> pop() {
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) throw empty_stack();
		std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
		data.pop();
		return res;
	}
	bool empty() const {
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
};

int main(int argc, char *argv[]) {
	return 0;
}
