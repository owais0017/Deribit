 #include "auth.h"
 #include <chrono>
 #include <ctime>
 #include <sstream>
 #include <iomanip>
 #include <openssl/sha.h>
  #include <openssl/evp.h>
 #include <openssl/hmac.h>
 #include <iostream>
 #include <algorithm>
 #include <stdlib.h>

 std::string generateDeribitAuthHeader(const std::string& apiKey, const std::string& apiSecret) {
     // Get current timestamp in milliseconds
     auto now = std::chrono::system_clock::now();
     auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
      std::stringstream ss;
      ss << now_ms;
     std::string timestamp = ss.str();

     std::string prehash = timestamp + "\n" + apiKey;
     unsigned char hash[SHA512_DIGEST_LENGTH];
     SHA512(reinterpret_cast<const unsigned char*>(prehash.c_str()), prehash.size(), hash);

     std::string hash_string(reinterpret_cast<char const*>(hash), SHA512_DIGEST_LENGTH);

     // Create HMAC SHA512 signature
      unsigned char *hmac_sha512_result = (unsigned char*)malloc(EVP_MAX_MD_SIZE);
     unsigned int hmac_sha512_len = 0;
    
     HMAC(EVP_sha512(),
      apiSecret.c_str(), apiSecret.length(),
      hash, SHA512_DIGEST_LENGTH,
      hmac_sha512_result, &hmac_sha512_len);

     std::string signature(reinterpret_cast<char const*>(hmac_sha512_result), hmac_sha512_len);

       // Convert signature to hex string
       std::stringstream hex_ss;
       hex_ss << std::hex << std::setfill('0');
      for (unsigned char c : signature) {
           hex_ss << std::setw(2) << static_cast<int>(c);
      }
    
       
    free(hmac_sha512_result);


     // Build the authentication header
     std::string authHeader = "deribit-v2 " + apiKey + ":" + hex_ss.str();
     return authHeader;
 }
