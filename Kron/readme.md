#Network Management I - Cron Task
Save running config every day. It's just like Cron for Linux. 

##Set up Kron Job with a name called SAVEConfig
```
Cron(config)#kron ?
  occurrence   Define the name, time, interval of kron occurrence
  policy-list  Define the name and type of policy-list

Cron(config)#kron occurrence SAVEConfig ?
  at    Date of kron occurrence eg. 14:30 Feb 13
  in    Delta time to kron occurrence
  user  Set the user name
```

##Define the Occurrence of when you wan't the task to fire automatically.
```
Cron(config)#kron occurrence SAVEConfig at 20:00 ?
  <1-31>     Day of month
  DAY        Day of Week eg mon, tue, etc
  MONTH      Month of year eg jan, feb, etc
  oneshot    Schedule kron occurrence exactly once
  recurring  Schedule kron occurrence repeatedly

Cron(config)#kron occurrence SAVEConfig at 20:00 sat ?
  oneshot    Schedule kron occurrence exactly once
  recurring  Schedule kron occurrence repeatedly
```

##Decide whether you want it happening as a single shot or every defined time-date
`Cron(config)#kron occurrence SAVEConfig at 20:00 sat recurring`

##Now you need to decide the policy-list
```
Cron(config-kron-occurrence)#?
KRON Policies to execute at specified occurrences:
  exit         Exit from kron submode
  no           Remove a Policy List
  policy-list  Specify the Policy List to be executed

Cron(config-kron-occurrence)#policy-list SAVECONFIG
Kron: Policy Accepted, Policy needs to be configured
```

##Configure the Policy List 
```
Cron#conf t
Enter configuration commands, one per line.  End with CNTL/Z.
Cron(config)#kron policy-list SAVECONFIG
Cron(config-kron-policy)#?
KRON Specific commands for this Policy:
  cli   Specify the exec level cli to be executed
  exit  Exit from kron submode
  no    Remove a CLI from the list

Cron(config-kron-policy)#cli ?
  LINE  Exec level cli to be executed

Cron(config-kron-policy)#cli write memory
```
`cli-write memory` writes the config at the defined time to the memory. 

##Verify Kron Task and Policy Configured
```
Cron#show run
Building configuration...

Current configuration : 530 bytes
!
!
!
!
kron occurrence SAVEConfig at 20:00 Sat recurring
 policy-list SAVECONFIG
!
kron policy-list SAVECONFIG
 cli write memory
!
```

##Check if it works.
```
Cron#show kron schedule
Kron Occurrence Schedule
SAVEConfig inactive, will run again in 1 days 19:55:49 at 20:00 on Sat
```
Right now, its inactive.

##I'm going to change the clock settings on my router to trigger cron-job work. 
```
Cron#debug kron all
All kron debug flags are on

Cron#clock set 19:59:50 10 december 2011 <--- this is a Saturday
*Dec 10 19:59:50.000: Clock Set Seen
*Dec 10 19:59:50.003: %SYS-6-CLOCKUPDATE: System clock has been updated from 00:05:48 UTC Fri Mar 1 2002 to 19:59:50 UTC Sat Dec 10 2011, configured from console by console.
Dec 10 19:59:50.003: Major 4, Minor 9
Dec 10 19:59:50.007: Start timer for SAVEConfig, expires in 60000 Msec
```

##watch it happen 
```

Cron# show kron schedule
Kron Occurrence Schedule
SAVEConfig inactive, will run again in 0 days 00:00:07 at 20:00 on Sat <--countdown

Cron# show kron schedule
Kron Occurrence Schedule
SAVEConfig inactive, will run again in 0 days 00:00:06 at 20:00 on Sat <--countdown 

------------------------------AUTO TRIGGERED-----------------------
Cron#
Dec 10 20:00:50.007: Major 1, Minor 0
Dec 10 20:00:50.007: Timer Event SAVEConfig
Dec 10 20:00:50.007: Kron delay for next SAVEConfig 604800000
Dec 10 20:00:50.015: Call parse_cmd 'write memory'
Dec 10 20:00:52.155: Kron CLI return 0
'
**CLI 'write memory':
Building configuration...[OK]'
Dec 10 20:00:52.159: Major 4, Minor 7
Dec 10 20:00:52.159: Respond to end of CLI Process
------------------------------AUTO TRIGGERED------------------------

Cron#show kron schedule
Kron Occurrence Schedule
SAVEConfig inactive, will run again in 6 days 23:59:25 at 20:00 on Sat
```


  


