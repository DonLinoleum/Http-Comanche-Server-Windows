#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

struct sockaddr_in setSockAddr()
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0;
    addr.sin_port = htons(8080);
    return addr;
}

char* setHeaders()
{
    char* headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n";
    return headers;
}

void sendResponse(SOCKET client)
{
    FILE* fp =  fopen("index.html","r");
    char content_buffer[2056] = {0};
    char response_buffer[4112] = {0};
    char* headers = setHeaders();
    size_t count_readed_bytes = fread(content_buffer,1,sizeof(content_buffer),fp);
    size_t count_bytes_to_send = strlen(headers) + count_readed_bytes;
    strcpy(response_buffer,headers);
    strcat(response_buffer,content_buffer);
    
    send(client,response_buffer,count_bytes_to_send,0);
    fclose(fp);
}

int main()
{
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2,2), &wsadata);
    SOCKET s = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in addr = setSockAddr();
    bind(s,(const struct sockaddr *)&addr,sizeof(addr));
    listen(s, SOMAXCONN);

    while (1){
        SOCKET client = accept(s,0,0);
        if (client == INVALID_SOCKET){
            closesocket(client);
            continue;
        }
        char request[2056] = {0};
        recv (client,request,sizeof(request),0);  
        if (memcmp(request, "GET / ", 6) == 0)
            sendResponse(client);
        closesocket(client);
        Sleep(10);
    }
    WSACleanup();
    return 0;
}


    

