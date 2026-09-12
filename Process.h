#pragma once

#include<vector>

struct Process{
    int id;
    int arrival_time;
    int dept_time;
    int cpu_idx;
    int io_idx;
    int io_finish=-1;
    int priority=0;
    std::vector<int> cpu_burst;
    std::vector<int> io_burst;
};
