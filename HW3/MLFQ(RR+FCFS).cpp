#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct process
{
    int id;
    int arrive_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
};

bool compare_id(struct process p1, struct process p2)
{
    return (p1.id < p2.id);
}

bool compare_arrive_time(struct process p1, struct process p2)
{
    return (p1.arrive_time < p2.arrive_time);
}

int main()
{
    //number of processes
    int num = 0;
    cin >> num;

    struct process init = {0, 0, 0, 0, 0};
    //vector to store input processes
    vector<struct process> input;

    for (int i = 0; i < num; i++){
        input.push_back(init);
        input[i].id = i;
        cin >> input[i].arrive_time;
        //store arrive time in turnaround time for later use
        input[i].turnaround_time=input[i].arrive_time;
    }
    for (int i = 0; i < num; i++){
        cin >> input[i].burst_time;
        //store burst time in waiting time for later use
        input[i].waiting_time=input[i].burst_time;
    }
    //number of quantum
    int q = 0;
    cin >> q;

    int time=0;
    int total_waiting=0;
    int total_turnaround=0;

    //vector to store processes in rr(q0)
    vector<struct process> rr;
    //vector to store processes in fcfs(q1)
    vector<struct process> fcfs;
    //vector to store processes completed
    vector<struct process> complete;

    //sort input according to arrival time
    sort(input.begin(),input.end(),compare_arrive_time);

    //while there are still process not complete
    while(complete.size()!=num){
        //if there are still process not yet arrived
        if(!input.empty()){
            //add process from input to rr while it arrives
            if(time>=input[0].arrive_time){
                rr.push_back(input[0]);
                input.erase(input.begin());
            }

            //if there is process exist in q0, run rr
            if(!rr.empty()){

                if(time>=rr[0].arrive_time){
                    //if burst time>q, process can only execute for time=q then need to pass to next proceess
                    //store the unfinished process to lower priority queue(fcfs)
                    if(rr[0].burst_time>q){
                      time+=q;
                      rr[0].arrive_time+=q;
                      rr[0].burst_time-=q;
                      fcfs.push_back(rr[0]);
                      //sort fcfs according to arrival time to make sure it is ordered by arrival_time
                      sort(fcfs.begin(),fcfs.end(),compare_arrive_time);
                      rr.erase(rr.begin());
                    }
                    //if the process has burst time<=q, then this will be its last execution
                    //erase it from rr and add it to complete
                    else{
                      time+=rr[0].burst_time;
                      //turnaround time=complete time-arrive time
                      rr[0].turnaround_time=time-rr[0].turnaround_time;
                      //waiting time=complete time-arrive time-burst time
                      rr[0].waiting_time=rr[0].turnaround_time-rr[0].waiting_time;
                      total_turnaround+=rr[0].turnaround_time;
                      total_waiting+=rr[0].waiting_time;
                      complete.push_back(rr[0]);
                      rr.erase(rr.begin());
                    }
                }
            }

            //if there are process not yet arrive but all process arrived has run rr 1 time and rr is currently empty
            else{
                //lower priority queue(fcfs) can only be executed when higher priority queue(rr) is empty
                //new process will arrive in rr
                while(time<input[0].arrive_time){
                    //since fcfs is sorted, continue execute the first process until it is finished or terminated
                    time++;
                    fcfs[0].arrive_time++;
                    fcfs[0].burst_time--;
                    //if the process is finished
                    if(fcfs[0].burst_time==0){
                        fcfs[0].turnaround_time=time-fcfs[0].turnaround_time;
                        //waiting time=complete time-arrive time-burst time
                        fcfs[0].waiting_time=fcfs[0].turnaround_time-fcfs[0].waiting_time;
                        total_turnaround+=fcfs[0].turnaround_time;
                        total_waiting+=fcfs[0].waiting_time;
                        complete.push_back(fcfs[0]);
                        fcfs.erase(fcfs.begin());
                    }
                }
                //if the process is not finished but preempted by newly arrived process
                fcfs.push_back(fcfs[0]);
                fcfs.erase(fcfs.begin());
            }

        }

        //if all processes have arrived and rr is empty, the rest of the processes are all fcfs
        else if(time>=fcfs[0].arrive_time&&rr.empty()){
            //the process is finished, add it to complete and erase it from fcfs
            time+=fcfs[0].burst_time;
            //turnaround time=complete time-arrive time
            fcfs[0].turnaround_time=time-fcfs[0].turnaround_time;
            //waiting time=complete time-arrive time-burst time
            fcfs[0].waiting_time=fcfs[0].turnaround_time-fcfs[0].waiting_time;
            total_turnaround+=fcfs[0].turnaround_time;
            total_waiting+=fcfs[0].waiting_time;
            complete.push_back(fcfs[0]);
            fcfs.erase(fcfs.begin());
        }
        //in case previous process has finished and new process not arrived yet
        else
            time++;
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
