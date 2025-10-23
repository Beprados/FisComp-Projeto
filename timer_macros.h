#ifndef TIMER_MACROS_FILE
#define TIMER_MACROS_FILE

#define TIMER(name) ScopedTimer __timer; timer_start(&__timer, name)
#define TIMER_STOP() timer_stop(&__timer)
#define TIMER_AUTO() ScopedTimer __timer; timer_start(&__timer, __func__)

#endif  