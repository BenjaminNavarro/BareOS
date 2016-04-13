#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	clear_screen();
	char* buf = (char*) malloc(10*sizeof(char));
	//char buf[10];
	snprintf(buf, 10, "\nHello!");
	write(0, buf, 7);
	//printf("\nHello2");
	//write(0," Hello!!!", 9);
	/*if(buf == NULL) {
		printf("malloc failed!");
	}
	else {
		printf("buf=%p", buf);
		sprintf(buf, "Hello %d!", 42);
		printf("        %s", buf);
	}*/
	return 0;
}
