#include <winsock2.h>

void InitWinsock2() {
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);
}

void DeInitWinsock2() {
    WSACleanup();
}