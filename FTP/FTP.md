#FTP Functionality

##This lab is to test out the FTP service provided by our server. Our server is functional at 10.10.10.1/24.

Access the web-server from your laptop (172.16.10.2) over WiFi. 

```
PC>ping 10.10.10.1

Pinging 10.10.10.1 with 32 bytes of data:

Reply from 10.10.10.1: bytes=32 time=13ms TTL=126
Reply from 10.10.10.1: bytes=32 time=8ms TTL=126
Reply from 10.10.10.1: bytes=32 time=8ms TTL=126
Reply from 10.10.10.1: bytes=32 time=11ms TTL=126

Ping statistics for 10.10.10.1:
    Packets: Sent = 4, Received = 4, Lost = 0 (0% loss),
Approximate round trip times in milli-seconds:
    Minimum = 8ms, Maximum = 13ms, Average = 10ms
```

##FTP into the ftp-server. 
```
PC>ftp 10.10.10.1
Trying to connect...10.10.10.1
Connected to 10.10.10.1
220- Welcome to PT Ftp server
Username:shreyas
331- Username ok, need password
Password:
230- Logged in
(passive mode On)
```

##Log in and search for available options. 
```
ftp>help
         ?
         cd
         delete
         dir
         get
         help
         passive
         put
         pwd
         quit
         rename
```

##List the Files already on the ftp-server
```
ftp>dir

Listing /ftp directory from 10.10.10.1: 
0   : asa842-k8.bin                                      5571584
1   : c1841-advipservicesk9-mz.124-15.T1.bin             33591768
2   : c1841-ipbase-mz.123-14.T7.bin                      13832032
3   : c1841-ipbasek9-mz.124-12.bin                       16599160
4   : c2600-advipservicesk9-mz.124-15.T1.bin             33591768
5   : c2600-i-mz.122-28.bin                              5571584
6   : c2600-ipbasek9-mz.124-8.bin                        13169700
7   : c2800nm-advipservicesk9-mz.124-15.T1.bin           50938004
8   : c2800nm-advipservicesk9-mz.151-4.M4.bin            33591768
9   : c2800nm-ipbase-mz.123-14.T7.bin                    5571584
10  : c2800nm-ipbasek9-mz.124-8.bin                      15522644
11  : c2950-i6q4l2-mz.121-22.EA4.bin                     3058048
12  : c2950-i6q4l2-mz.121-22.EA8.bin                     3117390
13  : c2960-lanbase-mz.122-25.FX.bin                     4414921
14  : c2960-lanbase-mz.122-25.SEE1.bin                   4670455
15  : c2960-lanbasek9-mz.150-2.SE4.bin                   4670455
16  : c3560-advipservicesk9-mz.122-37.SE1.bin            8662192
17  : pt1000-i-mz.122-28.bin                             5571584
18  : pt3000-i6q4l2-mz.121-22.EA4.bin                    3117390
```

##Lets fetch the first file: [asa842-k8.bin] to our laptop 
```
ftp>get asa842-k8.bin

Reading file asa842-k8.bin from 10.10.10.1: 
File transfer in progress...

[Transfer complete - 5571584 bytes]

5571584 bytes copied in 19.105 secs (66821 bytes/sec)
```

##Quit and check in your laptop if you got the file 
```
ftp>quit

Packet Tracer PC Command Line 1.0
PC>221- Service closing control connection.

PC>dir
 
 Volume in drive C has no label.
 Volume Serial Number is 5E12-4AF3
 Directory of C:\
  12/31/1969  18:0 PM       5571584             asa842-k8.bin       <-------- OUR FILE 
  2/7/2106    0:28 PM       26                  sampleFile.txt      
                            5571610 bytes       2 File(s)     
```

##Now, I'd like to send a file to the ftp-server. I made a text file named 'ParanoidAndroid.txt' 
```

PC>dir
 
 Volume in drive C has no label.
 Volume Serial Number is 5E12-4AF3
 Directory of C:\
  12/31/1969  18:0 PM       1136                ParanoidAndroid.txt <----- New File. 
  12/31/1969  18:0 PM       5571584             asa842-k8.bin       
  2/7/2106    0:28 PM       26                  sampleFile.txt      
                            5572746 bytes       3 File(s)     
```
##Lets Send it. 
```
ftp>put ParanoidAndroid.txt

Writing file ParanoidAndroid.txt to 10.10.10.1: 
File transfer in progress...

[Transfer complete - 1136 bytes]

1136 bytes copied in 0.036 secs (31555 bytes/sec)
```

##Checking on the server if the files there
```
ftp>dir

Listing /ftp directory from 10.10.10.1: 
0   : ParanoidAndroid.txt                                1136 <------ Sent File 
```

##You can use any PC to FTP into the server and get ParanoidAndroid.txt 