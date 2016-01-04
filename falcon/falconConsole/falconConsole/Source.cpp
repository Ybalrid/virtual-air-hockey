#include <iostream>

void pause()
{
	std::cout << "Press RETURN to continue" << std::endl;
	std::cin.get();
}
int main(void)
{
	pause();
	return 0;
}