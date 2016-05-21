/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
//The above line is for emacs. The dudes who made ns3 like a particular format
//so I'm not going to mess with that part at all.
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 */

/*Below we have the header files. Neatly combined to include and load other needed
files recursively. Not the most efficient way to do things but a bit easier. No 
lame name collisions or compilation errors due to some directory errors.*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;
//ns3 is implemented in a C++ namespace called ns3. This is merely a scope resolution thing.

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");
//basically enables and disables console logging by reference to the name.

int
main (int argc, char *argv[]) 
{
  Time::SetResolution (Time::NS); //smallest time value that can be represented.

  //The two lines below are to log in what is going on when packets are being exchanged.
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NS_LOG_INFO("Creating Topology"); //logging directive
  //use this to enable : $export NS_LOG=FirstScriptExample =info

  //The two lines below just creates 2 nodes. This is default code.
  NodeContainer nodes;
  nodes.Create (2);


  //Create a object pointToPoint of class : PointToPointHelper 
  //This is important to configure and connect ns3 PointToPointNetDevices
  //and PointToPointChannel objects in the script.
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  //Data Rate = 5Mbps
  //Delay = 2ms (Transmission Delay)


  ///for every node in the NodeContainer, a PointToPointNetDevice
  //is created and saved in the device container.
  //A pointTopointChannel is created and the two PointToPointNetDevices
  //are attached.
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);
  //The devices are now installed on nodes with pointToPoint drivers (so to speak)
  //and are connected together via a pointTopoint channel between 'em.

  InternetStackHelper stack; //creates an internet stack
  stack.Install (nodes); // installs that stack on the nodes.

  Ipv4AddressHelper address; //creates an IP address container
  address.SetBase ("10.1.1.0", "255.255.255.0"); //stores IP and netMask.

  Ipv4InterfaceContainer interfaces = address.Assign (devices);
  //This actually assigns the IP to the devices that exist on the nodes.


// NOTE : NODE '0' :: CLIENT
// NOTE : NODE '1' :: SERVER

//----------------SERVER-------------------

  //creates an instance of the echoServer at port '9' which is passed
  //as an argument to the constructor.
  UdpEchoServerHelper echoServer (9);

//This is a bit tricky to understand but I'm going to try here..
// echoServer.Install is going to install a UdpEchoSeverApplication on the 
//node found at index number '1' of the nodeContainer we use to manager
//our nodes.
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0)); //time to start the sever
  serverApps.Stop (Seconds (10.0)); //time to stop the server

  //one thing to note here is that we're explicitly changing the double 1.0 to 
  //an ns-3 time object by casting  Seconds(1.0) . Remember this.

//---------------SERVER---------------------


//---------------CLIENT---------------------

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  //creating a helper  and telling it to set the remote (destination) address
  //of the client to the IP address assigned to the node on which the 
  //server resides. Send the packets to port '9' as well.

  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  //maximum number of packets allowed during simulation.

  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  //wait time between two packets.


  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
  //Size of each packet - 1024 bytes

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  //This creates a clientApps instance of the Application class
  //it also uses the clientApp instance to install it on Node '0'

  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

//---------------CLIENT-----------------------

  //To create a tracing file for all that is happening.
  AsciiTraceHelper ascii; 
  pointToPoint.EnableAsciiAll(ascii.CreateFileStream("myfirst.tr"));
  //.pcap tracing
  pointToPoint.EnablePcapAll("myfirst");


//Run this bad boy.
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;


  /* 
  A Litte info : what actually happens.
  We have scheduled the events at 1.0 , 2.0 and 2 events at 10.0 seconds.
  When Simulator:Run() is called, 
  @1.0 , server is enabled.
  @2.0 , client is enabled.
  @A packet is sent. MaxPackets = 1.
  Rest : Simulation is idle.
  @10.0 Client and Server Stop.

  Usually, if there are no more events, simulation goes into stop mode.
  But when we got events like RIPs or FlowMonitor, we gotta use STOP to gracefully
  Stop the simulation.
  */
}

/* Building this 
shreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ cp examples/tutorial/first.cc scratch/myfirst.cc
shreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ ./waf
Waf: Entering directory `/home/shreyas/workspace/ns-allinone-3.23/ns-3.23/build'
[ 890/2314] cxx: scratch/myfirst.cc -> build/scratch/myfirst.cc.3.o
[2286/2314] cxxprogram: build/scratch/myfirst.cc.3.o -> build/scratch/myfirst
Waf: Leaving directory `/home/shreyas/workspace/ns-allinone-3.23/ns-3.23/build'
'build' finished successfully (8.947s)


shreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ ./waf --run scratch/myfirst
Waf: Entering directory `/home/shreyas/workspace/ns-allinone-3.23/ns-3.23/build'
Waf: Leaving directory `/home/shreyas/workspace/ns-allinone-3.23/ns-3.23/build'
'build' finished successfully (2.734s)
At time 2s client sent 1024 bytes to 10.1.1.2 port 9
At time 2.00369s server received 1024 bytes from 10.1.1.1 port 49153
At time 2.00369s server sent 1024 bytes to 10.1.1.1 port 49153
At time 2.00737s client received 1024 bytes from 10.1.1.2 port 9
shreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ 

when : $export 'NS_LOG=UdpEchoServerApplication=level_all|prefix_time'
for ./waf --run <file>/yourfile ==> OUTPUT is...
'build' finished successfully (2.680s)
0s UdpEchoServerApplication:UdpEchoServer(0x8c50738)
1s UdpEchoServerApplication:StartApplication(0x8c50738)
At time 2s client sent 1024 bytes to 10.1.1.2 port 9
2.00369s UdpEchoServerApplication:HandleRead(0x8c50738, 0x8c50b58)
2.00369s At time 2.00369s server received 1024 bytes from 10.1.1.1 port 49153
2.00369s Echoing packet
2.00369s At time 2.00369s server sent 1024 bytes to 10.1.1.1 port 49153
At time 2.00737s client received 1024 bytes from 10.1.1.2 port 9
10s UdpEchoServerApplication:StopApplication(0x8c50738)
UdpEchoServerApplication:DoDispose(0x8c50738)
UdpEchoServerApplication:~UdpEchoServer(0x8c50738)


PCAP TRACES::

shreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ tcpdump -nn -tt -r myfirst-0-0.pcap
reading from file myfirst-0-0.pcap, link-type PPP (PPP)
2.000000 IP 10.1.1.1.49153 > 10.1.1.2.9: UDP, length 1024
2.007372 IP 10.1.1.2.9 > 10.1.1.1.49153: UDP, length 1024
shreyas@shreyas-VirtualBox:~/workspace/ns-allinone-3.23/ns-3.23$ tcpdump -nn -tt -r myfirst-1-0.pcap
reading from file myfirst-1-0.pcap, link-type PPP (PPP)
2.003686 IP 10.1.1.1.49153 > 10.1.1.2.9: UDP, length 1024
2.003686 IP 10.1.1.2.9 > 10.1.1.1.49153: UDP, length 1024



*/