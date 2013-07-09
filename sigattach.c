#include <dlfcn.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * (utterly) Quick and dirty hack from an original idea of ggim (http://github.com/ploki)
 *
 * First, compile the stuff, e.g.:
 *
 * gcc -Wall -shared -fPIC -ggdb3 -O0 sigattach.c -o libsigattach.so
 *
 * Then:
 *
 * $ LD_PRELOAD=/path/to/libsigattach.so ./binary-to-debug
 *
 */

static void
handler(int sig)
{
        char cmdline[512];
        unsetenv("LD_PRELOAD");
        snprintf(cmdline, sizeof cmdline, "gdb /proc/%i/exe %i", getpid(), getpid());
        system(cmdline);
        exit(0);
}


void
signal(int sig,
       void (*func)(int))
{
        /* do nothing */
}

__attribute__((constructor)) void
init(void)
{
        void *handle = NULL;
        void (*real_signal)(int, void (*sighandler)(int)) = NULL;

        handle = dlopen("libc.so.6", RTLD_NOW | RTLD_GLOBAL);
        if (! handle) {
                perror("dlopen");
                _exit(1);
        }

        *(void **) (&real_signal) = dlsym(handle, "signal");
        if (! real_signal) {
                perror("dlsym");
                _exit(1);
        }

        real_signal(6,  handler); /* SIGABRT */
        real_signal(7,  handler); /* SIGBUS */
        real_signal(8,  handler); /* SIGFPE */
        real_signal(11, handler); /* SIGSEGV */
        real_signal(13, handler); /* SIGPIPE */
}
