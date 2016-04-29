#IP's, Subnetting, VLANs and DHCP Configs
This walkthrough lets you realize the network in the picture.
![Network Topology](https://github.com/shreyasgune/network-simulations/blob/master/manetwork.JPG)

The picture clearly tells you what routers,Switches,Laptops,End Devices and Servers to select.


##Configure static IP on Corporate Intranet Server 0
```
SERVER0>ipconfig /all

FastEthernet0 Connection:(default port)

   Connection-specific DNS Suffix..: 
   Physical Address................: 0060.3E83.3EC4
   Link-local IPv6 Address.........: FE80::260:3EFF:FE83:3EC4
   IP Address......................: 172.16.11.97
   Subnet Mask.....................: 255.255.255.240
   Default Gateway.................: 172.16.11.110
   DNS Servers.....................: 10.10.10.1
   DHCP Servers....................: 0.0.0.0
   DHCPv6 Client DUID..............: 00-01-00-01-06-AA-94-67-00-60-3E-83-3E-C4
```

   
##configure static IP on DNS Server 1 
```
SERVER1>ipconfig /all

FastEthernet0 Connection:(default port)

   Connection-specific DNS Suffix..: 
   Physical Address................: 0050.0FD8.880D
   Link-local IPv6 Address.........: FE80::250:FFF:FED8:880D
   IP Address......................: 10.10.10.1
   Subnet Mask.....................: 255.255.255.0
   Default Gateway.................: 10.10.10.254
   DNS Servers.....................: 10.10.10.1
   DHCP Servers....................: 0.0.0.0
   DHCPv6 Client DUID..............: 00-01-00-01-55-5D-79-D3-00-50-0F-D8-88-0D
```

##Router1 Configuration
```
Router(config)#hostname ISP
ISP(config)#interface fastEthernet 0/0
ISP(config-if)#ip add 10.10.10.254 255.255.255.0
ISP(config-if)#no shutdown

ISP(config)#interface serial 0/0/1
ISP(config-if)#ip address 88.40.12.2 255.255.255.252
ISP(config-if)#clock rate ?
Speed (bits per second
  1200           
  2400           
  4800           
  9600           
  19200          
  38400          
  56000          
  64000          
  72000          
  125000         
  128000         
  148000         
  250000         
  500000         
  800000         
  1000000        
  1300000        
  2000000        
  4000000        
  <300-4000000>  Choose clockrate from list above
ISP(config-if)#clock rate 128000
ISP(config-if)#bandwidth 128
ISP(config-if)#no shutdown

%LINK-5-CHANGED: Interface Serial0/0/1, changed state to down
##The state is down because the other end of the serial link is not up. It will turn up when the customer side router is up. 

ISP#show ip int br
Interface              IP-Address      OK? Method Status                Protocol
 
FastEthernet0/0        10.10.10.254    YES manual up                    up
 
FastEthernet0/1        unassigned      YES unset  administratively down down
 
Serial0/0/0            unassigned      YES unset  administratively down down
 
Serial0/0/1            88.40.12.2      YES manual down                  down
 
Vlan1                  unassigned      YES unset  administratively down down
```

##Configuration for Router0 - customer side
```
Router0(config)#int s0/0/0
Router0(config-if)#ip address 88.40.12.1 255.255.255.252
Router0(config-if)#bandwidth 128
Router0(config-if)#no shutdown

Router0(config-if)#
%LINK-5-CHANGED: Interface Serial0/0/0, changed state to up
Router0#sh ip int br
Interface              IP-Address      OK? Method Status                Protocol
 
FastEthernet0/0        unassigned      YES unset  up                    up
 
FastEthernet0/0.10     172.16.10.254   YES manual up                    up
 
FastEthernet0/0.20     172.16.11.62    YES manual up                    up
 
FastEthernet0/0.30     172.16.11.94    YES manual up                    up
 
FastEthernet0/0.40     172.16.11.110   YES manual up                    up
 
FastEthernet0/1        unassigned      YES unset  administratively down down
 
Serial0/0/0            88.40.12.1      YES manual up                    up
 
Serial0/0/1            unassigned      YES unset  administratively down down
 
Vlan1                  unassigned      YES unset  administratively down down

```

##CHECK CONNECTION STATE
```
Router0#ping 88.40.12.2

Type escape sequence to abort.
Sending 5, 100-byte ICMP Echos to 88.40.12.2, timeout is 2 seconds:
!!!!!
Success rate is 100 percent (5/5), round-trip min/avg/max = 8/9/10 ms
```


##Since there are far too many hosts on the VLANS, lets put up dynamic addressing for them lets look at the routing table

```
Router0-customer#show ip route
Codes: C - connected, S - static, I - IGRP, R - RIP, M - mobile, B - BGP
       D - EIGRP, EX - EIGRP external, O - OSPF, IA - OSPF inter area
       N1 - OSPF NSSA external type 1, N2 - OSPF NSSA external type 2
       E1 - OSPF external type 1, E2 - OSPF external type 2, E - EGP
       i - IS-IS, L1 - IS-IS level-1, L2 - IS-IS level-2, ia - IS-IS inter area
       * - candidate default, U - per-user static route, o - ODR
       P - periodic downloaded static route

Gateway of last resort is not set

     88.0.0.0/30 is subnetted, 1 subnets
C       88.40.12.0 is directly connected, Serial0/0/0
     172.16.0.0/16 is variably subnetted, 4 subnets, 4 masks
C       172.16.10.0/24 is directly connected, FastEthernet0/0.10
C       172.16.11.0/26 is directly connected, FastEthernet0/0.20
C       172.16.11.64/27 is directly connected, FastEthernet0/0.30
C       172.16.11.96/28 is directly connected, FastEthernet0/0.40
```


##To assign and enable DHCP, DNS and Default Router (gateway) to the internet for VLANS

*VLAN 10*
```
Router0-customer(config)#ip dhcp pool VLAN10
Router0-customer(dhcp-config)#172.16.10.0 255.255.255.0
Router0-customer(dhcp-config)#network 172.16.10.0 255.255.255.0
Router0-customer(dhcp-config)#default-router 172.16.10.254
Router0-customer(dhcp-config)#dns-server 10.10.10.1
```

*VLAN20*
```
Router0-customer(config)#ip dhcp pool VLAN20
Router0-customer(dhcp-config)#network 172.16.11.0 255.255.255.192
Router0-customer(dhcp-config)#default-router 172.16.11.62
Router0-customer(dhcp-config)#dns-server 10.10.10.1
```

*VLAN30*
```
Router0-customer(config)#ip dhcp pool VLAN30
Router0-customer(dhcp-config)#network 172.16.11.64 255.255.255.224
Router0-customer(dhcp-config)#default-router 172.16.11.94
Router0-customer(dhcp-config)#dns-server 10.10.10.1
```

##Now go to PC0 , desktop tab, IP-Configuration and select DHCP. You will see that the IP address have been dynamically alloted. 
```
*PC0* 
PC0>ipconfig /all

FastEthernet0 Connection:(default port)

   Connection-specific DNS Suffix..: 
   Physical Address................: 0009.7C62.85CC
   Link-local IPv6 Address.........: FE80::209:7CFF:FE62:85CC
   IP Address......................: 172.16.10.1
   Subnet Mask.....................: 255.255.255.0
   Default Gateway.................: 172.16.10.254
   DNS Servers.....................: 10.10.10.1
   DHCP Servers....................: 172.16.10.254
   DHCPv6 Client DUID..............: 00-01-00-01-C8-E9-61-9B-00-09-7C-62-85-CC
```

*PC1*
```
PC1>ipconfig /all

FastEthernet0 Connection:(default port)

   Connection-specific DNS Suffix..: 
   Physical Address................: 0001.43E1.40A5
   Link-local IPv6 Address.........: FE80::201:43FF:FEE1:40A5
   IP Address......................: 172.16.11.1
   Subnet Mask.....................: 255.255.255.192
   Default Gateway.................: 172.16.11.62
   DNS Servers.....................: 10.10.10.1
   DHCP Servers....................: 172.16.11.62
   DHCPv6 Client DUID..............: 00-01-00-01-C1-C5-D4-CA-00-01-43-E1-40-A5
```

*PC2*

```
PC2>ipconfig /all

FastEthernet0 Connection:(default port)

   Connection-specific DNS Suffix..: 
   Physical Address................: 0002.17D2.03EE
   Link-local IPv6 Address.........: FE80::202:17FF:FED2:3EE
   IP Address......................: 172.16.11.65
   Subnet Mask.....................: 255.255.255.224
   Default Gateway.................: 172.16.11.94
   DNS Servers.....................: 10.10.10.1
   DHCP Servers....................: 172.16.11.94
   DHCPv6 Client DUID..............: 00-01-00-01-7C-D1-45-45-00-02-17-D2-03-EE
```   
   
