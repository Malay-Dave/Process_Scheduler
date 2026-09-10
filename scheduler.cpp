#include"Scheduler.h"

#include<queue>


using namespace std;


void roundrobin(vector<Process>& process, vector<string>& ans){
    int n=process.size();
    int completed=0;
    int current_time=0;
    queue<int> q;
    int quanta=3;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    for(int i=0;i<n;i++)pq.push({process[i].arrival_time,i});
    while(completed < n){
        while(!pq.empty() && pq.top().first <= current_time){
            q.push(pq.top().second);
            pq.pop();
        }
        if(q.empty()){
            if(!pq.empty())current_time=pq.top().first;
            else current_time++;
            continue;
        }
        int top=q.front();
        q.pop();
        int finish_time;
        
        if(process[top].cpu_burst[process[top].cpu_idx]>=quanta)
        {
            finish_time=current_time+quanta-1;
            process[top].cpu_burst[process[top].cpu_idx]-=quanta;
        } 
        else
        { 
            finish_time=current_time + process[top].cpu_burst[process[top].cpu_idx]-1;
            process[top].cpu_burst[process[top].cpu_idx]=0;
        }
        ans.push_back("P"+ to_string(process[top].id)+","+ to_string(process[top].cpu_idx+1)+" "+to_string(current_time)+" " + to_string(finish_time));
        current_time = finish_time+1;


        //for giving priority to i/o operations
        while(!pq.empty() && pq.top().first<=current_time){
            q.push(pq.top().second);
            pq.pop();
        }

        if(process[top].cpu_burst[process[top].cpu_idx]==0){
            process[top].cpu_idx++;
            if(process[top].cpu_idx==process[top].cpu_burst.size()){
                process[top].dept_time=finish_time;
                completed++;
            }
            else{
                process[top].io_finish=current_time+process[top].io_burst[process[top].io_idx]-1;
                pq.push({current_time+process[top].io_burst[process[top].io_idx], top});
                process[top].io_idx++;
            }
        }
        else{
            q.push(top);
        }

    }
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
        ans.push_back( "P" + to_string(process[top].id) + "," + to_string(process[top].cpu_idx+1) + " "  + to_string(current_time) + " " + to_string( current_time + process[top].cpu_burst[process[top].cpu_idx]-1) );
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