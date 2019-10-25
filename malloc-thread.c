#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>

#define GB 1024*1024*1024

void *simplemalloc(void *arg)
{
    char *p;

    p = (char *)malloc(GB*sizeof(char));
    memset(p, 0, GB*sizeof(char));

    pthread_exit(p);
}

int main(int argc, char *argv[])
{
    int i, thread_count=2, second=0;
    pthread_t thread[thread_count];
    void *status[thread_count];
    char cmd[256];

    sprintf(cmd, "grep VmSize /proc/%d/status 1>&2", getpid());
    fprintf(stderr, "Pre\n");
    system(cmd);

    fprintf(stderr, "Exec\n");
    for(i=0; i<thread_count; i++)
    {
        pthread_create(&thread[i], NULL, simplemalloc, (void *)NULL);
        system(cmd);
    }

    for(i=0; i<thread_count; i++)
        pthread_join(thread[i], &status[i]);

    sleep(second);

    for(i=0; i<thread_count; i++)
        free(status[i]);
    fprintf(stderr, "Post\n");
    system(cmd);

    return 0;
}
