#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct srtf{
    int id;
    int arrive_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
};

bool compare_arrive_time(struct srtf p1,struct srtf p2){
    return(p1.arrive_time<p2.arrive_time);
}
bool compare_id(struct srtf p1,struct srtf p2){
    return(p1.id<p2.id);
}
//if 2 process have the same remaining time, the process currently execute should have higher priority
bool compare_remaining_time(struct srtf p1,struct srtf p2){
    return(p1.remaining_time<p2.remaining_time);
}

int main()
{
    //number of processes
    int num;
    cin>>num;

    //vector to store input processes
    vector<struct srtf> input;
    struct srtf init={0,0,0,0,0,0};

    for(int i=0;i<num;i++){
        input.push_back(init);
        input[i].id=i;
        cin>>input[i].arrive_time;
    }

    for(int i=0;i<num;i++){
        cin>>input[i].burst_time;
        input[i].remaining_time=input[i].burst_time;
    }
    //sort input according to arrival time
    sort(input.begin(),input.end(),compare_arrive_time);

    int total_waiting=0;
    int total_turnaround=0;
    int time=0;

    //vector to store the processes which have arrived and ready to be execute
    vector<struct srtf> srtf;
    //vector to store the processes completed
    vector<struct srtf> complete;

    //while there are still processes not complete
    while(complete.size()!=num){
        //add a process from input to srtf while the process arrive
        if(!input.empty()){
            if(time>=input[0].arrive_time){
                srtf.push_back(input[0]);
                input.erase(input.begin());
            }
        }
        //srtf is preemptive, so it needs to be sort by remaining time every time
        //to make sure that the process currently executing has the shortest remaining time
        sort(srtf.begin(),srtf.end(),compare_remaining_time);
        srtf[0].remaining_time--;
        time++;
        //if the process has finished execution, erase it from srtf and add it to complete
        if(srtf[0].remaining_time==0){
            //turnaround time=complete time-arrive time
            srtf[0].turnaround_time=time-srtf[0].arrive_time;
            //waiting time=complete time-arrive time-burst time
            srtf[0].waiting_time=srtf[0].turnaround_time-srtf[0].burst_time;
            total_waiting+=srtf[0].waiting_time;
            total_turnaround+=srtf[0].turnaround_time;
            complete.push_back(srtf[0]);
            srtf.erase(srtf.begin());
        }

    }
    //sort complete by process id
    sort(complete.begin(),complete.end(),compare_id);
    for(int i=0;i<num;i++){
        cout<<complete[i].waiting_time<<" "<<complete[i].turnaround_time<<endl;
    }
    cout<<total_waiting<<endl;
    cout<<total_turnaround<<endl;

    return 0;
}
