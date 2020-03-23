
// own_MM1v1.2.cpp
//
// Created by Tobias Lettenmeier on 13.01.16.
// Copyright © 2016 Tobias Lettenmeier. All rights reserved.
// Contact: pudding[AT]mailbox[DOT]org


#include <math.h>

double MeanInterArrivalTime = 4.5;
double MeanServiceTime = 3.2;

double uniform(double border){
  double r2 = (double)border*(rand())/2147483647;
  return r2;
}

double exponential(double mean){
  double random =0;
  double r1 = (double) (rand())/2147483647;
  random = -mean*log(r1);
  return random;
}

class ServerListElement {
public:
  ServerListElement(){
  };
  ServerListElement(int ID, double Arrivaltime, double ServiceTime){
    _id = ID;
    _arrivaltime = Arrivaltime;
    _servicetime = ServiceTime;
  }

  void setTimeInQueue(double t){
    _timeinqueue = t;
  }

  double getTimeInQueue(){
    return _timeinqueue;
  }

  void setFrom(double from){
    _from = from;
  }

  void setTo(double to){
    _to = to;
  }

  double getServiceTime(){
    return _servicetime;
  }
  double getArrivalTime(){
    return _arrivaltime;
  }

  double getTo(){
    return _to;
  }

  double getFrom(){
    return _from;
  }

  int getID(){
    return _id;
  }


  void printElement(){
    std::cout << "Element ID: "<< _id << std::endl;
    std::cout << "Arrival time: "<< _arrivaltime << std::endl;
    std::cout << "Service time: " << _servicetime<< std::endl;
    std::cout << "served \t from: " << _from<< std::endl;
    std::cout << "\t to: " << _to<< std::endl;
    std::cout << "Waiting time: "<< _timeinqueue << std::endl;
    std::cout << " " << std::endl;

  }
protected:
  int _id;
  double _arrivaltime; //Arrival time
  double _servicetime;
  double _from; // time when service starts
  double _to; // time when service ends
  double _timeinqueue;
};

class ServerList {
public:
  ServerList(){
  }

  ServerList(int NumberOfTotalCustomers){
    //create first element in List. Means everything is zero
    ServerListElement first(0, 0, 0);
    _list = new ServerListElement[NumberOfTotalCustomers]; // VORSICHT! dynamisch erzeugtes array
    _list[0] = first;
    _list[0].setTo(0);
    _list[0].setFrom(0);
    _meanservicetime = 3.2;
    _meaninterarrivaltime = 4.5;
    _totalwaitingtime = 0;
    _elementcounter = 0;
    _serverclock = 0;
  }
  ~ServerList(){
    _list = NULL;
    _meaninterarrivaltime =0;
    _meanservicetime =0;
    _elementcounter =0;
    _timeserverinservice =0;
    _utilization =0;
    _totalwaitingtime =0;
    _averagetimeinqueue =0;
    _averagenumberinsystem =0;
    _averagequeuelength =0;
    _serverclock =0;
    _actualqueuelength =0;
  }


  ServerListElement getElementNumber(int k){
    return _list[k];
  }

  // function add() does the most important work in the whole program.
  // NOTICE: works only in FIFO-mode
  void add(){

    ServerListElement e(_elementcounter, _list[_elementcounter-1].getArrivalTime() + exponential(_meaninterarrivaltime), exponential(_meanservicetime));

    if (_list[_elementcounter-1].getTo() <= e.getArrivalTime()) {

      // k = 2: ||-------------------|A2______+______s2|-
      // k = 1: ||---|A1__+__s1|-------------------------
      //        ||=======================================> time

      _list[_elementcounter] = e;
      e.setFrom(e.getArrivalTime());
      e.setTo(e.getArrivalTime() + e.getServiceTime());

    }else{

      // k = 2  ||----------|A2______+______s2|----------
      // k = 1  ||---|A1__+__s1|-------------------------
      //        ||=======================================> time

      // set _from of the element to _to of the last element:
      e.setFrom(_list[_elementcounter-1].getTo());
      // set _to of the element to _from + ServiceTime
      e.setTo(_list[_elementcounter-1].getTo() + e.getServiceTime());
    }

    // calculate and set the time in queue of e
    e.setTimeInQueue(e.getFrom() - e.getArrivalTime());

    // adding time values in order to get right statistics
    _timeserverinservice += e.getServiceTime();
    _totalwaitingtime += e.getTimeInQueue();

    // ------- following is needed for next event
    // put created element into list
    _list[_elementcounter] = e;

    // OPTIONAL print element information
    //e.printElement();

    // setting all parameters to the right values
    _elementcounter++;
    _serverclock = e.getTo();
  }


  void add(ServerListElement e){
  }

  void setServerTime(double t){
    _serverclock = t;
  }

  double getServerTime(){
    return _serverclock;
  }

  double getUtilization(){
    return _utilization;
  }

  void printActualQueue(){
    //std::cout << "QUEUE: " << _actualqueuelength << std::endl;
    for (int i = 0; i < _actualqueuelength; i++) {
      std::cout << " * ";
    }
    std::cout << " " << std::endl;
  }

  void printServerStats(){
    // calculate utilization:
    _utilization = _timeserverinservice/_serverclock;

    // calculate waiting time of e to totel waiting time:
    _averagetimeinqueue = _totalwaitingtime/_elementcounter;

    // average queue length:
    _averagequeuelength = _totalwaitingtime/_serverclock;
    //todo: calculating the QueueLength

    std::cout << "****************************************************************************" << std::endl;
    std::cout << "SERVERSTATS REPORTING:" << std::endl;
    std::cout << "\tSERVER CLOCK: \t \t \t"<< _serverclock << "\t[time units]"<< std::endl;
    std::cout << "\tSERVERUTILIZATION: \t\t" << _utilization*100 <<"\t%" <<std::endl;
    std::cout << "\tCUSTOMERS SERVED: \t\t"<< _elementcounter << std::endl;
    std::cout << "\tAVERAGE TIME IN QUEUE: \t\t"<< _averagetimeinqueue << "\t[time units]"<< std::endl;
    std::cout << "\tAVERAGE QUEUE LENGTH: \t\t"<< _averagequeuelength << std::endl;
    std::cout << "*****************************************************************************" << std::endl;
  }

protected:
  ServerListElement *_list;
  double _meaninterarrivaltime;
  double _meanservicetime;
  int _elementcounter;
  double _timeserverinservice;
  double _utilization;
  double _totalwaitingtime;
  double _averagetimeinqueue;
  double _averagenumberinsystem;
  double _averagequeuelength;
  double _serverclock;
  double _actualqueuelength;
};



int main(int argc, char* argv[]){
  std::cout << "*************************** START OF SIMULATION ****************************" << std::endl;
  srand(std::time(0)); //seeding RNG
  int TOTALCUSTOMERS = 10000000;
  ServerList L(TOTALCUSTOMERS);
  std::cout << "RUNNING SIMULATION..." << std::endl;
  for (int i = 0; i < TOTALCUSTOMERS; i++) {
    L.add();
  }
  L.printServerStats();
  L.~ServerList();
  std::cout << "...SIMULATION SUCCESSFULLY COMPLETED!" << std::endl;
}
