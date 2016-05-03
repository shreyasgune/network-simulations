#Connectivity

###We have 3 DHCP pools on the router serving VLAN 10,20 and 30. 
Those pools have given out first usable addresses in each subnet.(172.16.) - 10.1 , 11.1 and 11.65
```
Router0-customer#show ip dhcp binding
IP address       Client-ID/              Lease expiration        Type
                 Hardware address
172.16.10.1      0009.7C62.85CC           --                     Automatic
172.16.11.1      0001.43E1.40A5           --                     Automatic
172.16.11.65     0002.17D2.03EE           --                     Automatic
```

##we are going to add another entry to this, the wireless host, let's configure the wireless host.

Go to Access Point 0
```
>Config Tab
	change SSID to : SALES
	Channel : 6
	Authenticaion : WPA2-PSK
		PSK Pass Phrase : shreyasgune
		Encryption : AES
		
##Go to Laptop
>Switch off Laptop
>Take the wired-NIC out
>Slide the Wireless-NIC in
```
>Config Tab	
	change SSID to : SALES    ##make sure the SSID on Wireless-NIC matches SSID on Access Point
	Authenticaion : WPA2-PSK
		PSK Pass Phrase : shreyasgune
		Encryption : AES
```		
##check DHCP binding
```
Router0-customer#show ip dhcp binding
IP address       Client-ID/              Lease expiration        Type
                 Hardware address
172.16.10.1      0009.7C62.85CC           --                     Automatic
172.16.10.2      0040.0B49.430E           --                     Automatic
172.16.11.1      0001.43E1.40A5           --                     Automatic
172.16.11.65     0002.17D2.03EE           --                     Automatic
```
##Laptop0 Ping checks
pinging same subnet
```
Laptop0>ping 172.16.10.1

Pinging 172.16.10.1 with 32 bytes of data:

Reply from 172.16.10.1: bytes=32 time=21ms TTL=128
Reply from 172.16.10.1: bytes=32 time=9ms TTL=128
Reply from 172.16.10.1: bytes=32 time=13ms TTL=128
Reply from 172.16.10.1: bytes=32 time=12ms TTL=128

Ping statistics for 172.16.10.1:
    Packets: Sent = 4, Received = 4, Lost = 0 (0% loss),
Approximate round trip times in milli-seconds:
    Minimum = 9ms, Maximum = 21ms, Average = 13ms
```

Pinging a different subnet 
```
Laptop0>ping 172.16.11.65

Pinging 172.16.11.65 with 32 bytes of data:

Request timed out.
Reply from 172.16.11.65: bytes=32 time=9ms TTL=127
Reply from 172.16.11.65: bytes=32 time=15ms TTL=127
Reply from 172.16.11.65: bytes=32 time=11ms TTL=127

Ping statistics for 172.16.11.65:
    Packets: Sent = 4, Received = 3, Lost = 1 (25% loss),
Approximate round trip times in milli-seconds:
    Minimum = 9ms, Maximum = 15ms, Average = 11ms
```
	
##The router is used to switch between subnets - packet switching and path determination. When we pinged that remote subnet, by doing an ANDING operation on the IP address and Subnet Mask, the devicescan tell that the pinger needs to go up the default gateway to get to the correct pinged subnet.
```
PC>arp -a
  Internet Address      Physical Address      Type
  172.16.10.1           0009.7c62.85cc        dynamic    ## Same network
  172.16.10.254         0001.6377.8301        dynamic    ##MAC address of Router (DG)
``` 
##The router DG was used to get to the remote subnet.

##Ping 10.10.10.10
```
Pinging 10.10.10.1 with 32 bytes of data:

Reply from 172.16.10.254: Destination host unreachable.
Reply from 172.16.10.254: Destination host unreachable.
Reply from 172.16.10.254: Destination host unreachable.
Reply from 172.16.10.254: Destination host unreachable.

Ping statistics for 10.10.10.1:
    Packets: Sent = 4, Received = 0, Lost = 4 (100% loss),
```
##If we go to the router and look at Router0 IP Table
```
ip route
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
C       172.16.10.0/24 is directly connected, FastEthernet0/0.10   [SALES]
C       172.16.11.0/26 is directly connected, FastEthernet0/0.20   [RESEARCH]
C       172.16.11.64/27 is directly connected, FastEthernet0/0.30  [MANAGEMENT]
C       172.16.11.96/28 is directly connected, FastEthernet0/0.40  [SERVERS]
```

##we see that the access to 10.10.10.0 network is not on our ip route. In order to route traffic that is not
##listed in our routing table, we should send it out of our serial 0/0/0 link that goes out to the internet.

```
Router0-customer(config)#ip route 0.0.0.0 0.0.0.0 serial 0/0/0
```

##so , ip routes now shows :

```
Gateway of last resort is 0.0.0.0 to network 0.0.0.0

     88.0.0.0/30 is subnetted, 1 subnets
C       88.40.12.0 is directly connected, Serial0/0/0
     172.16.0.0/16 is variably subnetted, 4 subnets, 4 masks
C       172.16.10.0/24 is directly connected, FastEthernet0/0.10
C       172.16.11.0/26 is directly connected, FastEthernet0/0.20
C       172.16.11.64/27 is directly connected, FastEthernet0/0.30
C       172.16.11.96/28 is directly connected, FastEthernet0/0.40
S*   0.0.0.0/0 is directly connected, Serial0/0/0

##over on the internet side, we have the Router1(ISP). This needs to send the traffic back to the corporate network.
##Corporate network has the IP : 172.16.10.0/23. This is the destination route. The destination interface is the interface that is on the Router0(customer) that goes by the address 88.40.12.1. so basically, we're routing anything that is destined for 172.16.10.0/23 to reach 88.40.12.1 and the Router0-customer will take care of the traffic from there on.

```
ISP(config)#ip route 172.16.10.0 255.255.254.0 88.40.12.1 
```

##now ping 10.10.10.1 using laptop and see what you get.

```
PC>ping 10.10.10.1

Pinging 10.10.10.1 with 32 bytes of data:

Request timed out. ##this is dropped due to ARP
Reply from 10.10.10.1: bytes=32 time=16ms TTL=126
Reply from 10.10.10.1: bytes=32 time=23ms TTL=126
Reply from 10.10.10.1: bytes=32 time=11ms TTL=126

Ping statistics for 10.10.10.1:
    Packets: Sent = 4, Received = 3, Lost = 1 (25% loss),
Approximate round trip times in milli-seconds:
    Minimum = 11ms, Maximum = 23ms, Average = 16ms
```

##Check if the web server is running
##The web-server is located at 10.10.10.1 
##Open the laptop, go to browser, browse to 10.10.10.1 - if a page shows up, the web server is running kosher.

##Set up DNS
##Switch DNS service on, on the Services Tab.

```
Add a "A" Record : 
	1. www.shreyas.com A 10.10.10.10
	2. www.internal.com A 172.16.11.97 
```
  