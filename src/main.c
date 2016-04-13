int main(int argc, char* argv[]) {
	char* screen = 0xb8000 + 1988;
	char str[6] = "Hello";
	for(int i=0; i<5; ++i) {
		screen[2*i] = str[i];
		screen[2*i+1] = 0x1f;
	}
	return 0;
}