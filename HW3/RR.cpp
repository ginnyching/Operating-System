#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct rr
{
    int id;
    int arrive_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
};

bool compare_id(struct rr p1, struct rr p2)
{
    return (p1.id < p2.id);
}

bool compare_arrive_time(struct rr p1, struct rr p2)
{
    return (p1.arrive_time < p2.arrive_time);
}
int main()
{
    //number of processes
    int num = 0;
    cin >> num;

    struct rr init = {0, 0, 0, 0, 0};
    //vector to store input processes
    vector<struct rr> input;

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
    //vector to store the processes completed
    vector<struct rr> complete;

    //while there are still process not complete
    while(complete.size()!=num){

        //sort input according to arrival time
        sort(input.begin(),input.end(),compare_arrive_time);

        //in case previous process has finished and new process not arrived yet
        if(time>=input[0].arrive_time){
            //if burst time>q, process can only execute for time=q then need to pass to next proceess
            if(input[0].burst_time>q){
                time+=q;
                input[0].arrive_time+=q;
                input[0].burst_time-=q;
                input.push_back(input[0]);
                input.erase(input.begin());
            }
            //if the process has burst time<=q, then this will be its last execution
            //erase it from input and add it to complete
            else{
                time+=input[0].burst_time;
                //turnaround time=complete time-arrive time
                input[0].turnaround_time=time-input[0].turnaround_time;
                //waiting time=complete time-arrive time-burst time
                input[0].waiting_time=input[0].turnaround_time-input[0].waiting_time;
                total_turnaround+=input[0].turnaround_time;
                total_waiting+=input[0].waiting_time;
                complete.push_back(input[0]);
                input.erase(input.begin());
            }
        }
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
