#Remote Access VPN Config
##Aim is to create a remote access user that will be coming in through a DSL modem.
![Topo](https://github.com/shreyasgune/network-simulations/blob/master/Remote%20Access%20VPN/remote-access-vpn.JPG)
##Additional Components to the Network : 
	PC - Remote Access PC
	MODEM - DSL Modem0
	Cloud - Cloud0
	
##config the Cloud0
Goto Config Tab
```
	DSL
		Modem4 <---> Ethernet6 [ADD]
```
##Set up the new cloud network
```
IP : 72.44.20.0/28 (4 bits for hosts, which means it can hold 16 hosts)
Network Address : 72.44.20.1 | Broadcast Address : 72.44.20.15 | Hosts : 72.44.20.2 ~ 72.44.20.14
```

For Router0's fa 0/1 , add ip : 72.44.20.14  as a default gateway.
```
Router0-customer(config)#interface fa 0/1
Router0-customer(config-if)#ip add 72.44.20.14 255.255.255.240
Router0-customer(config-if)#no shutdown
%LINK-5-CHANGED: Interface FastEthernet0/1, changed state to up
%LINEPROTO-5-UPDOWN: Line protocol on Interface FastEthernet0/1, changed state to up
```

##Set up DHCP pool on the router0
```
Router0-customer#conf t
Enter configuration commands, one per line.  End with CNTL/Z.
Router0-customer(config)#ip dhcp pool REMOTE_POOL
Router0-customer(dhcp-config)#network 72.44.20.0 255.255.255.240
Router0-customer(dhcp-config)#default-router 72.44.20.14 
Router0-customer(dhcp-config)#dns-server 10.10.10.1 
```

##Go into the PC and check if you have a dynamic IP : It should work.

##Configuring AAA (Authentication Authorization Accounting)
```
Router0-customer(config)#aaa new-model   ##creates a new AAA model 
Router0-customer(config)#aaa authentication ?
  enable  Set authentication lists for enable.
  login   Set authentication lists for logins.
  ppp     Set authentication lists for ppp.
Router0-customer(config)#aaa authentication login REMOTE local ##ties back to 'local' security db on router
Router0-customer(config)#aaa authorization network REMOTE local
Router0-customer(config)#username VPN secret supersecret ## puts username : VPN and Password: supersecret into 'local' db.
```

#Setting up the VPN

##Set up IKE Phase 1
```
Router0-customer(config)#crypto isakmp policy 10
Router0-customer(config-isakmp)#encryption aes 256
Router0-customer(config-isakmp)#hash md5
Router0-customer(config-isakmp)#authentication pre-share
Router0-customer(config-isakmp)#group 2
Router0-customer(config-isakmp)#lifetime 21600
Router0-customer(config-isakmp)#exit
```

##Set up a group for the VPN
```
Router0-customer(config)#crypto isakmp client configuration group REMOTE
Router0-customer(config-isakmp-group)#key GUNE
Router0-customer(config-isakmp-group)#pool MYPOOL
Router0-customer(config-isakmp-group)#exit
```

##Set up IKE Phase 2 
`Router0-customer(config)#crypto ipsec transform-set MYSET esp-aes 256 esp-md5-hmac`

##Set up Dynamic Map 
```
Router0-customer(config)#crypto dynamic-map DYMAP 10
Router0-customer(config-crypto-map)#set transform-set MYSET
Router0-customer(config-crypto-map)#reverse-route  ##reverse-route injection is the ability to inject a static route in the routing process
```

##Set up Crypto-Map
```
Router0-customer(config)#crypto map CLIENT_MAP client authentication list REMOTE
Router0-customer(config)#crypto map CLIENT_MAP isakmp authorization list REMOTE
Router0-customer(config)#crypto map CLIENT_MAP client configuration address respond
Router0-customer(config)#crypto map CLIENT_MAP 10 ipsec-isakmp dynamic DYMAP
```

##Create IP Pool 
`Router0-customer(config)#ip local pool MYPOOL 172.16.10.150 172.16.10.200`

##I chose these address range because I wanted to hand over some addresses from VLAN 10 sales, so that PC-RemoteAccess can connect, but none of them should overlap with the DHCP handed addresses to PC0 and so on. .150 to .200 is safe.

##Apply Crypto-Map to Interface fa 0/1
```
Router0-customer(config)#interface fa 0/1
Router0-customer(config-if)#crypto map CLIENT_MAP
*Jan  3 07:16:26.785: %CRYPTO-6-ISAKMP_ON_OFF: ISAKMP is ON
```

##check
```
Router0-customer#sh crypto isakmp sa
IPv4 Crypto ISAKMP SA
dst             src             state          conn-id slot status
                                                                    
IPv6 Crypto ISAKMP SA
Router0-customer#sh crypto ipsec sa

No SAs found
```
##you don't see anything because the the VPN is not up yet.

##Connect VPN via VPN_client found on PC-RemoteAccess
```
VPN Client 
	GroupName : REMOTE 
	Group Key : GUNE 
	Host IP : 72.44.20.14 ##note that the tunnel runs only from PC-RemoteAccess to the interface on the router. The security
						  ##is only required in the internet. Not when the packet reaches the router and falls into the internal network.
	username : VPN 
	password : supersecret
```
	
##click on connect
```
VPN is Connected! , client IP : 172.16.10.150 , next time it'd be .151 

PC-RemoteAccess>ipconfig /all

FastEthernet0 Connection:(default port)

   Connection-specific DNS Suffix..: 
   Physical Address................: 0006.2A3C.3117
   Link-local IPv6 Address.........: FE80::206:2AFF:FE3C:3117
   IP Address......................: 72.44.20.1
   Subnet Mask.....................: 255.255.255.240
   Default Gateway.................: 72.44.20.14
   DNS Servers.....................: 10.10.10.1
   DHCP Servers....................: 72.44.20.14
   DHCPv6 Client DUID..............: 00-01-00-01-4A-58-8C-34-00-06-2A-3C-31-17

   Tunnel Interface IP Address.....: 172.16.10.151  <---- it appears to be on SALES LAN 
```
   
##Router0's perspective
```
Router0-customer#sh crypto isakmp sa
IPv4 Crypto ISAKMP SA
dst             src             state          conn-id slot status
72.44.20.1      72.44.20.14     QM_IDLE           1037    0 ACTIVE
IPv6 Crypto ISAKMP SA
```

##quickly go back to PC-RemoteAccess and ping the server on the other end : 172.16.11.97
```
Ping statistics for 172.16.11.97:
    Packets: Sent = 4, Received = 3, Lost = 1 (25% loss),
Approximate round trip times in milli-seconds:
    Minimum = 58ms, Maximum = 69ms, Average = 64ms
```

##Back to router0 , do : 
```
Router0-customer#sh crypto ipsec sa

interface: FastEthernet0/1
    Crypto map tag: CLIENT_MAP, local addr 72.44.20.14

   protected vrf: (none)
   local  ident (addr/mask/prot/port): (0.0.0.0/0.0.0.0/0/0)
   remote  ident (addr/mask/prot/port): (172.16.10.150/255.255.255.255/0/0)
   current_peer 72.44.20.1 port 500
    PERMIT, flags={origin_is_acl,}
   #pkts encaps: 0, #pkts encrypt: 0, #pkts digest: 0
   #pkts decaps: 8, #pkts decrypt: 8, #pkts verify: 0 <--- Check this. 
   #pkts compressed: 0, #pkts decompressed: 0
   #pkts not compressed: 0, #pkts compr. failed: 0
   #pkts not decompressed: 0, #pkts decompress failed: 0
   #send errors 0, #recv errors 0

     local crypto endpt.: 72.44.20.14, remote crypto endpt.:72.44.20.1
     path mtu 1500, ip mtu 1500, ip mtu idb FastEthernet0/1
     current outbound spi: 0x67A8211F(1739071775)
```

##Browsing works as well! : look up www.shreyas.com / www.internal.com 
##check :show running-config to cross-check everything on your Router0.




	

