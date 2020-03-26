#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <bits/sigaction.h>
static int count = 1;
void timer_handler(int signum)
{

    /*printf("timer expired %d times\n", ++count);*/

    if (count % 2 == 0)
    {
        printf("Tock\n");
    }
    else
    {
        printf("Tick");
        fflush(stdout);
    }
    count++;
}
int main()
{
    struct sigaction sa;
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGALRM, &sa, NULL);

    /* Configure the timer to expire after 250 msec... */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 500000;
    /* ... and every 250 msec after that. */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 500000;
    /* Start a virtual timer. It counts down whenever this process is
   executing. */

    setitimer(ITIMER_REAL, &timer, NULL);

    while (1)
        ;
}
/*
void handle_alarm(int signum)
{
    printf("here");
}

int main(int argc, char **argv)
{
    struct itimerval timer1;
    struct sigaction sa;

    sa.sa_handler = &handle_alarm;
    sigaction(SIGALRM, &sa, NULL);
    timer1.it_value.tv_sec = 1;
    timer1.it_value.tv_usec = 0;
    timer1.it_interval.tv_sec = 5;
    timer1.it_interval.tv_usec = 0;
    printf("Time value: %ld.%ld\n", timer1.it_value.tv_sec, timer1.it_value.tv_usec);
    if (setitimer(ITIMER_REAL, &timer1, NULL) == -1)
    {
        printf("Shit");
        return 0;
    }
    while (1)
    {
    }
}
*/