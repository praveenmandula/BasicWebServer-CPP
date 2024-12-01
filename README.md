# BasicWebServer-CPP
This project is useful to learn WebServer concepts using CPP or C++ 

Project Details
----------------
1) BasicWebServer     --  Responsible for initializing BasicWebServer with specified port, listens and responds to 
                          client requests, handles session management
						  
2) DBManager          --  Manages DB communication ( creating and validating users , as of now only sqlite db supported )

3) HtmlReader         --  Reads Html related files, and sends them in HttpResponse

4) HttpStreamHandler  --  Parses http stream data received on socket to user define HttpRequest structure and 
                          converts HttpResponse structure to http stream
						  
5) LogTrace           --  For Debug Logging , use LogMessage Macro for unified logging

6) TcpSocketHandler   --  Used for low level socket communication , receives and sends data on the socket


Steps to Build
--------------

Windows
-------
1) Open BasicWebServer-CPP/BasicWebServer.sln ( VisualStudio 2022 required ) ( Compiling with C++20 )
2) Select Debug/Relase , x64
3) Build Solution, once it get succeeded, post build event will copy Config,WebHostedFiles folders from Common directory to Build Output Directory ( Debug or Release - where exe will get generate )

Linux
------
Pending


Steps to Run
------------
1) Debug from Visual studio 
( BasicWebServer - startup project,command argument given as 8085 ) Debug -> StartDebugging
								OR 
  From Command prompt, Navigate to Build output directory, Run - BasicWebServer.exe 8085 ( port num can be configured )
  
2) Open Browser -> http://localhost:8085 , Login page will come, First time user please Register, next Login

3) Once login succeeded, Upload page will get loaded, you can upload any file and it saves into the Server