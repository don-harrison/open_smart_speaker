#include <sys/stat.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdint.h>

caddr_t _sbrk(int incr) {
    extern unsigned long _heap_start;
    extern unsigned long _heap_end;
    static unsigned char *heap = 0;
    unsigned char *prev;

    if (heap == 0) heap = (unsigned char *)&_heap_start;
    prev = heap;

    if (heap + incr > (unsigned char *)&_heap_end) {
        errno = ENOMEM;
        return (caddr_t)-1;
    }

    heap += incr;
    return (caddr_t)prev;
}

int _write(int file, const char *ptr, int len) { (void)file; (void)ptr; return len; }
int _read(int file, char *ptr, int len) { (void)file; (void)ptr; (void)len; return 0; }
int _close(int file) { (void)file; return -1; }
int _fstat(int file, struct stat *st) { (void)file; st->st_mode = S_IFCHR; return 0; }
int _isatty(int file) { (void)file; return 1; }
int _lseek(int file, int ptr, int dir) { (void)file; (void)ptr; (void)dir; return 0; }
void _exit(int status) { (void)status; while (1) {} }
int _kill(int pid, int sig) { (void)pid; (void)sig; return -1; }
int _getpid(void) { return 1; }