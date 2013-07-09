0. INTRO
========

How many times did you scream at your randomly-crashing-program, occuring after an unpredictable amount of time?

Especially, you can't always run it into valgrind (this awesome tool), gdb or linked with libefence, because of obvious performance issues.

This is where the black magic rises, thanks to ggim (one of my excellent colleagues). The idea is just to preload a shared object spawing gdb when some specific signals occur.

Hijack the regular signal() function, doing nothing, so that if the debugged program sets its own handlers, it will just be a NOP. Then set the "true" signal() (direclty loaded from the libc) handler consisting of spawning gdb on itself.

1. OUTRO
========

Have fun!