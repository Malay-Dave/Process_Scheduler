#include"Process.h"
#include"Scheduler.h"

#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>


using namespace std;



vector<Process> parse_file(char* s){
    vector<Process> temp;
    ifstream file(s);

    if(!file.is_open()){
        cout << "Failed To Open File\n" ;
        return temp;
    }
    string line;

    int i=1;
    while(getline(file,line)){
        if(line.empty())continue;

        stringstream ss(line);
        Process p;
        ss  >> p.arrival_time ;
        bool is_cpu=true;
        int burst;
        p.dept_time=0;
        p.id=i++;
        p.cpu_idx=0;
        p.io_idx=0;

        while(ss >> burst){
            if(burst==-1)break;
            if(is_cpu){
                p.cpu_burst.push_back(burst);
            }
            else {
                p.io_burst.push_back(burst);
            }
            is_cpu=!is_cpu;
        } 
        temp.push_back(p);
    }
    return temp;
}




int main(int argc , char* argv[]){
    if(argc!=3){
        cout<< "Include Proper Arguments. \n" ;
        return 1;
    }

    char* path=argv[2];

    vector<Process> process = parse_file(path);
    vector<string> ans;
    if(string(argv[1])=="FIFO"){
        fifo(process, ans);
        for(string i: ans)cout << i << endl;
    }
    if(string(argv[1])=="RR"){
        roundrobin(process, ans);
        for(string i: ans)cout << i << endl;
    }
    return 0;
}