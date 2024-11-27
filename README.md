# BasicWebServer-CPP
This project is created to learn WebServer concepts using CPP or C++ 
( handling httpRequest, Authentication other WebServer concepts ) without any third party softwares

Project Details
----------------
1) BasicWebServer     -- Responsible for initializing BasicWebServer with specified port, listens and responds to requests
2) TcpSocketHandler   -- Used for low level socket communication
3) HttpStreamHandler  -- Parses received data on socket to user define HttpRequest Structure and viceversa
4) HtmlReader         -- Reads Html related files
5) DBManager          -- Manages DB communication, as of now only sqlite db supported
6) LogTrace           -- For Debug Logging
