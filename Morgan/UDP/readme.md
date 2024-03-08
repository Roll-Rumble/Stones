# UDP and TCP
The following folder contains the tcp and udp code

The code uses winsock for the client side and unix for the server side

## EC2 Instance Criteria

### Allowing UDP on the EC2 instance

Step 1: Locate EC2

    In the EC2 Dashboard, look for the "Instances" link on the left-hand sidebar and click on it.
    This will display a list of your EC2 instances.
    Find the instance running your server application and note its "Security Group" (there might be more than one).

Step 2: Security Groups

    In the left-hand sidebar, under "Network & Security," find and click on "Security Groups."
    Find the security group associated with your EC2 instance from Step 3. You might have to compare the names or IDs listed in the "Instances" view.

Step 3: Edit Inbound Rules

    With the correct security group selected, click on the "Actions" button, then "Edit inbound rules."
    This will open a dialogue where you can add new rules or modify existing ones.

Step 4: Add Inbound Rule for UDP

    Click on "Add Rule."
    For "Type," choose "Custom UDP Rule."
    For "Protocol," it should automatically set to UDP.
    For "Port Range," enter the port number your server is listening on (12000 or 13000).
    For "Source," select "Anywhere" to allow traffic from any IPv4 address. This option will fill the field with 0.0.0.0/0, ::/0. For production environments, it's recommended to restrict this to known IP addresses for security reasons.

### Disabling EC2 Firewalls

**UFW**:
```sudo ufw disable```

**firewalld**:
```sudo systemctl stop firewalld```

## Winnsock Compiling

When Compiling the CPP client code file, you must add the following command so that the Winsock libary is included in the compiler
```-lws2_32```

for example:
```g++ -o my_program client_UDP.cpp -lws2_32```

## Troubleshooting


### Address already in use

run this to see if the desired port is already being used in ec2
```sudo lsof -i UDP:12000```
then run this however replace pid with whats returned from the previous command
```sudo kill -9 PID```

### testing client code

run this on the ec2 to listen for UDP packets

```nc -u -l 12000```

### testing server code

run the shell scrpit provided in windows power shell. this will send a packet over udp

```$echo = New-Object System.Net.Sockets.UdpClient('3.8.190.125', 12000)
$sendbytes = [System.Text.Encoding]::ASCII.GetBytes("test message")
$echo.Send($sendbytes, $sendbytes.Length)
$echo.Close()```
