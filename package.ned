simple Source
{
    parameters:
        volatile double interarrival_time; 
        volatile int initial_queue;   
        volatile int id;            
									 					  
    gates:
        output out;
}

simple Server
{
    parameters:
        volatile double service_time;
        int queue_size;
        volatile int id;       
    gates:
        input in1;
        input in2;
        output out;
}

simple Sink
{
    gates:
        input in;
}

module GG1
{
	parameters:
	
    submodules:
        source1: Source {
            parameters:
                @display("i=block/source1");
        }
        source2: Source {
            parameters:
                @display("i=block/source2");
        }
        server1: Server {
            parameters:
                @display("i=block/queue1");
        }
        server2: Server {
            parameters:
                @display("i=block/queue2");
        }
        sink: Sink {
            parameters:
                @display("i=block/sink");
        }
    connections allowunconnected:
        source1.out --> {  } --> server1.in1;
        source2.out --> {  } --> server1.in2;
        server1.out --> {  } --> server2.in1;
        server2.out --> {  } --> sink.in;
}

network gg1 extends GG1
{
    parameters:
}
