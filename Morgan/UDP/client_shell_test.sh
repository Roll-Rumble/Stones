$echo = New-Object System.Net.Sockets.UdpClient('3.8.190.125', 12000)
$sendbytes = [System.Text.Encoding]::ASCII.GetBytes("test message")
$echo.Send($sendbytes, $sendbytes.Length)
$echo.Close()
// to run this run  nc -u -l 12000 on server