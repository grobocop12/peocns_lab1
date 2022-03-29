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
        volatile int initial_message_count;       
    gates:
        input in;
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
        server1: Server {
            parameters:
                @display("i=block/queue1");
        }
        server2: Server {
            parameters:
                @display("i=block/queue2");
        }
    connections allowunconnected:
        server1.out --> {  } --> server2.in;
        server2.out --> {  } --> server1.in;
}

network gg1 extends GG1
{
    parameters:
}
