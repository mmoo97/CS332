#include <stdio.h>

int main() {
	char hello[]= "hello\0";
	printf("%c\n", hello);
	printf("%c", hello[4]);
}
