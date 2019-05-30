#include <stdio.h>

//原程序zend/ zend_mm_small_size_to_bin()
void test2(int size)
{
	if (size<=64) {
		printf("%d\n", ((size-!!size)>>3));
	}else{
		//不讨论
	}
}

void test1()
{
	printf("%d\n",!0);//1
	printf("%d\n",!1);//0
	printf("%d\n",!100);//0
	printf("%d\n",!-100);//0
}

int main(int argc, const char *argv[])
{
	//test1();
	test2(0);//0
	test2(1);//0
	test2(7);//0
	test2(15);//1
	return 0;
}
