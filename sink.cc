#include <vector>
#include <omnetpp.h>

using namespace omnetpp;

class Sink: public cSimpleModule {
protected:
    virtual void handleMessage(cMessage *msgin);
    virtual void finish();
};

Define_Module(Sink);

void Sink::handleMessage(cMessage *msgin) {
//    EV << "message in sink, timestamp: " << msgin->getTimestamp() << std::endl;
    delete msgin;
}

void Sink::finish() {
}
