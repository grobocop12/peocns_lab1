#include <omnetpp.h>
#include <cmath>
#include "histogram.h"

using namespace omnetpp;

class Server : public cSimpleModule
{
private:
    int queueSize;
    cQueue queue;             // the queue; first job in the queue is being serviced
    cMessage *departure;      // message-reminder of the end of service (job departure)
    simtime_t departure_time; // time of the next departure
    simtime_t lastArrival;
    QueueHist queueHist;
    IntervalCalculator departureIntervalCalc;
    IntervalCalculator arrivalIntervalCalc;
    IntervalCalculator messagesLifetimeCalc;
    void summarize(ostream &str);
    void printProbabilities(double ro, ostream &str);

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);
    virtual void finish();
};

Define_Module(Server);

void Server::initialize()
{
    queueSize = par("queue_size").intValue();
    lastArrival = 0;
    departure = new cMessage("Departure");
    queueHist = QueueHist(queueSize);
    departureIntervalCalc = IntervalCalculator();
    arrivalIntervalCalc = IntervalCalculator();
    messagesLifetimeCalc = IntervalCalculator();
}

void Server::handleMessage(cMessage *msgin)
{
    if (msgin == departure) // job departure
    {
        cMessage *msg = (cMessage *)queue.pop(); // remove job from the head of the queue
        messagesLifetimeCalc.put(simTime() - msg->getTimestamp());
        send(msg, "out");
        queueHist.get();
        if (!queue.isEmpty()) // schedule next departure event
        {
            simtime_t interval = par("service_time");
            departureIntervalCalc.put(interval);
            scheduleAt(simTime() + interval, departure);
        }
    }
    else // job arrival
    {
        msgin->setTimestamp(simTime());
        arrivalIntervalCalc.put(simTime() - lastArrival);
        lastArrival = simTime();
        if (queue.isEmpty())
        {
            simtime_t interval = par("service_time");
            departureIntervalCalc.put(interval);
            scheduleAt(simTime() + interval, departure);
        }
        if (queue.getLength() < queueSize)
        {
            queue.insert(msgin); // put job at the end of the queue
            queueHist.put();
        }
        else
        {
            delete msgin;
        }
    }
}
// jak queue jest pusta, to system pusty
// jak jest jeden w queue to system obsługuje, a kolejka jest pusta
void Server::finish()
{
    summarize(std::cout);
    cancelAndDelete(departure);
}

void Server::summarize(ostream &str)
{
    double meanMessagesInSystem = queueHist.createPv();
    double meanServiceTime = departureIntervalCalc.calculateMean();
    double mu = 1.0 / meanServiceTime;
    double meanArrivalInterval = arrivalIntervalCalc.calculateMean();
    double lambda = 1.0 / meanArrivalInterval;
    double meanMessageLifetime = messagesLifetimeCalc.calculateMean();
    double ro = lambda / mu;
    double EN = ro / (1.0 - ro);
    double ES = EN / lambda;
    double ET = ro / (mu - lambda); 
    str.width(10);

    str << "histogram:" << std::endl;
    queueHist.printPv(str);
    str << std::endl;

    str << "λ: " << lambda << std::endl;
    str << "μ: " << mu << std::endl;
    str << "ρ: " << ro << std::endl;
    str << std::endl;
    str << "średnia liczba zadań w systemie: " << std::endl;
    str << "z wzoru ρ/(1-ρ): \t" << EN << std::endl;
    str << "z histogramu: \t\t" << meanMessagesInSystem << std::endl;
    str << std::endl;
    str << "średni czas pobytu w systemie:" << std::endl;
    str << "zmierzony:\t" << meanMessageLifetime << std::endl;
    str << "obliczony:\t" << ES  << std::endl;
    str << std::endl;
    str << "średni czas oczekiwania w kolejce:" << std::endl;
    str << "zmierzony:\t" << meanMessageLifetime - meanServiceTime << std::endl;
    str << "obliczony:\t" << ET << std::endl;
    str << std::endl;
    printProbabilities(ro, str);
}

void Server::printProbabilities(double ro, ostream &str)
{
    //double N[] = {0, 1, 2, 5, 50, 500, 5000000};
    double N[] = {0, 1, 2, (double)queueSize};
    int length = sizeof(N)/sizeof(N[0]);
    for (int i = 0; i < length; i++) {
        long double probability = (1.0 - ro) * pow(ro, N[i]);
        str << "P(" << N[i] << "): " << probability << std::endl;
    }
}
