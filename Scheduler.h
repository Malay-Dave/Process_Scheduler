#pragma once

#include"Process.h"

#include<vector>
#include<string>

void fifo(std::vector<Process>& process, std::vector<std::string>& ans);
void roundrobin(std::vector<Process>& process, std::vector<std::string>& ans);