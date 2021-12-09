#include <stdio.h>
#include "../utils/process.h"
#include "../utils/helpers.h"

process *sortProcessesByCT(process *tab, int size)
{
    process aux;
    for (int i = 1; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (tab[i].ct >= tab[j].ct)
            {
                aux = tab[i];
                tab[i] = tab[j];
                tab[j] = aux;
            }
        }
    }

    return tab;
}

int sjf(process *tab, int processesNumber, int mode)
{
    data dataTab[processesNumber];
    int processedInTotal, outputIndex, dataIndex, tookTour, ganttTimeIndex = 0;
    int output[100];
    int ganttTime[100];

    tab = sortProcesses(tab, processesNumber); // just to get the first process no matter what, if it has a shortest job or not

    dataTab[0].id = output[0] = tab[0].id;
    dataTab[0].started = tab[0].at;
    dataTab[0].burst = tab[0].ct;
    dataTab[0].finished = tab[0].at + tab[0].ct;
    outputIndex = dataIndex = processedInTotal = 1;
    int currentTime = dataTab[0].finished;
    tookTour = 0;
    // the first and second value are always fixed
    ganttTime[ganttTimeIndex] = tab[0].at;
    ganttTimeIndex++;
    ganttTime[ganttTimeIndex] = dataTab[0].finished;
    ganttTimeIndex++;

    tab = sortProcessesByCT(tab, processesNumber); // now we should work

    while (processedInTotal < processesNumber)
    {
        tookTour = 0;
        for (int i = 1; i < processesNumber; i++)
        {
            if (tab[i].at <= currentTime && tab[i].ct > 0)
            {
                dataTab[dataIndex].id = tab[i].id;
                dataTab[dataIndex].started = tab[i].at;
                dataTab[dataIndex].burst = tab[i].ct;
                if (tab[i].at >= dataTab[dataIndex - 1].finished)
                {
                    dataTab[dataIndex].finished = tab[i].at + tab[i].ct;
                }
                else
                {
                    dataTab[dataIndex].finished = dataTab[dataIndex - 1].finished + tab[i].ct;
                }
                output[outputIndex] = tab[i].id;
                outputIndex++;
                ganttTime[ganttTimeIndex] = dataTab[dataIndex].finished;
                ganttTimeIndex++;
                tab[i].ct = 0;
                currentTime = dataTab[dataIndex].finished;
                tookTour++;
                dataIndex++;
                processedInTotal++;
                break;
            }
        }
        if (tookTour == 0)
        {
            currentTime++;
            output[outputIndex] = -1;
            outputIndex++;
            ganttTime[ganttTimeIndex] = ganttTime[ganttTimeIndex - 1] + 1;
            ganttTimeIndex++;
        }
    }

    if (mode == 1)
    {
        displayForApp(outputIndex, output, processesNumber, dataTab, ganttTimeIndex, ganttTime);
    }
    else
    {
        displayForCLI(outputIndex, output, processesNumber, dataTab, ganttTimeIndex, ganttTime);
    }
}

// int main(int argc, char *argv[])
// {
//     process *tab;
//     int *output;

//     tab = fillProcesses(argv[1]);

//     sjf(tab, countProcesses(argv[1]), 1);
//     return 0;
// }