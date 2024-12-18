#include "httpclient.h"
#include <iostream>
#include <curl/curl.h>
#include <sstream>
#include <stdexcept>
#include <string>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

std::string httpPostRequest(const std::string& url, const std::string& authHeader, const std::string& jsonData) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Could not initialize curl" << std::endl;
        curl_global_cleanup();
        return "ERROR";
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Headers
    struct curl_slist* headers = nullptr;
     if(url.find("public") == std::string::npos) {
      headers = curl_slist_append(headers, "Content-Type: application/json");
      if (!headers) {
       std::cerr << "Could not append Content-Type header" << std::endl;
       curl_easy_cleanup(curl);
       curl_global_cleanup();
      return "ERROR";
      }
      headers = curl_slist_append(headers, ("Authorization: " + authHeader).c_str());
       if (!headers) {
          std::cerr << "Could not append Authorization header" << std::endl;
           curl_easy_cleanup(curl);
          curl_global_cleanup();
        return "ERROR";
       }
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
     }
     else {
        headers = curl_slist_append(headers, "Content-Type: application/json");
          if (!headers) {
            std::cerr << "Could not append Content-Type header" << std::endl;
             curl_easy_cleanup(curl);
            curl_global_cleanup();
            return "ERROR";
             }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

     }

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

    // Set a timeout
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return "ERROR";
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return response;
}