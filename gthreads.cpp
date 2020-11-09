#include <stdio.h>
#include <string.h>
#include <iostream>
#include <glib.h>
#include <time.h>
#include <unistd.h>

using namespace std;

#define tcount 5000
#define USLEEP_TIME 0

char array1[tcount] = {1};
char array2[tcount] = {2};
char array3[tcount] = {3};

int num_threads = 0;
GMutex mutex;
GCond cond;

void *thread_function (void *data)
{
    g_print ("side thread writes value 2 \n");
    //usleep (1000000);
    g_mutex_lock(&mutex);
    num_threads++;
    for (int i = 0; i < tcount; i++)
    {
        usleep (USLEEP_TIME);
        array1[i] = array2[i];
    }
    if (num_threads == 1)
        g_cond_signal(&cond);
    
    g_mutex_unlock(&mutex);

    return NULL;
}

int main ()
{
    int i;
    GThread *thread[1];
    memset (array1, 1, sizeof(char)*tcount);
    memset (array2, 2, sizeof(char)*tcount);
    memset (array3, 3, sizeof(char)*tcount);

    for (i=0;i<1;i++)
    {
        cout << "creating thread" << i << endl;
        thread[i] = g_thread_new("thread function", thread_function, NULL);
    }

    g_print ("main thread writes value 3\n");
    /*Note:
     * if g_cond_wait enters with mutex locked then it will first unlock the mutex and will wait on the condition.
     * if g_cond_wait enters with mutex unlocked then g_cond_wait will simply wait on the condition and will lock the mutex when it returns*/

    g_mutex_lock (&mutex);
    
    while (num_threads < 1)
        g_cond_wait (&cond,&mutex);

    for (int j = 0; j < tcount; j++)
    {
        usleep (USLEEP_TIME);
        array1[j] = array3[j];
    }
    g_mutex_unlock (&mutex);

    g_thread_join (thread[0]);

    if (memcmp (array1, array2, sizeof(char) * tcount) == 0)
        g_print ("arrays have value 2\n");
    else if (memcmp (array1, array3, sizeof (char) * tcount) == 0)
        g_print ("arrays have value 3\n");
    else
        g_print ("corruption\n");
}
