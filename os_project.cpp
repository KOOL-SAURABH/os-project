
#include <bits/stdc++.h>
using namespace std;

struct Process_Data
{
	int Num;
	int Pid;  
	int A_time; 
	int B_time; 
	int Priority; 
	int F_time; 
	int R_time; 
	int W_time; 
	int S_time; 
	int Res_time;

};

struct Process_Data current;
typedef struct Process_Data P_d ;

bool idsort(const P_d& x , const P_d& y)
{
	return x.Pid < y.Pid;
}

bool arrivalsort( const P_d& x ,const P_d& y)
{
	if(x.A_time < y.A_time)
		return true;
	else if(x.A_time > y.A_time)
		return false;
	if(x.Priority < y.Priority)
		return true;
	else if(x.Priority > y.Priority)
		return false;
	if(x.Pid < y.Pid)
		return true;

	return false;
}


bool Numsort( const P_d& x ,const P_d& y)
{
	return x.Num < y.Num;
}

struct comPare
{
	bool operator()(const P_d& x ,const P_d& y)
	{
		if( x.Priority > y.Priority )
			return true;
		else if( x.Priority < y.Priority )
			return false;
		if( x.Pid > y.Pid )
			return true;

		return false;
		
	}
	
};
void my_check(vector<P_d> mv)
{
	for(unsigned int i= 0; i < mv.size() ;i++)
	{
		cout<<" Pid :"<<mv[i].Pid<<" _time : "<<mv[i].A_time<<" B_time : "<<mv[i].B_time<<" Priority : "<<mv[i].Priority<<endl;
	}

}

int main()
{
	int i;
	vector< P_d > input;
	vector<P_d> input_copy;
	P_d temp;
	int pq_process = 0; 
	int rq_process = 0; 
	int A_time;
	int B_time;
	int Pid;
	int Priority;
	int n;
	int clock;
	int total_exection_time = 0;
	cout<<"Enter the total number of processe:-";
	cin>>n;
	
	for( i= 0; i< n; i++ )
	{
		cout<<endl<<"------------------------------------NEXT PROCESS-------------------------"<<endl;
		cout<<"enter process id:-";
		cin>>Pid;
		cout<<"\nenter the arrival time:-";
		cin>>A_time;
		cout<<"\nenter the burst time:-";
		cin>>B_time;
		cout<<"\npriority of process:-";
		cin>>Priority;
		temp.Num = i+1;
		temp.A_time = A_time;
		temp.B_time = B_time;
		temp.R_time = B_time;
		temp.Pid = Pid;
		temp.Priority = Priority;
		input.push_back(temp);
	}
	input_copy = input;
	sort( input.begin(), input.end(), arrivalsort );
    
    total_exection_time = total_exection_time + input[0].A_time;
    for( i= 0 ;i< n; i++ )
    {
    	if( total_exection_time >= input[i].A_time )
    	{
    		total_exection_time = total_exection_time +input[i].B_time;
    	}
    	else
    	{
    		int diff = (input[i].A_time - total_exection_time);
    		total_exection_time = total_exection_time + diff + B_time;

    	}
    }

	int Ghant[total_exection_time]={0}; 
	for( i= 0; i< total_exection_time; i++ )
	{
		Ghant[i]=-1;
	}
	

	priority_queue < P_d ,vector<Process_Data> ,comPare> pq; 

	queue< P_d > rq; 
	int cpu_state = 0; 
	int quantum = 2 ; 
	current.Pid = -2;
	current.Priority = 999999;

	for ( clock = 0; clock< total_exection_time; clock++ )
	{
		
		for( int j = 0; j< n ; j++ )
		{
			if(clock == input[j].A_time)
			{
				pq.push(input[j]);
			}
		}
		

		if(cpu_state == 0) 
		{
			if(!pq.empty())
			{
				current = pq.top();
				cpu_state = 1;
				pq_process = 1;
				pq.pop();
				quantum = 2; 
			}
			else if(!rq.empty())
			{
				current = rq.front();
				cpu_state = 1;
				rq_process = 1;
				rq.pop();
				quantum = 2;
			}
		}
		else if(cpu_state == 1) 
		{
			if(pq_process == 1 && (!pq.empty()))
			{
				if(pq.top().Priority < current.Priority ) 
				{
					rq.push(current); 
					current = pq.top();
					pq.pop();
					quantum = 2; 
				}
			}
			else if(rq_process == 1 && (!pq.empty())) 
			{
				rq.push(current);
				current = pq.top();
				pq.pop();
				rq_process = 0;
				pq_process = 1;
				quantum = 2 ;
			}
			

		}


		if(current.Pid != -2) 
		{
			current.R_time--;
			quantum--;
			Ghant[clock] = current.Pid;
			if(current.R_time == 0) 
			{
				cpu_state = 0 ;
				quantum = 2 ;
				current.Pid = -2;
				current.Priority = 999999;
				rq_process = 0;
				pq_process = 0;
			}
			else if(quantum == 0 ) 
			{
				rq.push(current);
				current.Pid = -2;
				current.Priority = 999999;
				rq_process = 0;
				pq_process = 0;
				cpu_state=0;

			}

		}
		
	}


	sort( input.begin(), input.end(), idsort );
	
	for(int i=0;i<n;i++)
	{
		for(int k=total_exection_time;k>=0;k--)
		{
			if(Ghant[k]==i+1)
			{
				input[i].F_time=k+1;
				break;

			}
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<total_exection_time;k++)
		{

			if(Ghant[k]==i+1)
			{
				input[i].S_time=k;
				break;
			}
		}
	}
	
	sort( input.begin(), input.end(), Numsort );

	for(int i=0;i<n;i++)
	{
		input[i].Res_time=input[i].S_time-input[i].A_time;
		input[i].W_time=(input[i].F_time-input[i].A_time)-input[i].B_time;

	}
	cout<<"\nprocess id |  response time |  finish time | waiting time  \n";
	for(int i=0;i<n;i++)
	{
		cout<<"	"<<input[i].Pid<<"      	"<<input[i].Res_time<<"	   	 "<<input[i].F_time<<"  		"<<input[i].W_time<<endl;
		
	}	
	return 0;
}
