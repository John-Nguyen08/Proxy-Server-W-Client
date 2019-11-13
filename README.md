# Proxy-Server-W-Client

To Compile The Proxy Server, make sure to have both files in one directory and within the terminal, move to the directory with these two files:
Open two puttys up, csce01 will be server and csce02 will be client [csce01 and csce02 will be the names of the two puttys]
Compile csce01 with “gcc -o server server.c” without quotes 
Then compile csce02 with “gcc -o client client.c” without quotes
To execute: do “./server 5001” on csce01. If error binding, add 1 to 5000 (so port number becomes = 5001) and repeat until you do not have a binding error
Then execute csce02 with “./client 5002” (same number as server port number).

Notes:
Some things you might wonder when running my code. You can only run one URL and receive one response back before you have to curl c. That response will generate a file containing that URL html body. 
