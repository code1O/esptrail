/**
 *
 * Create an self HTTP server for handle incomming ESP32 commands
 * 
 * DEFINED FOR: WIN32 & UNIX SYSTEMS
 * 
 * NET PROTOCOL: TCP/IP
 * WEB PROTOCOL: HTTP
 * USER LIMITED: NO
 * THREADS EXCELLENCY: YES
 *
*/

#include <stdio.h>
#include <cstdlib>
#include <string.h>

typedef (*CallableVoid)(void); // Convert a `void*` function into a callable one
typedef int SOCKET_TYPE, SOCKET_FAM, SOCKET_PROTO;

BOOL Error_ListenQueue, is_ErrorBinding, exists_InvalidSocket;

int recv_data;

#define DEFAULT_PORT 5000
#define BUFFER_SIZE 512
#define THREAD_POOL_SIZE 4

void create_server();
void handle_client(const UINT_PTR sock_client, const char* expected_command, const void* task);
void create_threads();

#if defined(_WIN32)
#pragma comment(lib, "Ws2_32.lib")

#include <winsock2.h>
#include <WS2tcpip.h>

WSADATA wsadata;
SOCKET ServerSocket, ClientSocket;

SOCKET_TYPE socket_T = SOCK_STREAM;
SOCKET_FAM socket_F = AF_INET;
SOCKET_PROTO socket_P = IPPROTO_TCP;

socklen_t addrlen = sizeof(server_addr);

typedef struct {
    SOCKET clientSocket;
    HANDLE event; 
} ThreadData;

struct sockaddr_in server_addr;

void init_data() {
    server_addr.sin_family = socket_F;
    server_addr.sin_port = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int iresult = WSAStartup(MAKEWORD(2, 2), &wsadata);

    if (iresult != 0) printf("Error at trying to create socket: %d\n", WSAGetLastError());
}

void create_server(){

    init_data();
    ServerSocket = socket(socket_F, socket_T, socket_P);
    listen_queue = (listen(ServerSocket, SOMAXCONN) == SOCKET_ERROR);
    ClientSocket = accept(ServerSocket, (struct sockaddr *)&server_addr, &addrlen);

    is_ErrorBinding = ( bind(ClientSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR );
    exists_InvalidSocket = ( (ServerSocket == INVALID_SOCKET) || (ClientSocket == INVALID_SOCKET) );
    
    if ( is_ErrorBinding || exists_InvalidSocket ) printf("Something went wrong: %d...", WSAGetLastError());

}

void handle_client(const char* expected_command, const void* task) {
    memset(&server_addr, 0, sizeof(server_addr));
    ThreadData *data = (ThreadData*)lparam;
   
    while (1) {
      WaitForSingleObject(data->event, INFINITE);
      SOCKET socket = data->clientSocket;
      
      char buffer[BUFFER_SIZE];
      recv_data = recv(socket, buffer, BUFFER_SIZE, 0);
      
  }
}

void create_thread_connections(ThreadData& threadData){
  for (int i = 0; i < THREAD_POOL_SIZE; i++){
      if (WaitForSingleObject(threadData[i].event, 0) == WAIT_FOR_OBJECT_0) {
        threadData[i].clientSocket = ClientSocket;
        SetEvent(threadData[i].event);
        break;
      }
  }
}

void create_threads(){
  HANDLE* threadHandles = (HANDLE*)malloc(sizeof(HANDLE) * THREAD_POOL_SIZE);
  ThreadData* threadData = (ThreadData*)malloc(sizeof(ThreadData) * THREAD_POOL_SIZE);

  if ( (threadHandles == NULL) || (threadData == NULL) ) printf("memory allocation failed");

  for (int i = 0; i < THREAD_POOL_SIZE; i++) {
      threadData[i].event = CreateEvent(NULL, TRUE, FALSE, NULL);
      threadHandles[i] = CreateThread(NULL, 0, handle_client, &threadData[i], 0, NULL);
  }

  while (1){
      ClientSocket = accept(ServerSocket, (struct sockaddr *)&server_addr, &addrlen);
      if (ClientSocket == INVALID_SOCKET) {
        printf("There was some error with the client... %d", WSAGetLastError());
        continue;
      }
      create_thread_connections(threadData);
  }

  for (int i = 0; i < THREAD_POOL_SIZE; i++) {
      CloseHandle(threadHandles[i]);
      CloseHandle(threadData[i].event);
  }

  free(threadHandles);
  free(ThreadData);

  closesocket(ServerSocket);
}

#elif defined(__linux__) || defined(__unix__)

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

SOCKET_FAM socket_F = AF_INET;
SOCKET_TYPE socket_T = SOCK_STREAM;
SOCKET_PROTO socket_P = 6; // TCP PROTOCOL

int ServerSocket, ClientSocket;
struct sockaddr_in server_addr;

size_t addrlen = sizeof(server_addr);

void create_server() {}

void handle_client() {}

#endif
