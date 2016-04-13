#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>

#undef errno
extern int errno;

register char * stack_ptr asm("sp");

char *__env[1] = { 0 };
char **environ = __env;

void __stack_chk_fail() {
	//while(1);
	write(0, "fail", 4);
}

int _getpid(void)
{
	return 1;
}

int getpid() {
	return _getpid();
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

int kill(int pid, int sig) {
	return _kill(pid, sig);
}

void _exit (int status)
{
	_kill(status, -1);
	while (1) {}		/* Make sure we hang here */
}

int col = 0;
int row = 0;
int color = 0x1f;
#define MAX_COL 80
#define MAX_ROW 25

void write_char(char c) {
	if(c == '\n') {
		col = 0;
		++row;
	}
	else if(c == '\r') {
		++row;
	}
	else {
		int offset = MAX_COL*row + col;
		char* screen = (char*)(0xb8000);

		screen[2*offset] = c;
		screen[2*offset+1] = color;

		++col;
		if(col >= MAX_COL) {
			col = 0;
			++row;
		}
	}
	if(row >= MAX_ROW)
		row = 0;
}

void clear_screen() {
	col = 0;
	row = 0;
	int col_bak = color;
	color = 0x00;
	for(int r=0; r<MAX_ROW; ++r)
		for(int c=0; c<MAX_COL; ++c)
			write_char(' ');
	color = col_bak;
}

int _write(int file, char *ptr, int len)
{
	int todo;
	//write_char('.');
	for (todo = 0; todo < len; todo++)
	{
		write_char(*ptr++);
	}

	/* Implement your write code here, this is used by puts and printf for example */
	return len;
}

int write(int file, char *ptr, int len) {
	return _write(file, ptr, len);
}

caddr_t _sbrk(int incr) {
  extern char heap_low;		/* Defined by the linker */
  extern char heap_top;
  static char *heap_end;
  char *prev_heap_end;
  write(0, "sbrk:", 5);
  if (heap_end == 0) {
    heap_end = &heap_low;
  }
  prev_heap_end = heap_end;
  if (heap_end + incr > &heap_top) {
	/*char str[20];
	itoa((int) &heap_low , str, 10);
	int l = strlen(str);
	str[l] = ' ';
	itoa((int) &heap_top, &str[l+1], 10);
	write(0, str, strlen(str));*/
    write(0, "Heap and stack collision\n", 25);
    abort();
  }

  char add[10];
  itoa(incr, add, 10);
  write(0, add, strlen(add)); 
  heap_end += incr;
  return (caddr_t) prev_heap_end;
}
/*
caddr_t _sbrk(int incr)
{
	extern char heap_low;
	extern char heap_top;
	static char *heap_end = 0;
	char *prev_heap_end;
	extern char _end;

	//write(0, "---------------sbrk1", 20);

	if (heap_end == 0)
		heap_end = &heap_low;

	
	char str[20];
	itoa((int) stack_ptr, str, 16);
	int l = strlen(str);
	str[l] = ' ';
	itoa((int) heap_end, &str[l+1], 16);
	write(0, str, strlen(str));
	while(1);
	
	//write(0, "---------------sbrk2", 20);
	prev_heap_end = heap_end;
	if (heap_end + incr > &heap_top)
	{
//		write(1, "Heap and stack collision\n", 25);
//		abort();
		errno = ENOMEM;
		return (caddr_t) 0;
	}

	//write(0, "---------------sbrk3", 20);
	heap_end += incr;

	return (caddr_t) prev_heap_end;
}
*/
caddr_t sbrk(int incr) {
	return _sbrk(incr);
}

int _close(int file)
{
	return -1;
}

int close(int file) {
	return _close(file);
}

int _fstat(int file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int fstat(int file, struct stat *st) {
	return _fstat(file, st);
}

int _isatty(int file)
{
	return 1;
}

int isatty(int file) {
 return _isatty(file);
}

int _lseek(int file, int ptr, int dir)
{
	return 0;
}

int lseek(int file, int ptr, int dir) {
	return _lseek(file, ptr, dir);
}

int _read(int file, char *ptr, int len)
{
	return 0;
}

int read(int file, char *ptr, int len) {
	return _read(file, ptr, len);
}

int _open(char *path, int flags, ...)
{
	/* Pretend like we always fail */
	return -1;
}

int _wait(int *status)
{
	errno = ECHILD;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _times(struct tms *buf)
{
	return -1;
}

int _stat(char *file, struct stat *st)
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _fork(void)
{
	errno = EAGAIN;
	return -1;
}

int _execve(char *name, char **argv, char **env)
{
	errno = ENOMEM;
	return -1;
}

