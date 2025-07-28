#include <iostream>
#include <chrono>
using namespace std;

int main()
{
	long long int s=0;
	auto start_time = std::chrono::steady_clock::now();

	for (long long int i=0; i < 1000000000; i++)
	{
		s+=i;
		if (i%10000000 == 0)
		{
			cout << "inside:" << s<< endl;
		}
	
	}

	cout << s << endl;
	auto end_time = std::chrono::steady_clock::now();
	auto duration = end_time - start_time;
	auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	std::cout << "Total loop duration: " << duration_ms.count() << " milliseconds." << std::endl;



return 0;
}


