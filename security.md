#Security on the network

![Topology](https://github.com/shreyasgune/network-simulations/blob/master/manetwork.JPG)

##I have now accessed the corporate router
```
Router0-customer#show privilege
Current privilege level is 15 ##this is the most highest level of access.
```

##Set Password for this Router0-customer
```
Router0-customer(config)#line console 0
Router0-customer(config-line)#password shreyas
Router0-customer(config-line)#login
```

When you log out and log back in, you will see a that you now need a password to get inside the system.

##to set minimum length for password ::
```
Router0-customer(config)#security passwords min-length ?
  <0-16>  Minimum length of all user/enable passwords
```  

##Set Router login password
```
Router0-customer(config)#username shreyas password december
Router0-customer(config)#line console 0
Router0-customer(config-line)#login local
```

##for enable secret password
`Router0-customer(config)#username shrey secret november`


##Password for priviledged mode
```
Router0-customer(config)#enable password september ##clear text
Router0-customer(config)#enable secret shreyasshreyas ##encrypted
```

##encrypt all passwords (level 7)
`Router0-customer(config)#service password-encryption`

##Priviledge level 15 access (admin)
`Router0-customer(config)#username gune privilege 15 secret network1234`

##To time out a user after certain time period
```
Router0-customer(config)#line console 0
Router0-customer(config-line)#exec-timeout 1 30
```

##If 5 bad attempts are made in 45 seconds , logout automatically
`Router0-customer(config)#login block-for 120 attempts 5 within 45`

##Set a Banner
`Router0-customer(config)#banner motd <char> <TEXT> <char> `

##Test telnet with any host on the network. Telnet into the Router0-customer
```
PC>telnet 172.16.10.254
Trying 172.16.10.254 ...Open DO NOT LOGIN NOW 
User Access Verification
ACCESS SUCCESSFUL 
```

##Telnet is a bad idea because all exchanges are taking place in clear-text. You don't want that.
##It's safer to set up SSH access.

##for SSH, you need a domain-name for your router. 
`Router0-customer(config)#ip domain-name customer-router`
##SSH creates a public key and a private key. They are based upon your hostname and IP domain name.
##Generate SSH key (2048 bits)

```
Router0-customer(config)#crypto key generate rsa
The name for the keys will be: Router0-customer.customer-router
```
Choose the size of the key modulus in the range of 360 to 2048 for your
  General Purpose Keys. Choosing a key modulus greater than 512 may take
  a few minutes.
```
How many bits in the modulus [512]: 2048
% Generating 2048 bit RSA keys, keys will be non-exportable...[OK]
*Mar 1 0:17:59.11:  %SSH-5-ENABLED: SSH 1.99 has been enabled 
```

##Implement SSH on the vty line
```
Router0-customer(config)#line vty 0 4
Router0-customer(config-line)#transport input ssh
```

##If you try to telnet into the router, you will see that the connection is refused.

##try SSH into the router0
```
PC>ssh -l <username> 172.16.10.254
Open
Password: <password>

 <BANNER>

Router0-customer# ##now we have root access.

##Setting up Switchboard security
SW0(config)#int fa 0/3
SW0(config-if)#switchport port-security
SW0(config-if)#switchport port-security ?
  mac-address  Secure mac address
  maximum      Max secure addresses
  violation    Security violation mode
  <cr>
SW0(config-if)#switchport port-security mac-address sticky
``` 
sticky will add switchport security to first host thats on the interface

##Violation Mode
In this mode,we set the violation to shutdown by default. In this mode it will generate a violation
alert and disable the port if any unknown device with an unknown MAC-address connects to that port
The only way to re-enable the port will be to do : no shutdown

`SW0(config-if)#switchport port-security violation shutdown`
 

##Restrict Mode 
In this mode, frames from non-allowed addresses can be dropped. This also logs entries.

##Protect Mode
Same as restrict, but entry logging is unavailable. 

##To test this out, I've introduced another PC03 into the VLAN10 @ fa 0/3 .It's connected to the SALES switch.
I've went ahead and let DHCP run on it, so now, the PC03 has an IP of 172.16.10.3 and a MAC-address of 000B.BEA3.2E0C

##Switch interface config :: 
```
interface FastEthernet0/3
 switchport access vlan 10
 switchport mode access
 switchport port-security
 switchport port-security mac-address sticky 
 switchport port-security mac-address sticky 000B.BEA3.2E0C <--- check this out.
```

Now if you add another PC04 and unplug PC03 and plug that connection to PC04 and let DHCP run on it.
It's going to FAIL because the port just went dead on the switch. It gets a APIPA address. It's random.

```
SW0(config-if)#do show port-security
Secure Port MaxSecureAddr CurrentAddr SecurityViolation Security Action <----- check this.
               (Count)       (Count)        (Count)
--------------------------------------------------------------------
        Fa0/3        1          1                 1         Shutdown
----------------------------------------------------------------------
```

##Reconnect the old MAC-address PC and bring back the port up
```
SW0(config-if)#shutdown
%LINK-5-CHANGED: Interface FastEthernet0/3, changed state to administratively down
SW0(config-if)#no shutdown
SW0(config-if)#
%LINK-5-CHANGED: Interface FastEthernet0/3, changed state to up
SW0(config-if)#do show interface fa 0/3
FastEthernet0/3 is up, line protocol is up (connected)

================================================================================
```
