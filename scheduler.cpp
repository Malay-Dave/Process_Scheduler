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
            process[top].io_finish=current_time+process[top].io_burst[process[top].io_idx]-1;
            process[top].io_idx++;
            pq.push({process[top].io_finish, top});
        }
    }

}

void boost_process(vector<queue<int>>& ready_queues, vector<Process>& process, int num_levels){
    for(int i=1;i<num_levels;i++){
        while(!ready_queues[i].empty()){
            int top=ready_queues[i].front();
            ready_queues[i].pop();
            process[top].priority=0;
            ready_queues[0].push(top);
        }
    }

}

void MLFQ(vector<Process>& process, vector<string>& ans){
    int current_time=0;
    int completed=0;
    int n=process.size();
    int num_levels=3;
    int quanta=2;
    bool enable_boost=true;
    int next_boost=20;
    vector<queue<int>> ready_queues(num_levels);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    for(int i=0;i<n;i++)pq.push({process[i].arrival_time,i});

    while(completed < n){
        int i=0;
        while(!pq.empty() && pq.top().first<=current_time){
            ready_queues[process[pq.top().second].priority].push(pq.top().second);
            pq.pop();
        }

        if(enable_boost && current_time>=next_boost){
            boost_process(ready_queues,process,num_levels);
            next_boost= ((current_time/20)+1)*20;
        }

        while(i<num_levels && ready_queues[i].empty()){
            i++;
        }
        if(i==num_levels){
            current_time=pq.top().first;

            if(enable_boost && next_boost <=current_time){
                next_boost=((current_time/20)+1)*20;
            }
            continue;
        }
        int finish_time;
        int top=ready_queues[i].front();
        ready_queues[i].pop();
        if(process[top].cpu_burst[process[top].cpu_idx] >= quanta){
            finish_time=current_time + quanta -1;
            process[top].cpu_burst[process[top].cpu_idx]-= quanta;
            process[top].priority= min(2, process[top].priority+1);
        }
        else{
            finish_time=current_time+process[top].cpu_burst[process[top].cpu_idx] -1;
            process[top].cpu_burst[process[top].cpu_idx]= 0;
        }
        ans.push_back("P" + to_string(process[top].id) + "," + to_string(process[top].cpu_idx+1) + " " + to_string(current_time) + " "+  to_string(finish_time));

        current_time=finish_time +1;

        while(!pq.empty() && pq.top().first<=current_time){
            ready_queues[process[pq.top().second].priority].push(pq.top().second);
            pq.pop();
        }

        if(process[top].cpu_burst[process[top].cpu_idx]==0){
            process[top].cpu_idx++;
            if(process[top].cpu_idx==process[top].cpu_burst.size()){
                process[top].dept_time=finish_time;
                completed++;
            }
            else{
                process[top].priority=0;
                pq.push({current_time + process[top].io_burst[process[top].io_idx], top});
                process[top].io_finish=current_time + process[top].io_burst[process[top].io_idx]-1;
                process[top].io_idx++;
            }
        }
        else{
            ready_queues[process[top].priority].push(top);
        }
    }
}