#Configuring VLANs


##Configure trunk on SW0
```
SW0(config)#interface range fastEthernet 0/23 - 24
SW0(config-if-range)#switchport mode trunk 
```

##configure trunk on SW1
```
SW1(config)#interface range fastEthernet 0/23 - 24
SW1(config-if-range)#switchport mode trunk
```

##configure trunk on SW2
```
SW2(config)#interface range fastEthernet 0/23 - 24
SW2(config-if-range)#switchport mode trunk
```

##copying configuration
```
SW0#wr
SW0#copy running-config startup-config 
Destination filename [startup-config]? 
Building configuration...
[OK]
```
##Now, we need to set up the VLAN Trunking Protocol.
```
SW0#conf t
Enter configuration commands, one per line.  End with CNTL/Z.
SW0(config)#vtp mode ?
  client       Set the device to client mode.
  server       Set the device to server mode.
  transparent  Set the device to transparent mode.
 
SW0(config)#vtp mode server
Device mode already VTP SERVER.
SW0(config)#vtp domain CISCO
Changing VTP domain name from NULL to CISCO
SW0(config)#vtp password gune
Setting device VLAN database password to gune
```

##Check configuration 

For Switch 1 
```
SW1#show vtp status
VTP Version                     : 2
Configuration Revision          : 0
Maximum VLANs supported locally : 255
Number of existing VLANs        : 5
VTP Operating Mode              : Server
VTP Domain Name                 : CISCO
VTP Pruning Mode                : Disabled
VTP V2 Mode                     : Disabled
VTP Traps Generation            : Disabled
MD5 digest                      : 0x1A 0xFC 0x64 0xDA 0x8E 0xA1 0x8A 0x3B 
Configuration last modified by 0.0.0.0 at 0-0-00 00:00:00
Local updater ID is 0.0.0.0 (no valid interface found)
```
For Switch 2
```
SW2#show vtp status
VTP Version                     : 2
Configuration Revision          : 0
Maximum VLANs supported locally : 255
Number of existing VLANs        : 5
VTP Operating Mode              : Server
VTP Domain Name                 : CISCO
VTP Pruning Mode                : Disabled
VTP V2 Mode                     : Disabled
VTP Traps Generation            : Disabled
MD5 digest                      : 0x1A 0xFC 0x64 0xDA 0x8E 0xA1 0x8A 0x3B 
Configuration last modified by 0.0.0.0 at 0-0-00 00:00:00
Local updater ID is 0.0.0.0 (no valid interface found)
```
## Set up Security for Switch 2 
```
SW2(config)#vtp password gune
Setting device VLAN database password to gune
SW2#show vtp password
VTP Password: gune
```

##Check the VLANs that are defined on Switch 2 
```
SW2#show vlan brief

VLAN Name                             Status    Ports
---- -------------------------------- --------- -------------------------------
1    default                          active    Fa0/1, Fa0/2, Fa0/3, Fa0/4
                                                Fa0/5, Fa0/6, Fa0/7, Fa0/8
                                                Fa0/9, Fa0/10, Fa0/11, Fa0/12
                                                Fa0/13, Fa0/14, Fa0/15, Fa0/16
                                                Fa0/17, Fa0/18, Fa0/19, Fa0/20
                                                Fa0/21, Fa0/22, Fa0/23
1002 fddi-default                     active    
1003 token-ring-default               active    
1004 fddinet-default                  active    
1005 trnet-default                    active
```

##Check trunk interfaces 
```
SW2#sh int trunk
Port        Mode         Encapsulation  Status        Native vlan
Fa0/24      on           802.1q         trunking      1

Port        Vlans allowed on trunk
Fa0/24      1-1005

Port        Vlans allowed and active in management domain
Fa0/24      1

Port        Vlans in spanning tree forwarding state and not pruned
Fa0/24      1
```

##Instantiate VLANs 

```
SW0#conf t
Enter configuration commands, one per line.  End with CNTL/Z.
SW0(config)#vlan 10 
SW0(config-vlan)#name SALES
SW0(config-vlan)#exit
SW0(config)#vlan 20
SW0(config-vlan)#name RESEARCH
SW0(config-vlan)#exit
SW0(config)#vlan 30
SW0(config-vlan)#name MANAGEMENT
SW0(config-vlan)#exit
SW0(config)#vlan 40
SW0(config-vlan)#name SERVERS
SW0(config-vlan)#exit
SW0(config)#^Z
```
##Check settings
```
SW0#
%SYS-5-CONFIG_I: Configured from console by console

SW0#show vtp status
VTP Version                     : 2
Configuration Revision          : 9
Maximum VLANs supported locally : 255
Number of existing VLANs        : 9
VTP Operating Mode              : Server
VTP Domain Name                 : CISCO
VTP Pruning Mode                : Disabled
VTP V2 Mode                     : Disabled
VTP Traps Generation            : Disabled
MD5 digest                      : 0xC4 0xE5 0x30 0xFE 0x97 0x6F 0x30 0x21 
Configuration last modified by 0.0.0.0 at 3-2-93 04:02:36
Local updater ID is 0.0.0.0 (no valid interface found)
```

