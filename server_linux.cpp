#include "http_tcpServer_linux.h"
#include "music.h"
#include <winsock2.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <io.h>


Music myMusic;


namespace
{
    void log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

    void exitWithError(const std::string &errorMessage)
    {
        log("ERROR: " + errorMessage);
        exit(1);
    }
}


http::TcpServer::TcpServer(std::string ip_address, int port)
    : m_ip_address(ip_address), m_port(port), m_socket(),
      m_new_socket(), m_incomingMessage(), m_socketAddress(),
      m_socketAddress_len(sizeof(m_socketAddress))
      {
        startServer();
      }

http::TcpServer::~TcpServer()
{
    closeServer();
}

int http::TcpServer::startServer()
{

    if(WSAStartup(MAKEWORD(2,0), &m_wsaData))
    {
        exitWithError("WSAStartup Failed");
    }

    http::TcpServer::m_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(m_socket < 0)
    {
        exitWithError("Cannot create socket");
        return 1;
    }

    http::TcpServer::m_socketAddress.sin_family = AF_INET;
    http::TcpServer::m_socketAddress.sin_port = htons(m_port);
    http::TcpServer::m_socketAddress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

    if(bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0)
    {
        exitWithError("Cannot connect socket to address");
        return 1;
    }

    return 0;

}

void http::TcpServer::closeServer()
{
    closesocket(m_socket);
    closesocket(m_new_socket);
    WSACleanup();
    exit(0);
}

void http::TcpServer::startListen()
{
    if(listen(m_socket,10) < 0)
    {
        exitWithError("Socket listen failed.");
    }
    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: "
       << inet_ntoa(m_socketAddress.sin_addr)
       << " PORT: " << ntohs(m_socketAddress.sin_port)
       << " ***\n\n";
    
    log(ss.str());

    int bytesReceived;
    const int BUFFER_SIZE = 30720;
    char buffer[BUFFER_SIZE] = {0};

    while(true)
    {
        log("====== Waiting for a new connection ======\n\n\n");
        acceptConnection(m_new_socket);

        char buffer[BUFFER_SIZE] = {0};
        bytesReceived = recv(m_new_socket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived < 0)
        {
            exitWithError("Failed to receive bytes from client socket connection");
        }

        std::ostringstream ss;
        ss << "------ Received Request from client ------\n\n";
        log(ss.str());

        myMusic.checkAndUpdate(); //updates every second.
        sendResponse();

        closesocket(m_new_socket);
    }
}

void http::TcpServer::acceptConnection(SOCKET &new_socket)
{
    new_socket = accept(m_socket, (sockaddr *) &m_socketAddress, &m_socketAddress_len);

    if(new_socket < 0)
    {
        std::ostringstream ss;
        ss <<
        "Server failed to accept incoming connection from ADDRESS: " 
        << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: " 
        << ntohs(m_socketAddress.sin_port);
        exitWithError(ss.str());
    }
}

std::string http::TcpServer::buildResponse()
{
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> Carlos Mercado's Server </h1><p> I am currently listening to " + myMusic.getMusicInfo() + " :) </p></body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
        << htmlFile;

    return ss.str();
}


void http::TcpServer::sendResponse()
{
    // Build fresh response each time
    std::string response = buildResponse();
    
    int bytesSent;
    long totalBytesSent = 0;
    while (totalBytesSent < response.size())
    {
        bytesSent = send(m_new_socket, response.c_str(), response.size(), 0);
        if (bytesSent < 0)
        {
            break;
        }
        totalBytesSent += bytesSent;
    }
    if (totalBytesSent == response.size())
    {
        log("------ Server Response sent to client ------\n\n");
    }
    else
    {
        log("Error sending response to client.");
    }
}
/*
*********************************************************************************
*                                                                               *
*                                    MUSIC                                      *
*                                                                               *
*********************************************************************************

*/

void Music::updateMusicInfo()
{
    std::ifstream inputFile("G:\\now_playing\\now_playing.txt");

    if (!inputFile.is_open()) 
    {
        info = "No music playing";  // Don't crash, set default
        return;
    }

    std::getline(inputFile, info);
    if (info.empty()) {
        info = "No music playing";
    }
    inputFile.close();
}

void Music::checkAndUpdate()
{
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastCheck = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastCheckTime);
    
    // Only check every 100ms
    if (timeSinceLastCheck.count() >= 1000) {
        updateMusicInfo();
        lastCheckTime = now;
    }
}



int main()
{


    http::TcpServer server("0.0.0.0", 8080);
    server.startListen();

    return 0;
}