#Configuring and testing Mail Server 

##In this lab, I'll be configuring email service on my server at 10.10.10.1/24 

##First, Set the domain , create two unique mail IDs.

```
SMTP : ON
POP3 : ON 
Domain : mail.radiohead.com
``` 

```
Users : 
Name              Password
shreyas             gune
jonny             greenwood 
```

##Create a DNS A-Record for this mail Server 
`mail.radiohead.com  Type: A Record   Detail : 10.10.10.1 `

##Set Up Configuration for your users on any host on the network 
```
Your Name : Shreyas@Laptop(172.16.10.1)  / Jonny@PC0(172.16.10.2)
e-mail : shreyas@mail.radiohead.com / jonny@mail.radiohead.com 
Incoming Mail Server : mail.radiohead.com 
Outgoing Mail Serve : mail.radiohead.com
Logon : user/pass - shreyas/gune   |  jonny/greenwood 
```

##Compose the mail from Laptop
```
To : jonny@mail.radiohead.com 
Subject : A Moon Shaped Pool 
Message : The New Album is Amazing!

SEND!
```
What's happening behind the scenes.
```
Sending mail to jonny@mail.radiohead.com , with subject : A Moon Shaped Pool .. 
Mail Server : mail.radiohead.com 
DNS Resolving. Resolving name : mail.radiohead.com by querying to DNS Server 10.10.10.1 
DNS Resolved IP :10.10.10.1 
Send Success~
```

##Lets check at PC0 if Jonny got the email. 
You have to click on the 'Receive' tab to actively receive the mail. You'll see the entry

```
Receiving mail from POP3 Server mail.radiohead.com
DNS Resolving. Resolving name : mail.radiohead.com by querying to DNS Server 10.10.10.1 
DNS Resolved IP :10.10.10.1 
Receive Mail Success 
```
What the mail reads ...
```
A Moon Shaped Pool
shreyas@mail.radiohead.com
Sent : Sun May 8 201617:15:44

The new album is amazing! 
____________________________
```








