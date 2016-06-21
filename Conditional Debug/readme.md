#Conditional Debug
Make it so that the screen only outputs of certain protocols and not flood the screen with everything.

![topology](https://github.com/shreyasgune/network-simulations/blob/master/Conditional%20Debug/screenshot.png)
Assumption : IPs, OSPF and RIP has been configured before hand. I have individual labs for those if you want.

Thom only shows RIP information on fa0/0. 

When you do `Router# debug ?` , you get a sea of information, thats just too much.
You can narrow it down to access-list specific options by : 
```
radiohead#debug ip packet ?
  <1-199>      Access list
  <1300-2699>  Access list (expanded range)
  detail       Print more debugging detail
  <cr>
```

##Lets check on the RIP debug , see what we're getting..
```
radiohead#debug ip rip
RIP protocol debugging is on
```
##We can see RIP packets sent to Loopback 
```
*Mar  1 00:05:32.287: RIP: sending v2 update to 224.0.0.9 via Loopback0 (1.1.1.1)
*Mar  1 00:05:32.287: RIP: build update entries
*Mar  1 00:05:32.287:   2.2.2.0/24 via 0.0.0.0, metric 2, tag 0
*Mar  1 00:05:32.291:   192.168.12.0/24 via 0.0.0.0, metric 1, tag 0
*Mar  1 00:05:32.295: RIP: ignored v2 packet from 1.1.1.1 (sourced from one of our addresses)
```

##We can see RIP packets sent to fa0/0 (192.168.12.1)
```
radiohead#
*Mar  1 00:05:34.623: RIP: sending v2 update to 224.0.0.9 via FastEthernet0/0 (192.168.12.1)
*Mar  1 00:05:34.623: RIP: build update entries
*Mar  1 00:05:34.623:   1.1.1.0/24 via 0.0.0.0, metric 1, tag 0
radiohead#
*Mar  1 00:05:43.463: RIP: received v2 update from 192.168.12.2 on FastEthernet0/0
*Mar  1 00:05:43.463:      2.2.2.0/24 via 0.0.0.0 in 1 hops
radiohead#no de all
All possible debugging has been turned off
```

##What if, I want to only see debug info from fa0/0 and ignore loopback0's debug info
```
radiohead#
radiohead#debug  condition ?
  application  Application
  called       called number
  calling      calling
  card         card
  glbp         interface group
  interface    interface
  ip           IP address
  mac-address  MAC address
  match-list   apply the match-list
  standby      interface group
  username     username
  vcid         VC ID
  vlan         vlan

radiohead#debug  condition interface ?
  Async              Async interface
  BVI                Bridge-Group Virtual Interface
  CDMA-Ix            CDMA Ix interface
  CTunnel            CTunnel interface
  Dialer             Dialer interface
  FastEthernet       FastEthernet IEEE 802.3
  Group-Async        Async Group interface
  Lex                Lex interface
  Loopback           Loopback interface
  MFR                Multilink Frame Relay bundle interface
  Multilink          Multilink-group interface
  Null               Null interface
  Port-channel       Ethernet Channel of interfaces
  Tunnel             Tunnel interface
  Vif                PGM Multicast Host interface
  Virtual            Virtual interface
  Virtual-PPP        Virtual PPP interface
  Virtual-Template   Virtual Template interface
  Virtual-TokenRing  Virtual TokenRing

radiohead#debug  condition interface fa0/0
Condition 1 set
```

##Let's run the RIP debug again and see what we get..
```
radiohead#debug ip rip
RIP protocol debugging is on
radiohead#
*Mar  1 00:11:09.943: RIP: sending v2 update to 224.0.0.9 via FastEthernet0/0 (192.168.12.1)
*Mar  1 00:11:09.943: RIP: build update entries
*Mar  1 00:11:09.943:   1.1.1.0/24 via 0.0.0.0, metric 1, tag 0
radiohead#
*Mar  1 00:11:20.751: RIP: received v2 update from 192.168.12.2 on FastEthernet0/0
*Mar  1 00:11:20.751:      2.2.2.0/24 via 0.0.0.0 in 1 hops
radiohead#
*Mar  1 00:11:35.855: RIP: sending v2 update to 224.0.0.9 via FastEthernet0/0 (192.168.12.1)
*Mar  1 00:11:35.855: RIP: build update entries
*Mar  1 00:11:35.855:   1.1.1.0/24 via 0.0.0.0, metric 1, tag 0
radiohead#no de all
All possible debugging has been turned off
```

As we see above, only RIP updates for fa0/0 appear. Hence, we have debugged, conditionally. 
