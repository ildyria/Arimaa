#include "tests.h"

void fun_test1 ()
 {
	v_stat vect1 = 
	{
		n_stat(6,p_stat(1,1)),
		n_stat(5,p_stat(1,2)),
		n_stat(4,p_stat(1,3)),
		n_stat(3,p_stat(1,4)),
		n_stat(2,p_stat(1,5)),
		n_stat(1,p_stat(1,6))
	};

	v_stat vect2 = 
	{
		n_stat(16,p_stat(1,5)),
		n_stat(12,p_stat(4,2)),
		n_stat(8,p_stat(7,3)),
		n_stat(6,p_stat(6,4)),
		n_stat(4,p_stat(1,5)),
		n_stat(2,p_stat(2,6))
	};


	std::cout << std::endl;
	std::cout << "vect1 :" << std::endl;
	std::cout << vect1;

	std::cout << std::endl;
	std::cout << "vect1 sorted second :" << std::endl;
	sort(vect1.begin(),vect1.end(),second);
	std::cout << vect1;

	std::cout << std::endl;
	std::cout << "vect1 sorted first :" << std::endl;
	sort(vect1.begin(),vect1.end(),first);
	std::cout << vect1;

	std::cout << std::endl;
	std::cout << "vect2 sorted first :" << std::endl;
	sort(vect2.begin(),vect2.end(),first);
	std::cout << vect2;

	v_stat v = vect1 + vect2;

	std::cout << std::endl;
	std::cout << "v = vect1 + vect2 :" << std::endl;
	std::cout << v;

	vect1 += vect2;

	std::cout << std::endl;
	std::cout << "vect1 += vect2 :" << std::endl;
	std::cout << vect1 << std::endl;
}