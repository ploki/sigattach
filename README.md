0. INTRO
========

How many times did you scream at your randomly-crashing-program, when it occurs after an unpredictable amount of time?

Especially, you can't always run it into valgrind (this awesome tool), gdb or linked with libefence, because of obvious performance issues.

This is where the black magic rises, thanks to ggim (one of my excellent colleagues, https://github.com/ploki). The idea is just to preload a shared object which will spawn a gdb if some specific signals occur.

Hijack the regular signal() function and make it do nothing, so that if the debugged program sets its own handlers, it will just be a NOP. Then set the "true" signal() (direclty loaded from the libc) handler such that is runs gdb on the current process.

1. OUTRO
========

Have fun!