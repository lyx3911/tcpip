if(WSAStartup(MAKEWORD(2,2),&wsaData)!=0) 
        ErrorHandling("WSAStartup() error");