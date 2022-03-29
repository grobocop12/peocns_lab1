simple Source
{
    parameters:
        volatile double interarrival_time; 
        volatile int initial_queue;        
        int source_id;
									 					  
    gates:
        output out;
}

simple Server
{
    parameters:
        volatile double service_time;
        int queue_size;           
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
                @display("i=block/source1");
        }
        server: Server {
            parameters:
                @display("i=block/queue");
        }
        sink: Sink {
            parameters:
                @display("i=block/sink");
        }
    connections:
        source1.out --> {  } --> server.in1;
        source2.out --> {  } --> server.in2;
        server.out --> {  } --> sink.in;
}

network gg1 extends GG1
{
    parameters:
}
