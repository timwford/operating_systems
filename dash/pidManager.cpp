// sorry didn't want to find out which of these I actually needed lol

#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <pthread.h>


using namespace std;

#define PROCESS_MAX 500
#define PROCESS_MIN 300

const int processMin = PROCESS_MIN;
const int processMax = PROCESS_MAX;
int THREAD_COUNT = 100;
int pidMap[PROCESS_MAX + 1];
int lastUsedProcess;
int processUsed[PROCESS_MAX + 1];
pthread_mutex_t mutex;
pthread_mutex_t testMutex;

int allocateMap()
{
    memset(pidMap, 0, sizeof(pidMap));

    int size = processMax + 1;
    for (int i = 0; i < size; i++)
    {
        if (pidMap[i])
        {
            return -1;
        }
    }

    return 0;
}

int allocatePID()
{
    bool foundProcess = false;

    if (pthread_mutex_lock(&mutex))
    {
        cout << "Lock failed" << endl;
    }

    for (int i = 0; i < (processMax + 1) && !foundProcess; i++)
    {
        if (!pidMap[i])
        {
            lastUsedProcess = 1 + i + processMin;
            pidMap[i] = 1;
            foundProcess = true;
        }
    }

    if (pthread_mutex_unlock(&mutex))
    {
        cout << "Unlock Failed" << endl;
    }

    return foundProcess ? lastUsedProcess : -1;
}

void releasePID(int id)
{
    if (pthread_mutex_lock(&mutex))
    {
        cout << "Lock failed" << endl;
    }

    pidMap[id - 1 - processMin] = 0;

    if (pthread_mutex_unlock(&mutex))
    {
        cout << "Unlock Failed" << endl;
    }
}

void *pidAllocate(void *param)
{
    int i, id;

    for (i = 0; i < 10; i++)
    {
        sleep((int)(random() % 5)); //sleep for a random period of time
        id = allocatePID();         // allocate a pid

        if (id == -1)
        {
            cout << "No ids available" << endl;
        }
        else
        {
            if (pthread_mutex_lock(&testMutex))
            {
                cout << "Failed to lock" << endl;
            }

            if (processUsed[id])
            {
                processUsed[id] = 1;
            }
            cout << "Allocated: " << id << endl;

            if (pthread_mutex_unlock(&testMutex))
            {
                cout << "Couldn't unlock" << endl;
            }

            sleep((int)(random() % 5));

            releasePID(id);
            cout << "process id released" << endl;
            processUsed[id] = 0;
        }
    }

    return NULL;
}

void testPID()
{
    int i;
    pthread_t ids[THREAD_COUNT];
    memset(processUsed, 0, processMax + 4);

    if (allocateMap() == -1)
    {
        printf("map failed\n");
        return;
    }

    srandom((unsigned)time(NULL));
    for (i = 0; i < THREAD_COUNT; i++)
    {
        if (pthread_create(&ids[i], NULL, pidAllocate, (void *)&i) != 0)
            return;
    }

    for (i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(ids[i], NULL);
    }
}

int main()
{
    testPID();
}