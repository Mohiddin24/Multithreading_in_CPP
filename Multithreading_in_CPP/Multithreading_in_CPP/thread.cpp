#include <iostream>
#include <thread>

void foo() {
	std::cout << "Hello from foo!" << std::this_thread::get_id() << std::endl;
}

class callable_class {
public:
	void operator()() {
		std::cout << "Hello from callable class!" << std::this_thread::get_id() << std::endl;
	}						
};

void run() {
	std::thread thread1(foo);
	callable_class obj;
	std::thread thread2(obj);
	std::thread thread3([] 
	{
		std::cout << "Hello from lambda!" << std::this_thread::get_id() << std::endl;
	});

	thread1.join();
	thread2.join();
	thread3.join();

	std::cout << "Hello from main !" << std::this_thread::get_id() << std::endl;
}

int main() {
	run();
	return 0;
}	