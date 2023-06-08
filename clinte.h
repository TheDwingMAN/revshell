#pragma once
#include<string>



void sendPostRequest(const std::string& server, int port, const std::string& endpoint, const std::string& data, const std::string& contentType);

std::string sendGetRequest(const std::string& server, int port, const std::string& endpoint);

void sendMessage(const std::string& message);

std::string receiveMessages();

void downloadFile(const std::string& server, int port, const std::string& endpoint, const std::string& localFilePath);







