0. INTRO
========

How many times did you scream at your randomly-crashing-program, when it occurs after an unpredictable amount of time?

Especially, you can't always run it into valgrind (this awesome tool), gdb or linked with libefence, because of obvious performance issues.

This is where the black magic rises, thanks to ggim (one of my excellent colleagues, https://github.com/ploki). The idea is just to preload a shared object which will spawn a gdb if some specific signals occur.

Hijack the regular signal() function and make it do nothing, so that if the debugged program sets its own handlers, it will just be a NOP. Then set the "true" signal() (direclty loaded from the libc) handler such that is runs gdb on the current process.

Example:

        $ make
        gcc -Wall -shared -fPIC -ggdb3 -O0 sigattach.c -o libsigattach.so -ldl

        $ cat crash.c

        #include <stdio.h>
        #include <signal.h>

        #define FAR_AWAY 1000000

        static void
        handler(int sig)
        {
                printf("[*] In the regular handler\n");
        }

        int main(void)
        {
                char dummy[1];

                signal(SIGSEGV, handler);
                dummy[FAR_AWAY] = 'a';
                printf("%d\n", dummy[FAR_AWAY]);

                return 0;
        }

        $ LD_PRELOAD=./libsigattach.so ./crash
        [...]
        Reading symbols from /proc/10104/exe...done.
        Attaching to program: /proc/10104/exe, process 10104
        Reading symbols from ./libsigattach.so...done.
        Loaded symbols for ./libsigattach.so
        [...]
        0x00007f4837c2d44e in __libc_waitpid (pid=<optimized out>, stat_loc=0x7fff9eacf700, options=0) at ../sysdeps/unix/sysv/linux/waitpid.c:32
        32	../sysdeps/unix/sysv/linux/waitpid.c: No such file or directory.
        (gdb) bt
        #0  0x00007f4837c2d44e in __libc_waitpid (pid=<optimized out>, stat_loc=0x7fff9eacf700, options=0) at ../sysdeps/unix/sysv/linux/waitpid.c:32
        #1  0x00007f4837bb329e in do_system (line=0x7fff9eacf750 "gdb /proc/10104/exe 10104") at ../sysdeps/posix/system.c:149
        #2  0x00007f4837f449af in handler (sig=11) at sigattach.c:25
        #3  <signal handler called>
        #4  main () at crash.c:18
        (gdb) f 4
        #4  main () at crash.c:18
        18	        dummy[FAR_AWAY] = 'a';
        (gdb)


1. OUTRO
========

Have fun!