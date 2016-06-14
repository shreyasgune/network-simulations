#Configuring Time Sync using NTP

##Topology
![Topology](https://github.com/shreyasgune/network-simulations/blob/master/NTP%20(Network%20Time%20Protocol)/ntp_topo.JPG)
Network Time Protocol (NTP) is a networking protocol for clock synchronization between computer systems over packet-switched, variable-latency data networks.

Refer to the topology diagram for IP addresses.

##Preconfigs : IP addresses on interfaces, EIGRP for full connectivity.

##Configure router Thom and Jonny to be NTP Masters, use stratum 10.
###Thom 
```
Thom(config)#ntp master ?
  <1-15>  Stratum number
  <cr>

Thom(config)#ntp master 10
Thom(config)#ntp ?
  access-group        Control NTP access
  authenticate        Authenticate time sources
  authentication-key  Authentication key for trusted time sources
  broadcastdelay      Estimated round-trip delay
  clock-period        Length of hardware clock tick
  logging             Enable NTP message logging
  master              Act as NTP master clock
  max-associations    Set maximum number of associations
  peer                Configure NTP peer
  server              Configure NTP server
  source              Configure interface for source address
  trusted-key         Key numbers for trusted time sources

Thom(config)#ntp peer 192.168.12.2
```

###Jonny 
```
Jonny(config)#ntp peer 192.168.12.1
Jonny(config)#ntp master 10
```


##Make sure router Thom and Jonny synchronize with each other.
Set Clock for Thom 
```
Thom#clock set 20:27:00 20 may 2016
Thom#
.May 20 20:27:00.000: %SYS-6-CLOCKUPDATE: System clock has been updated from 00:04:01 UTC Fri Mar 1 2002 to 20:27:00 UTC Fri May 20 2016, configured from console by console.
```
Set Clock for Jonny
```
Jonny#clock set 20:27:00 20 may 2016
Jonny#
.May 20 20:27:00.000: %SYS-6-CLOCKUPDATE: System clock has been updated from 00:04:15 UTC Fri Mar 1 2002 to 20:27:00 UTC Fri May 20 2016, configured from console by console.
```

##Router Colin has to check router Thom and Jonny for NTP updates, it has to prefer router Jonny.
```
Colin(config)#ntp server 192.168.12.1 ? <--- this is Thom
  key      Configure peer authentication key
  prefer   Prefer this peer when possible
  source   Interface for source address
  version  Configure NTP version
  <cr>

Colin(config)#ntp server 192.168.12.2 prefer ? <--- this is Jonny 
  key      Configure peer authentication key
  version  Configure NTP version
  <cr>

Colin(config)#ntp server 192.168.12.1
Colin(config)#ntp server 192.168.12.2 prefer 
```

##Configure router Colin so it broadcasts NTP packets on its FastEthernet 1/0 interface.
The NTP packets that Colin recieves, get pushed on to the fa1/0 interface that links to Ed.
```
Colin(config)#int f1/0
Colin(config-if)#ntp ?
  broadcast  Configure NTP broadcast service
  disable    Disable NTP
  multicast  Configure NTP multicast service
  
Colin(config-if)#ntp broadcast
```

##Configure router Ed so it configures itself using the NTP broadcast packets from router Colin.
```
Ed(config)#int f0/0
Ed(config-if)#ntp broadcast ?
  client       Listen to NTP broadcasts
  destination  Configure broadcast destination address
  key          Configure broadcast authentication key
  version      Configure NTP version
  <cr>

Ed(config-if)#ntp broadcast client
```

##Configure router Thom to send NTP packets to multicast address 239.5.5.5.

Enable Muilticast Routing in Thom : 
```
Thom(config)#ip multicast-routing
Thom(config)#int fa2/0
Thom(config-if)#ip pim dense-mode
May 20 20:42:12.303: %PIM-5-NBRCHG: neighbor 192.168.15.5 UP on interface FastEthernet2/0
May 20 20:42:12.423: %PIM-5-DRCHG: DR change from neighbor 0.0.0.0 to 192.168.15.5 on interf ace FastEthernet2/0
```

Enable Muilticast Routing in Phil : 
```
Phil(config)#ip multicast-routing`
Phil(config)#int fa0/0
Phil(config-if)#ip pim dense-mode
Phil(config-if)#
*Mar  1 00:18:57.435: %PIM-5-DRCHG: DR change from neighbor 0.0.0.0 to 192.168.15.5 on interface FastEthernet0/0
Phil(config-if)#
*Mar  1 00:19:14.355: %PIM-5-NBRCHG: neighbor 192.168.15.1 UP on interface FastEthernet0/0
```

Check
```
Phil#show ip pim interface fastEthernet 0/0

Address          Interface                Ver/   Nbr    Query  DR     DR
                                          Mode   Count  Intvl  Prior
192.168.15.5     FastEthernet0/0          v2/D   1      30     1      192.168.15.5
Phil#

Thom#show ip pim interface fastEthernet 2/0

Address          Interface                Ver/   Nbr    Query  DR     DR
                                          Mode   Count  Intvl  Prior
192.168.15.1     FastEthernet2/0          v2/D   1      30     1      192.168.15.5
```

Send those multicast packets from Thom 
```
Thom#conf t
Enter configuration commands, one per line.  End with CNTL/Z.
Thom(config)#int f2/0
Thom(config-if)#ntp multicast 239.5.5.5
```

##Configure router Phil to configure itself using the NTP multicast packets.
```
Phil(config)#int f0/0
Phil(config-if)#ntp multicast ?
  A.B.C.D  Multicast group IP address
  client   Listen to NTP multicasts
  key      Configure multicast authentication key
  ttl      TTL of the multicast packet
  version  Configure NTP version
  <cr>

Phil(config-if)#ntp multicast client 239.5.5.5
```

##Verify (Keep in mind, that it's going to take time for the NTP to be distributed)
Also, the Stratums increase due to hop size.
###Thom
```
Thom#show ntp status
Clock is synchronized, stratum 10, reference is 127.127.7.1
nominal freq is 250.0000 Hz, actual freq is 250.0000 Hz, precision is 2**24
reference time is DAE9F71C.999648D9 (20:50:36.599 UTC Fri May 20 2016)
clock offset is 0.0000 msec, root delay is 0.00 msec
root dispersion is 0.02 msec, peer dispersion is 0.02 msec

Thom#show ntp associations

      address         ref clock     st  when  poll reach  delay  offset    disp
*~127.127.7.1      127.127.7.1       9    59    64  377     0.0    0.00     0.0
+~192.168.12.2     127.127.7.1      10    47    64  376    44.0  -13882    15.5
 * master (synced), # master (unsynced), + selected, - candidate, ~ configured

 
```

###Jonny 
```
Jonny#show ntp status
Clock is synchronized, stratum 10, reference is 127.127.7.1
nominal freq is 250.0000 Hz, actual freq is 250.0000 Hz, precision is 2**24
reference time is DAE9F730.8D422E56 (20:50:56.551 UTC Fri May 20 2016)
clock offset is 0.0000 msec, root delay is 0.00 msec
root dispersion is 0.02 msec, peer dispersion is 0.02 msec

Jonny#show ntp associations

      address         ref clock     st  when  poll reach  delay  offset    disp
*~127.127.7.1      127.127.7.1       9     2    64  377     0.0    0.00     0.0
+~192.168.12.1     127.127.7.1      10    54    64  376    63.9  13884.    21.7
 * master (synced), # master (unsynced), + selected, - candidate, ~ configured


```

###Phil
```
Phil#show ntp status
Clock is synchronized, stratum 11, reference is 192.168.15.1
nominal freq is 250.0000 Hz, actual freq is 250.0000 Hz, precision is 2**18
reference time is DAE9F724.57992D1F (20:50:44.342 UTC Fri May 20 2016)
clock offset is -52.2698 msec, root delay is 140.01 msec
root dispersion is 15927.32 msec, peer dispersion is 15875.02 msec

Phil#show ntp associations

      address         ref clock     st  when  poll reach  delay  offset    disp
* 192.168.15.1     127.127.7.1      10    59    64  376   140.0   80.10    78.6
 * master (synced), # master (unsynced), + selected, - candidate, ~ configured

```

Multicast Route Information 
```
Phil#show ip mroute
IP Multicast Routing Table
Flags: D - Dense, S - Sparse, B - Bidir Group, s - SSM Group, C - Connected,
       L - Local, P - Pruned, R - RP-bit set, F - Register flag,
       T - SPT-bit set, J - Join SPT, M - MSDP created entry,
       X - Proxy Join Timer Running, A - Candidate for MSDP Advertisement,
       U - URD, I - Received Source Specific Host Report,
       Z - Multicast Tunnel, z - MDT-data group sender,
       Y - Joined MDT-data group, y - Sending to MDT-data group
Outgoing interface flags: H - Hardware switched, A - Assert winner
 Timers: Uptime/Expires
 Interface state: Interface, Next-Hop or VCD, State/Mode

(*, 239.5.5.5), 00:05:22/stopped, RP 0.0.0.0, flags: DCL
  Incoming interface: Null, RPF nbr 0.0.0.0
  Outgoing interface list:
    FastEthernet0/0, Forward/Dense, 00:05:22/00:00:00

(192.168.15.1, 239.5.5.5), 00:05:12/00:02:03, flags: PLT <-- recieves multicast stream
  Incoming interface: FastEthernet0/0, RPF nbr 0.0.0.0
  Outgoing interface list: Null

(*, 224.0.1.40), 00:11:41/00:02:31, RP 0.0.0.0, flags: DCL
  Incoming interface: Null, RPF nbr 0.0.0.0
  Outgoing interface list:
    FastEthernet0/0, Forward/Dense, 00:11:45/00:00:00
```

###Colin 
```
Colin#show ntp status
Clock is synchronized, stratum 11, reference is 192.168.12.2
nominal freq is 250.0000 Hz, actual freq is 249.9998 Hz, precision is 2**18
reference time is DAE9F813.F5A002B1 (20:54:43.959 UTC Fri May 20 2016)
clock offset is -13879.5282 msec, root delay is 23.96 msec
root dispersion is 14028.43 msec, peer dispersion is 148.86 msec

Colin#show ntp associations

      address         ref clock     st  when  poll reach  delay  offset    disp
*~192.168.12.1     127.127.7.1      10     5    64  377   120.0   15.86    22.2
 ~192.168.12.2     127.127.7.1      10    56    64  377    59.9  -13861    42.2
 * master (synced), # master (unsynced), + selected, - candidate, ~ configured

```

###Ed
```
Ed#show ntp status
Clock is synchronized, stratum 12, reference is 192.168.34.3
nominal freq is 250.0000 Hz, actual freq is 250.0000 Hz, precision is 2**18
reference time is DAE9F8AE.E6263C66 (20:57:18.899 UTC Fri May 20 2016)
clock offset is 2.1401 msec, root delay is 192.03 msec
root dispersion is 15900.39 msec, peer dispersion is 15876.97 msec


Ed#show ntp associations

      address         ref clock     st  when  poll reach  delay  offset    disp
* 192.168.34.3     192.168.12.1     11    16    64  136    68.0    8.93  1901.0
 * master (synced), # master (unsynced), + selected, - candidate, ~ configured

```
