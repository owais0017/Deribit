#ifndef AUTH_H
#define AUTH_H

#include <string>

// Function to generate the authentication header
std::string generateDeribitAuthHeader(const std::string& apiKey, const std::string& apiSecret);

#endif