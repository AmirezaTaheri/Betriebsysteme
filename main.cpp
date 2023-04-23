#include <iostream>
#include <sys/resource.h>
#include <fstream>
#include <ctime>
using namespace std;

void funcMem()
{
    const int ARRAY_SIZE = 100000;
    int *arr = new int[ARRAY_SIZE];
    int primeCounter = 0, arrayIndex = 0;
    long maxrss_start, maxrss_new, isrss_start, isrss_new, idrss_start, idrss_new;
    struct rusage startUsage;

    if(getrusage(RUSAGE_SELF,&startUsage)!=0)
            cerr << "START-RUSAGE fehlgeschlagen!" << endl;

    maxrss_start = startUsage.ru_maxrss;
    isrss_start = startUsage.ru_isrss;
    idrss_start = startUsage.ru_idrss;

    // der Array wird mit Primzahlen belegt. Damit CPU beschäftigt wird und Zeit vergeht
    for(int i=3; i<10000000; i++)
    {
        primeCounter = 0;
        for(int j=2; j<i; j++)
        {
            if(i%j==0)
                primeCounter++;
        }

        if(primeCounter == 0)
        {
            arr[arrayIndex] = i;
            if(arrayIndex%100 == 0)
            {
                struct rusage newUsage;
                if(getrusage(RUSAGE_SELF, &newUsage) != 0)
                    cerr << "newUsage-USAGE fehlgeschlagen" << endl;
                cout << "\t********\t\n";
                cout << "Verbrauchte User-Cpu-Zeit: " << (newUsage.ru_utime.tv_sec - startUsage.ru_utime.tv_sec) + ((newUsage.ru_utime.tv_usec - startUsage.ru_utime.tv_usec) / 1000000) << endl;
                cout << "Verbrauchte System-Cpu-Zeit: " << (newUsage.ru_stime.tv_sec - startUsage.ru_stime.tv_sec) + ((newUsage.ru_stime.tv_usec - startUsage.ru_stime.tv_usec) / 1000000) << endl;
                //ru_isrss -> Amount of stack memory used (kilobyte-seconds).
                //ru_idrss -> Amount of data segment memory used (kilobyte-seconds).
                //ru_maxrss -> Maximum resident set size (in kilobytes).
                maxrss_new = newUsage.ru_maxrss;
                isrss_new = newUsage.ru_isrss;
                idrss_new = newUsage.ru_idrss;
                cout << "Amount of stack memory used (kilobyte-seconds): " << isrss_new - isrss_start << endl;
                cout << "Amount of data segment memory used (kilobyte-seconds): " << idrss_new - idrss_start << endl;
                cout << "Maximum resident set size (in kilobytes): " << maxrss_new - maxrss_start << endl;
            }
            arrayIndex++;
            if(arrayIndex >= ARRAY_SIZE)
                break;
        }

    }

}


void funcRec()
{
    static rusage startUsage;
    if(getrusage(RUSAGE_SELF, &startUsage) != 0)
        cerr << "Rekursiv-start-Usage fehlgeschlagen" << endl;
    static int rekursivCounter = 0;
    rekursivCounter++;

    if(rekursivCounter%100 == 0)
    {
        struct rusage newUsage;
        if(getrusage(RUSAGE_SELF, &newUsage)!=0)
            cerr << "newUsage-Usage fehlgeschlagen" << endl;
        cout << "\t********\t\n";
        cout << "Verbrauchte User-Cpu-Zeit: " << (newUsage.ru_utime.tv_sec - startUsage.ru_utime.tv_sec) + ((newUsage.ru_utime.tv_usec - startUsage.ru_utime.tv_usec) / 1000000) << endl;
        cout << "Verbrauchte System-Cpu-Zeit: " << (newUsage.ru_stime.tv_sec - startUsage.ru_stime.tv_sec) + ((newUsage.ru_stime.tv_usec - startUsage.ru_stime.tv_usec) / 1000000) << endl;
    }

    funcRec();
}

int main()
{
    //funcMem();
    funcRec();

}