```
SW0#show vlan brief

VLAN Name                             Status    Ports
---- -------------------------------- --------- -------------------------------
1    default                          active    Fa0/1, Fa0/2, Fa0/3, Fa0/4
                                                Fa0/5, Fa0/6, Fa0/7, Fa0/8
                                                Fa0/9, Fa0/10, Fa0/11, Fa0/12
                                                Fa0/13, Fa0/14, Fa0/15, Fa0/16
                                                Fa0/17, Fa0/18, Fa0/19, Fa0/20
                                                Fa0/21, Fa0/22, Fa0/24
10   SALES                            active    
20   RESEARCH                         active    
30   MANAGEMENT                       active    
40   SERVERS                          active    
1002 fddi-default                     active    
1003 token-ring-default               active    
1004 fddinet-default                  active    
1005 trnet-default                    active  
```

##Putting the ports to VLANs

Switch 0
``` 
SW0(config)#interface range fastEthernet 0/1 - 20
SW0(config-if-range)#switchport mode access
SW0(config-if-range)#swithport access vlan 10
```

Switch 1
```
SW1(config)#interface range fastEthernet 0/1 - 20
SW1(config-if-range)#switchport mode access
SW1(config-if-range)#switchport access vlan 20
```

Switch 2 
```
SW2(config)#interface range fastEthernet 0/1 - 9
SW2(config-if-range)#switchport mode access
SW2(config-if-range)#switchport access vlan 30
SW2(config)#interface range fastEthernet 0/10 - 20
SW2(config-if-range)#switchport  mode access
SW2(config-if-range)#switchport access vlan 40
```


##Showing VLAN briefs for each switch

SW0
```
VLAN Name                             Status    Ports
---- -------------------------------- --------- -------------------------------
1    default                          active    Fa0/21, Fa0/22, Fa0/24
10   SALES                            active    Fa0/1, Fa0/2, Fa0/3, Fa0/4
                                                Fa0/5, Fa0/6, Fa0/7, Fa0/8
                                                Fa0/9, Fa0/10, Fa0/11, Fa0/12
                                                Fa0/13, Fa0/14, Fa0/15, Fa0/16
                                                Fa0/17, Fa0/18, Fa0/19, Fa0/20
```

SW1
```
VLAN Name                             Status    Ports
---- -------------------------------- --------- -------------------------------
1    default                          active    Fa0/21, Fa0/22  
20   RESEARCH                         active    Fa0/1, Fa0/2, Fa0/3, Fa0/4
                                                Fa0/5, Fa0/6, Fa0/7, Fa0/8
                                                Fa0/9, Fa0/10, Fa0/11, Fa0/12
                                                Fa0/13, Fa0/14, Fa0/15, Fa0/16
                                                Fa0/17, Fa0/18, Fa0/19, Fa0/20
```

SW2
```
VLAN Name                             Status    Ports
---- -------------------------------- --------- -------------------------------
1    default                          active    Fa0/21, Fa0/22, Fa0/23    
30   MANAGEMENT                       active    Fa0/1, Fa0/2, Fa0/3, Fa0/4
                                                Fa0/5, Fa0/6, Fa0/7, Fa0/8
                                                Fa0/9
40   SERVERS                          active    Fa0/10, Fa0/11, Fa0/12, Fa0/13
                                                Fa0/14, Fa0/15, Fa0/16, Fa0/17
                                                Fa0/18, Fa0/19, Fa0/20
												
```

##Now to put sub-interfaces on the router. These will be the default gateways for the subnets that belong to those VLANS. These sub-internets were created to cater to the 4 VLANs we just defined. 
##Encapsulation is dot1Q for the trunk links.
```
Router0(config)#interface fastEthernet 0/0.10												
Router0(config-subif)#encapsulation dot1Q 10
Router0(config-subif)#ip address 172.16.10.254 255.255.255.0

Router0(config)#interface fastEthernet 0/0.20												
Router0(config-subif)#encapsulation dot1Q 20
Router0(config-subif)#ip address 172.16.11.62 255.255.255.192

Router0(config)#interface fastEthernet 0/0.30												
Router0(config-subif)#encapsulation dot1Q 30
Router0(config-subif)#ip address 172.16.11.94 255.255.255.224

Router0(config)#interface fastEthernet 0/0.40												
Router0(config-subif)#encapsulation dot1Q 40
Router0(config-subif)#ip address 172.16.11.94 255.255.255.240
```

##Check config for Router 0 
```
Router0#sh ip int br
Interface              IP-Address      OK? Method Status                Protocol
 
FastEthernet0/0        unassigned      YES unset  up                    up
 
FastEthernet0/0.10     172.16.10.254   YES manual up                    up
 
FastEthernet0/0.20     172.16.11.62    YES manual up                    up
 
FastEthernet0/0.30     172.16.11.94    YES manual up                    up
 
FastEthernet0/0.40     172.16.11.110   YES manual up                    up
```

##Hence, all our VLAN's are now UP! 
