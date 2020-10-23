#ifdef _WIN32
#define OS 0

#elif __linux__ 
#define OS 1

#endif

int main() {
	if (OS == 1) {
		puts("We are on linux");
	}
	else {
		puts("We are on windows");
	}
}
