#include<iostream>
#include <cmath>
#include "lcgrand.h"      
using namespace std; 
 
/////////////////////////////////////////////////////////////////////////
const int Queu_Limit=100;
const int BUSY=1;
const int IDLE=0;
 
int  
      choice,
      Num_Completed_Customers,    //Number of Completed Customers
      Number_of_Events,           //Number of Events 1.Arriving 2.Completion
      Number_in_Queue,            //Number of Customers In Queue
      Server_Status;              //Server Status ( Idle , Busy )
 
double
      End_Time,
      Type_Next_Event,
      Mean_interArrival_Time, 
      Mean_service_Time,
      Clock, 
      Time_Arrival[Queu_Limit + 1], 
      Service_Time[Queu_Limit + 1],
      Next_Arrival_Time, 
      Next_Completion_Time,
      Next_Service_Time,
      Total_Flow_Time,
      Progres_Arrival_Time,
      Progres_Completion_Time,
      Waiting_Time;
 
 
////////////////////////////////////////////////////////////////////////////
// Prototype of the System Function
 
void  initialize();
void  Timing();
void  Arrival();
void  Completition();
float expon(float mean);
void  Search_Min(double[],double[]);
 
////////////////////////////////////////////////////////////////////////////////////
//Main Function
 
int main()  
{
     
    initialize();           // Intialization of the System
 
    cout<<"                 * Single-server queueing system with fixed run *\n";
 
    cout<<"                 _________________________________________________"<<endl;
    
    cout<<"\n 1.First In First Out"<<endl;
 
    cout<<" 2.Minimum Processing Time"<<endl<<endl;
 
    do
    {
    cout<<"\tEnter your Policy: ";
 
    cin>>choice;
     
    }while(choice>2||choice<1);
     
    cout<<"\nMean Inter arrival Time: "<<Mean_interArrival_Time;
 
    cout<<"\nMean Service Time: "<<Mean_service_Time<<endl;
 
    cout<<"The End of Simulation Time: "<<End_Time<<endl<<endl;
  
 
    while(true) 
    {
         
        Timing();  // Timing Routine To Determine The Next Event
 
        if(Clock>End_Time)
           break;
     
        switch (int(Type_Next_Event)) 
        {
         
        case 1:
            Arrival();
            break;
             
        case 2:
            Completition();
            break;
        }
    } 
      
    // Print Summary Statistics.
     
    cout<<"\nTotal Flow Time: "<<Total_Flow_Time;
 
    cout<<"\nTotal Waiting Time in Queue: "<<Waiting_Time;
 
    cout<<"\nAverage Waiting Time in Queue: "<<Waiting_Time / Num_Completed_Customers;
     
    cout<<"\nAverage Flow Time: "<<Total_Flow_Time / Num_Completed_Customers;
     
    cout<<"\nNumber of Completed Customers: "<<Num_Completed_Customers;
 
    cout<<"\nAverage Number of Customers In System / Unit Time: "<<Num_Completed_Customers / Clock<<endl<<endl;
      
    return 0;
}
 
//////////////////////////////////////////////////////////////////////////////
//Intialization Function
 
void initialize()  
{
 
    Number_of_Events = 2;      // Arrival , Completion
     
    Mean_interArrival_Time=1.0;
     
    Mean_service_Time=0.5;
 
    End_Time=100.0;
     
    Clock = 0.0;
     
    Server_Status = IDLE;
     
    Number_in_Queue = 0;
 
    Num_Completed_Customers = 0;
 
    Total_Flow_Time = 0.0;
 
    Waiting_Time = 0.0;
 
    Next_Arrival_Time = Clock + expon(Mean_interArrival_Time);//Arriving
     
    Next_Service_Time = expon(Mean_service_Time);
     
    Next_Completion_Time = 1.0e+10;    // Completing  Guarantening that the first event is arriving
 
    Progres_Arrival_Time=0.0;
 
    Progres_Completion_Time = 0.0;
}
//////////////////////////////////////////////////////////////////////////////
// Timing Routine Function
 
void Timing()  
{
    Type_Next_Event = 0;
 
    if(Next_Arrival_Time < Next_Completion_Time)
    {
         
        Type_Next_Event = 1;
         
        Clock=Next_Arrival_Time;
 
    }
     
    else
    {
        Type_Next_Event = 2;
 
        Clock = Next_Completion_Time;
 
    }
     
    if (Type_Next_Event == 0) 
    {
        cout<<"\nEvent List Empty at Time: "<<Clock;
        
        exit(1);
    }       
 
}
 
////////////////////////////////////////////////////////////////////////////
// Arriving Customer function
 
void Arrival()  
{
    if (Server_Status == BUSY) 
    {
        ++Number_in_Queue;
 
        if (Number_in_Queue > Queu_Limit) 
        {
            cout<<"\nOverflow of the array time_arrival at";
             
            cout<<"time: "<<Clock;
             
            exit(2);
        }
 
        Time_Arrival[Number_in_Queue] = Clock;
 
        Service_Time[Number_in_Queue] = Next_Service_Time;
 
    }
 
    else
    {
        Server_Status = BUSY;
         
        Next_Completion_Time = Clock + Next_Service_Time;
 
        Progres_Arrival_Time = Next_Arrival_Time;
 
        Progres_Completion_Time = Next_Completion_Time;
         
    }
 
    Next_Arrival_Time = Clock + expon(Mean_interArrival_Time);
 
    Next_Service_Time = expon(Mean_service_Time);
     
     
}
/////////////////////////////////////////////////////////////////////////////
// Completion Customer Function
 
void Completition() 
{
 
    double Delay;
 
    ++Num_Completed_Customers;
     
    Total_Flow_Time+= ( Progres_Completion_Time - Progres_Arrival_Time );
 
 
    if (Number_in_Queue == 0) 
    {
        Server_Status= IDLE;
 
        Next_Completion_Time = 1.0e+10;      // High Value
    }
 
    else
    {
 
        if(choice==2)
            Search_Min(Time_Arrival,Service_Time);    // Minimum Processing Time
         
        Delay= Clock - Time_Arrival[1];
 
        Waiting_Time+= Delay;
     
        Next_Completion_Time = Clock + Service_Time[1]; 
 
        Progres_Arrival_Time = Time_Arrival[1];
 
        Progres_Completion_Time = Next_Completion_Time; 
                 
        --Number_in_Queue;
         
        for (int i=1;i<=Number_in_Queue;i++)
        {
            Time_Arrival[i] = Time_Arrival[i + 1];
             
            Service_Time[i] = Service_Time[i + 1];
        }
         
    }
}
 
/////////////////////////////////////////////////////////////////////////
//Sort Functtion
void Search_Min(double A_time[],double S_time[])
{
    int Min=1;
 
    double temp;
 
    for(int i=1;i<Number_in_Queue;i++)
        if(S_time[Min]>S_time[i+1])
            Min=i+1;
         
    temp=S_time[1];
    S_time[1]=S_time[Min];
    S_time[Min]=temp;
                 
    temp=A_time[1];
    A_time[1]=A_time[Min];
    A_time[Min]=temp;
 
}
 
///////////////////////////////////////////////////////////////////////////
// Generate The Rondom Number
 
float expon(float mean)  
{
    return (-mean * log(lcgrand(1)));
}
