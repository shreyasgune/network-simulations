/* In this example today, we are going to simulate the bus topology.
I know it sounds quite exciting. All those figures in the textbooks and 
I still didn't quite get it now, did I? I'll walk you all through the 
standard procedures, so don't you worry my chumps!

Date : 8/5/2015
Time : 11:48 AM
Music : Richard Wagner : Rise of the Valkyries. 

Let's carck on.

*/

//Getting headers sorted.

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/application-module.h"
#include "ns3/ipv4-global-routing-helper.h"

/* Description of the topology

            10.1.1.0
n0 ------------------------- n1  n2  n3  n4 
point-to-point :: ---------  |    |   |   |
                           ==================
                               LAN 10.1.2.0

*/

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("BUS Topology with CSMA");

int main (int argc, char *argv[])
{

bool verbose = true; //flag to determine whether the UdpEchoClientApp and UdpEchoServer App logging is enableed.
//turn the logging off when regression testing.
uint32_t nCsma =3; // set the number of csma devices to 3.

CommandLine cmd;

cmd.AddValue ("nCsma","Number of \"extra\" CSMA nodes/devices ," nCsma);
cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);

cmd.Parse(argc,argv);

if(verbose)
{
	LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
	LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);
}

nCsma = nCsma == 0 ? 1 : nCsma; //making sure we have atleast one extra node.

NodeContainer p2pNodes;
p2pNodes.Create(2); //created two p2p nodes.

NodeContainer csmaNodes;
csmaNodes.Add(p2pNodes.Get(1));
csmaNodes.Create(nCsma);
//node 1 is going to be on the p2p device as well as the csma device.
//rest of the nodes created become the part of the CSMA section.

//Setting up attributes for P2P
PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
pointToPoint.SetDeviceAttribute("Delay", StringValue("2ms"));

NetDeviceContainer p2pDevices;
p2pDevices = pointToPoint.Install(p2pNodes); //device installed on the nodes.

//Setting up attributes for CSMA
CsmaHelper csma;
csma.SetChannelAttribute("DataRate",StringValue("100Mbps"));
csma.SetChannelAttribute("Delay",TimeValue(NanoSeconds(6560)); //1 nanosecond delay per foot over a 100 meter segment.

NetDeviceContainer csmaDevices;
csmaDevices = csma.Install(csmaNodes); //device installed on CSMAnodes


//Setting up the Internet Stack
InternetStackHelper stack;
stack.Install(p2pNodes.Get(0));
stack.Install(csmaNodes);

//Assigning IP's
Ipv4AddressHelper address;
address.SetBase("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer p2pInterfaces;
p2pInterfaces = address.Assign(p2pDevices); //Addresses assigned to p2p dvices.

//Now all we need to do is assign the address to CSMA Devices interfaces.
address.SetBase("10.1.2.0","255.255.255.0");
Ipv4InterfaceContainer csmaInterfaces;
csmaInterfaces = address.assign(csmaDevices);

//Now we are going to instantiate the server on one of the nodes that has a CSMA
//device and the client on the node having only a point2point device.

UdpEchoServerHelper echoServer (9);

ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(nCsma));
serverApps.Start(Seconds(1.0));
serverApps.Stop(Seconds(10.0));
//the sever is installed on one of the csma nodes and starts @1.0 and ends @10.0


//Client Configuration

UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress(nCsma),9);
echoClient.SetAttribute("MaxPackets",UintegerValue(1));
echoClient.SetAttribute("Interval",TimeValue(Seconds (1.0)));
echoClient.SetAttribute("PacketSize",UintegerValue(1024));

ApplicationContainer clientApps = echoClient.Install(p2pNodes.Get(0));
clientApps.Start(2.0);
clientApps.Stop(10.0);

//client Configuration done and installed on p2p node 0 , which starts @2.0 and stops @10.0

///Taking care of the routing.
/* ns3 gives us a very easy way to handle routing cuz it assumes that since you have 
set up the entire internetwork, all of it is accesible in our simulation. It sets up everything related to routing.

Each node behaves as if it were a OSPF router that communicates instantly with all other routers.
Each node generates link advertisements and communicates them directly to global route manager
which uses the global information to construct the routing tables for each node.
*/

//Taking care of the routing with this one magic line.
Ipv4GlobalRoutingHelper :: PopulateRoutingTables();

//Let us have pcap tracing please.
pointToPoint.EnablePcapAll("Point2PointTrace");
csma.EnablePcap("csmaTrace", csmaDevices.Get(1),true);

/*
CSMA is a multi point to point network. There are multiple end-points on a shared medium
Each of these endpoints has a net devices associated with it. There are two ways to gather the trace.
1.create a trace file for each net device and store only the packets that are 
emitted or consumed by that very device.
2.pick one of the the dvice and place it in promiscous mode. Single device then sniffs the networkfor all 
packets and stores them in a single pcap file. This seems to be the better option. We need to see all that is 
going on in our network. Also, I think mulitple pcap files is too much of a hassle. Wouldn't you agree?

That is what we have done in line 138.
*/

//lets run this bad boy
Simulator::Run();
Simulator::Destroy();
return 0;

}


/*

hreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ export NS_LOG=
shreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ ./waf --run scratch/bus
Waf: Entering directory `/home/shreyas/workspace/ns-allinone-3.23/ns-3.23/build'
Waf: Leaving directory `/home/shreyas/workspace/ns-allinone-3.23/ns-3.23/build'
'build' finished successfully (2.357s)
At time 2s client sent 1024 bytes to 10.1.2.4 port 9
:UDP echo client sent a packet to the server

At time 2.0078s server received 1024 bytes from 10.1.1.1 port 49153
:EchoServer got packet 
At time 2.0078s server sent 1024 bytes to 10.1.1.1 port 49153
At time 2.01761s client received 1024 bytes from 10.1.2.4 port 9

After changing the number of CSMA devices
shreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ ./waf --run "scratch/bus --nCsma=4"
Waf: Entering directory `/home/shreyas/workspace/ns-allinone-3.23/ns-3.23/build'
Waf: Leaving directory `/home/shreyas/workspace/ns-allinone-3.23/ns-3.23/build'
'build' finished successfully (2.283s)
At time 2s client sent 1024 bytes to 10.1.2.5 port 9
At time 2.0118s server received 1024 bytes from 10.1.1.1 port 49153
At time 2.0118s server sent 1024 bytes to 10.1.1.1 port 49153
At time 2.02461s client received 1024 bytes from 10.1.2.5 port 9

*/
