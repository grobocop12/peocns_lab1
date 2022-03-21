#include <omnetpp.h>
#include <cmath>
#include "histogram.h"

using namespace omnetpp;

class Server: public cSimpleModule {
private:
    cQueue queue;	 //the queue; first job in the queue is being serviced
    cMessage *departure; //message-reminder of the end of service (job departure)
    simtime_t departure_time;  //time of the next departure
    simtime_t lastArrival;
    QueueHist queueHist;
    cHistogram histogram;
    IntervalCalculator jobIntervalCalc;
    IntervalCalculator messageIntervalCalc;
    IntervalCalculator messageInSystem;



protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);
    virtual void finish();
    void summarize(ostream &str);
};

Define_Module(Server);

void Server::initialize() {
    lastArrival = 0;
    departure = new cMessage("Departure");
    jobIntervalCalc = IntervalCalculator();
    messageIntervalCalc = IntervalCalculator();
    messageInSystem = IntervalCalculator();
}

void Server::handleMessage(cMessage *msgin) {
    if (msgin==departure) //job departure
        {
            cMessage *msg = (cMessage *)queue.pop(); //remove job from the head of the queue
            messageInSystem.put(simTime() - msg->getTimestamp());
            send(msg,"out");
            queueHist.get();
            if (!queue.isEmpty()) //schedule next departure event
            {
                departure_time=simTime()+par("service_time");
                jobIntervalCalc.put(departure_time - simTime());
                scheduleAt(departure_time,departure);
            }
        }
        else //job arrival
        {
            msgin->setTimestamp(simTime());
            messageIntervalCalc.put(simTime() - lastArrival);
            lastArrival = simTime();
            if (queue.isEmpty())
            {
                departure_time=simTime()+par("service_time");
                jobIntervalCalc.put(departure_time - simTime());
                scheduleAt(departure_time,departure);
            }
            if (queue.getLength() < par("buffer_size").intValue())
            {
                queue.insert(msgin); //put job at the end of the queue
                queueHist.put();
            } else {
                delete msgin;
            }

        }
}
//jak queue jest pusta, to system pusty
//jak jest jeden w queue to system obsługuje, a kolejka jest pusta
void Server::finish() {
    summarize(std::cout);
    cancelAndDelete(departure);
}

void Server::summarize(ostream &str) {
    str << "histogram:" << std::endl;
    queueHist.printPv(str);
    str << std::endl;
    str << "średnia długość kolejki " << queueHist.createPv() << std::endl;
    double meanServiceTime = jobIntervalCalc.calculateMean();
    double mi = 1.0/meanServiceTime;
    str << "Mean Service Time: " << meanServiceTime << std::endl;
    str << "mi: " << mi << std::endl;
    double meanArrivalInterval = messageIntervalCalc.calculateMean();
    double lambda = 1.0/meanArrivalInterval;
    str << "Mean Interarrival Time: " << meanArrivalInterval << std::endl;
    str << "λ: " << lambda << std::endl;
    double sredniCzasPobytu = messageInSystem.calculateMean();
    str << "Średni czas pobytu w systemie: "<< sredniCzasPobytu << std::endl;
    double ro = lambda/mi;
    double EL = ro/(1.0 - ro);
    str<< "ro: " << ro << std::endl;
    str << "E(L) " << EL << std::endl;
    double ES = EL / lambda;
    str << "E(S) " << ES << std::endl;
    double P5 = (1.0 - ro) * pow(ro, 5.0);
    str << "P(5) " << P5 << std::endl;
    double P100 = (1.0 - ro) * pow(ro, 100.0);
    str << "P(100) " << P100 << std::endl;
}


