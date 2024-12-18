   #include "httpclient.h"
   #include <curl/curl.h>
   #include <iostream>
   #include <string>

  size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
       size_t totalSize = size * nmemb;
       output->append((char*)contents, totalSize);
       return totalSize;
  }

   std::string httpPostRequest(const std::string& url, const std::string& authHeader, const std::string& jsonData) {
       CURL* curl;
       CURLcode res;
       std::string response;

       curl = curl_easy_init();
       if (curl) {
           // Set URL
           curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

          // Set Request type to POST
           curl_easy_setopt(curl, CURLOPT_POST, 1L);

           //set post data
           curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

           // Set Authentication Header
           struct curl_slist* headers = nullptr;
           headers = curl_slist_append(headers, ("Authorization: " + authHeader).c_str());
           headers = curl_slist_append(headers, "Content-Type: application/json");
           curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Capture response
           curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
           curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


           // Perform the request
           res = curl_easy_perform(curl);
          
           if (res != CURLE_OK) {
               std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
               response = "ERROR";
           }


            curl_slist_free_all(headers);
           curl_easy_cleanup(curl);
       } else {
           std::cerr << "curl_easy_init() failed" << std::endl;
             response = "ERROR";
       }
       return response;
   }