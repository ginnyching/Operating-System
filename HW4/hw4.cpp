#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void FIFO(int frames, vector<int> page){

    int page_fault=0;
    vector<int> frame(frames,-1);

    for(int i=0;i<page.size();i++){
        //if not found in frame
        if(find(frame.begin(),frame.end(),page[i])==frame.end()){
            frame.erase(frame.begin());
            frame.push_back(page[i]);
            page_fault++;
        }

    }
    cout<<page_fault<<endl;

}

void LRU(int frames, vector<int> page){

    int page_fault=0;
    //old->new, i.e. the last element in recently_used is the last page accessed
    vector<int> recently_used;
    vector<int> frame;

    for(int i=0;i<page.size();i++){

        //if not found in frame
        if(find(frame.begin(),frame.end(),page[i])==frame.end()){
            //frame not full yet
            if(recently_used.size()<frames){
                //first time accessing this page
                if(find(recently_used.begin(),recently_used.end(),page[i])==recently_used.end()){
                    recently_used.push_back(page[i]);
                    frame.push_back(page[i]);
                    page_fault++;
                }
                //page has been accessed before
                else{
                    //erase last occurrence in recently_used and update new occurrence
                    recently_used.erase(find(recently_used.begin(),recently_used.end(),page[i]));
                    recently_used.push_back(page[i]);
                }
            }
            //frame is full
            else {
                int frame_index=-1;
                //first time accessing this page
                if(find(recently_used.begin(),recently_used.end(),page[i])==recently_used.end()){


                    vector<int>::iterator least_recently_used=recently_used.end();

                    for(int j=0;j<frames;j++){
                        vector<int>::iterator position=find(recently_used.begin(),recently_used.end(),frame[j]);
                        if(position<least_recently_used){
                            least_recently_used=position;
                            frame_index=j;
                        }
                    }
                    //erase from frame and push back new page
                    frame.erase(frame.begin()+frame_index);
                    frame.push_back(page[i]);
                    recently_used.push_back(page[i]);
                    page_fault++;
                }
                //accessed before but has been swapped out
                else{
                    //erase last occurrence in recently_used and update new occurrence
                    recently_used.erase(find(recently_used.begin(),recently_used.end(),page[i]));
                    recently_used.push_back(page[i]);

                    vector<int>::iterator least_recently_used=recently_used.end();

                    for(int j=0;j<frames;j++){
                        vector<int>::iterator position=find(recently_used.begin(),recently_used.end(),frame[j]);
                        if(position<least_recently_used){
                            least_recently_used=position;
                            frame_index=j;
                        }
                    }
                    //erase from frame and push back new page
                    frame.erase(frame.begin()+frame_index);
                    frame.push_back(page[i]);
                    page_fault++;
                }
            }

        }

        else{
            recently_used.erase(find(recently_used.begin(),recently_used.end(),page[i]));
            recently_used.push_back(page[i]);
        }
    }
    cout<<page_fault<<endl;
}


void LFU(int frames, vector<int> page){

    int page_fault=0;
    //frequency={page_index, page_frequency}
    vector<pair<int,int>> frequency;

    vector<int> frame;

    for(int i=0;i<page.size();i++){
        //not in frame
        if(find(frame.begin(),frame.end(),page[i])==frame.end()){

            //frame not full yet
            if(frame.size()<frames){
                frame.push_back(page[i]);
                frequency.push_back({page[i],1});
                page_fault++;
            }
            //frame is full
            else{
                int least_frequency=101;
                int frame_index=-1;
                /*find page with least frequency
                since frame is updated by push_backand is not sorted in other ways
                the first element in frame is the first arriving page in frame
                so it will follow FIFO if they haave the same frequency*/
                for(int i=0;i<frames;i++){
                    if(frequency[i].second<least_frequency){
                        least_frequency=frequency[i].second;
                        frame_index=i;
                    }
                }
                //erase from frame and push back new page
                frame.erase(frame.begin()+frame_index);
                frame.push_back(page[i]);
                //erase from frequency and push back new page and frequency
                frequency.erase(frequency.begin()+frame_index);
                frequency.push_back({page[i],1});

                page_fault++;
            }

        }
        //page in frame
        else{
            for(int j=0;j<frequency.size();j++){
                //add frequency
                if(frequency[j].first==page[i])
                    frequency[j].second++;
            }
        }

    }
    cout<<page_fault<<endl;
}


int main()
{
    int frame=0,length=0;
    cin>>frame>>length;

    vector<int> page(length,-1);
    for (int i=0;i<length;i++){
        cin>>page[i];
    }

    FIFO(frame,page);
    LRU(frame,page);
    LFU(frame,page);

    return 0;
}
