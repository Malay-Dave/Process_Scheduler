#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
#include<queue>
#include<list>
#include<algorithm>

using namespace std;
struct Process{
    int id;
    int arrival_time;
    int dept_time;
    int cpu_idx;
    int io_idx;
    int io_finish=-1;
    vector<int> cpu_burst;
    vector<int> io_burst;

};



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



void fifo(vector<Process>& process, vector<string>& ans){
    queue<int> que;
    int current_time=0;
    
    int n=process.size();
    
    int completed=0;
    priority_queue<pair<int,int> , vector<pair<int,int>>, greater<pair<int,int>>> pq; 
    for(int i=0;i<n;i++){
        pq.push({process[i].arrival_time, i});
    }
    while(completed < n){
        
        while(!pq.empty() && pq.top().first <=current_time){
            que.push(pq.top().second);
            pq.pop();
        }
        if(que.empty()){
            if(!pq.empty()){
                current_time=pq.top().first;
            }
            else current_time++;
            continue;
        }
        int top=que.front();
        que.pop();
        ans.push_back( "P" + to_string(process[top].id) + "," + to_string(process[top].cpu_idx+1) + " "  + to_string(current_time) + "\t" + to_string( current_time + process[top].cpu_burst[process[top].cpu_idx]-1) );
        current_time+=process[top].cpu_burst[process[top].cpu_idx];
        process[top].cpu_idx++;
        if(process[top].cpu_idx==process[top].cpu_burst.size()){
            process[top].dept_time=current_time-1;
            completed++;
        }
        else{
            process[top].io_finish=current_time+process[top].io_burst[process[top].io_idx];
            process[top].io_idx++;
            pq.push({process[top].io_finish, top});
        }
    }

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
        
    }
    return 0;
}