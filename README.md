# BasicWebServer-CPP
This project is created to learn WebServer concepts using CPP or C++ 
( Creates and Listens on socket, Handling httpRequest, Authentication and session managment 
  without using any third party libraries )

Project Details
----------------
1) BasicWebServer     -- Responsible for initializing BasicWebServer with specified port, listens and responds to requests
2) TcpSocketHandler   -- Used for low level socket communication , receives and sends data on the socket
3) HttpStreamHandler  -- parses http stream data on socket to user define HttpRequest structure and 
                         converts HttpResponse structure to http stream
4) HtmlReader         -- Reads Html related files
5) DBManager          -- Manages DB communication, as of now only sqlite db supported
6) LogTrace           -- For Debug Logging , use LogMessage Macro for unified logging


Steps to Build and Run
----------------------

Windows
-------
1) Open BasicWebServer-CPP/BasicWebServer.sln ( VisualStudio 2022 required )
2) Select Debug/Relase , x64
3) Build Solution, once it succeed post build event will copies Config,WebHostedFiles folders from Common directory to Build Output Directory

Linux
------
Pending


Steps to Run
------------
1) Debug from visual studio or from command prompt, Navigate to Build output directory
   BasicWebServer.exe 8085 ( port num can be configured )
2) Open Browser -> http://localhost/8085 , Login page will come, First time user please Register, next Login
3) Once login succeed, Upload page will get called, you can upload any file and it saves into the Server

