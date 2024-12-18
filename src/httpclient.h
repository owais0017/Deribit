#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <string>

// Function to perform a basic HTTP POST request
std::string httpPostRequest(const std::string& url, const std::string& authHeader, const std::string& jsonData);

#endif