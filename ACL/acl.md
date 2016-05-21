#Access Control List Implementation
I want to stop any traffic that comes from 192.168.1.0 to my 192.168.5.0 network.

##Configuration for Router 0
```
Network : 192.168.3.0
Interface fa0/0
R0(config-if)#ip add 192.168.3.1 255.255.255.0
R0(config-if)# no shut
```
```
Network : 192.168.2.0
Interface se0/0/1
R0(config-if)#ip add 192.168.2.2 255.255.255.0
R0(config-if)#no shut
```
```
Network : 192.168.4.0 
R0(config)#int se 0/0/0
R0(config-if)#ip add 192.168.4.2 255.255.255.0 
```

##Syntax for routing : ip route <intended network to route to> <mask> <next hop IP>

##Routing for interfaces on Router 0 
```
R0(config)#ip route 192.168.1.0 255.255.255.0 192.168.2.1
R0(config)#ip route 192.168.5.0 255.255.255.0 192.168.4.1
```
-------------------------------------------------------------------
Configuration for Router 1
``` 
Network : 192.168.1.0 
R1(config)#int fa0/1
R1(config-if)#ip add 192.168.1.1 255.255.255.0
R1(config-if)#no shut

Network : 192.168.6.0 
R1(config)#int fa0/0
R1(config-if)#ip add 192.168.6.1 255.255.255.0
R1(config-if)#no shut

Network : 192.168.2.0 
R1(config)#int se0/0/0
R1(config-if)#ip add 192.168.2.1 255.255.255.0
R1(config-if)#clock rate 128000
R1(config-if)#bandwidth 128
R1(config-if)#no shut
```

##Routing for interfaces on Router 1 
```
R1(config)#ip route 192.168.3.0 255.255.255.0 192.168.2.2
R1(config)#ip route 192.168.5.0 255.255.255.0 192.168.6.2
```
----------------------------------------------------------------------

Configuration for Router 2
```
Network : 192.168.6.0 
R2(config)#int fa0/0
R2(config-if)#ip add 192.168.6.2 255.255.255.0
R2(config-if)#no shut

Network : 192.168.4.0 
R2(config)#int se 0/0/1
R2(config-if)#ip add 192.168.4.1 255.255.255.0
R2(config-if)#no shut

Network : 192.168.5.0 
R2(config)#int fa0/1
R2(config-if)#ip add 192.168.5.1 255.255.255.0
R2(config-if)#no shut
```

##Routing for interfaces on Router 2 
```
R2(config)#ip route 192.168.3.0 255.255.255.0 192.168.4.2
R2(config)#ip route 192.168.1.0 255.255.255.0 192.168.6.1 
```

======================================================================= 
##Staic IPs :
```
PC0 :  192.168.1.2 | DG - 192.168.1.1 
PC1 :  192.168.3.2 | DG - 192.168.3.1 
PC2 :  192.168.5.2 | DG - 192.168.5.1 
```
Pings Work : `PC0<-->PC1 | PC0<-->PC2 | PC1<-->PC2` 
=======================================================================

##ACL on Router2 to stop traffic from
``` 
R2(config)#access-list 1 deny 192.168.1.0 0.0.0.255 
R2(config)#access-list 1 permit any
R2(config)#int fa0/1
R2(config-if)#no ip  access-group 1 out
R2(config-if)#exit

R2#sh access-list
Standard IP access list 1
    10 deny 192.168.1.0 0.0.0.255 (4 match(es))
    20 permit any
```	
=======================================================================	

##Pings not working : PC0<--X-->PC2 
##Pings work : PC0<-->PC1 , PC1<-->PC2 

#ACL successfully implemented! 



