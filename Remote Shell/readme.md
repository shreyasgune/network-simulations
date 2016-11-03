#Network Management - IV : Remote Shell Access
This is a simple remote login into Radiohead's shell  : access between two network devices to run commands.


##Topology

![Topo](https://github.com/shreyasgune/network-simulations/blob/master/Remote%20Shell/screenshot.png)
##Configure Topology for network : 192.168.12.0 /24

###Router- Radiohead
```
Router#conf t
Enter configuration commands, one per line.  End with CNTL/Z.
Router(config)#hostname Radiohead
Radiohead(config)#interface fa0/0
Radiohead(config-if)#ip address 192.168.12.2 255.255.255.0
Radiohead(config-if)#no shutdown

*Mar  1 00:01:45.399: %LINK-3-UPDOWN: Interface FastEthernet0/0, changed state to up
*Mar  1 00:01:46.399: %LINEPROTO-5-UPDOWN: Line protocol on Interface FastEthernet0/0, changed state to up
```

###Router - Beatles
```
Router#conf t
Enter configuration commands, one per line.  End with CNTL/Z.
Router(config)#hostname Beatles
Beatles(config)#interface fa0/0
Beatles(config-if)#ip address 192.168.12.1 255.255.255.0
Beatles(config-if)#no shutdown

*Mar  1 00:01:33.947: %LINK-3-UPDOWN: Interface FastEthernet0/0, changed state to up
*Mar  1 00:01:34.947: %LINEPROTO-5-UPDOWN: Line protocol on Interface FastEthernet0/0, changed state to up

```

Check
```
Radiohead#show ip int br
Interface                  IP-Address      OK? Method Status                Protocol
FastEthernet0/0            192.168.12.2    YES manual up                    up

Beatles#show ip int br
Interface                  IP-Address      OK? Method Status                Protocol
FastEthernet0/0            192.168.12.1    YES manual up                    up
```

##Enable Remote Shell on Radiohead 
```
Radiohead#conf t
Enter configuration commands, one per line.  End with CNTL/Z.
Radiohead(config)#ip rcmd rsh-enable
Radiohead(config)#ip rcmd ?
  domain-lookup     Enable IP DNS queries for Rcmd
  rcp-enable        Enable server side of RCP
  remote-host       Set host+user equivalence
  remote-username   Set system's username for rcp
  rsh-enable        Enable server side of RSH
  source-interface  Specify interface for source address in rcmd connections

Radiohead(config)#ip rcmd remote-host Radiohead ?
  <1-99>               Access list number
  Hostname or A.B.C.D  Remote hostname or IP address

Radiohead(config)#ip rcmd remote-host Radiohead 192.168.12.1 ?
  WORD  Remote username

Radiohead(config)#ip rcmd remote-host Radiohead 192.168.12.1 Beatles ?
  enable  Allow enable-mode commands
  <cr>

Radiohead(config)#ip rcmd remote-host Radiohead 192.168.12.1 Beatles enable
*Mar  1 00:08:46.819: %SYS-5-CONFIG_I: Configured from console by console
```

Switch on the debug to see what is going on.
```
Radiohead#debug ip tcp rcmd
RCMD transactions debugging is on
```

##Beatles Side : 192.168.12.1 
Beatles#sh ip int br
Interface                  IP-Address      OK? Method Status                Protocol
FastEthernet0/0            192.168.12.1    YES NVRAM  up                    up
Loopback0                  1.1.1.1         YES NVRAM  up                    up
Beatles#
Beatles#rsh 192.168.12.2 /user Radiohead show run

Building configuration...

Current configuration : 683 bytes
!
version 12.4
service timestamps debug datetime msec
service timestamps log datetime msec
no service password-encryption
!
hostname Radiohead
!
boot-start-marker
boot-end-marker
!
!
no aaa new-model
memory-size iomem 5
!
!
ip cef
no ip domain lookup

ip rcmd rsh-enable
ip rcmd remote-host Radiohead 192.168.12.1 Beatles enable

interface FastEthernet0/0
 ip address 192.168.12.2 255.255.255.0
 duplex auto
 speed auto

no ip http server
no ip http secure-server
ip route 1.1.1.1 255.255.255.255 192.168.12.1

control-plane

line con 0
 exec-timeout 0 0
 logging synchronous
line aux 0
line vty 0 4
 login

end
```

##Radiohead Sees 
```
Radiohead#
*Mar  1 00:02:41.379: RCMD: [514 <- 192.168.12.1:1023] recv \0
*Mar  1 00:02:41.379: RCMD: [514 <- 192.168.12.1:1023] recv Turtle\0Radiohead\0show run\0
*Mar  1 00:02:41.383: RCMD: [514 -> 192.168.12.1:1023] send <OK>
Radiohead#
```




