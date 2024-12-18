#include <iostream>
#include <string>
#include <fstream>
#include "auth.h"
#include "httpclient.h"

std::string readAPIKeyFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open API key file: " << filename << std::endl;
        return "";
    }
    std::string apiKey;
    std::getline(file, apiKey);
    return apiKey;
}

std::string readAPISecretFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open API secret file: " << filename << std::endl;
        return "";
    }
    std::string apiSecret;
    std::getline(file, apiSecret);
    return apiSecret;
}

int main() {
    std::cout << "Deribit Trader Project Initialized!" << std::endl;

    // Read API key and secret from files
    std::string apiKey = readAPIKeyFromFile("data/api_key.txt");
    std::string apiSecret = readAPISecretFromFile("data/api_secret.txt");

    if (apiKey.empty() || apiSecret.empty()) {
        std::cerr << "Could not read API key or secret. Please check data/api_key.txt and data/api_secret.txt." << std::endl;
        return 1; // Indicate an error
    }

    std::string authHeader = generateDeribitAuthHeader(apiKey, apiSecret);

    std::string url = "https://test.deribit.com/api/v2/public/get_instruments";
    std::string jsonData = "{\"currency\": \"BTC\", \"kind\": \"future\" }";
    std::string response = httpPostRequest(url, authHeader, jsonData);

    if (response != "ERROR") {
        std::cout << "Response:" << std::endl;
        std::cout << response << std::endl;
    } else {
        std::cerr << "Error contacting Deribit API." << std::endl;
        return 1;
    }
    return 0;
}