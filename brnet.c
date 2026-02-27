#include "brnet.h"

#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

void getip(char *domain)
{
    struct hostent *host;
    host = gethostbyname(domain);
    if (host == NULL)
    {
        printf("ip: NULL");
    }
    printf("IP: %s\n", inet_ntoa(*(struct in_addr *)host->h_addr));
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total_size = size * nmemb;
    char **response_ptr = (char **)userp;
    size_t current_len = *response_ptr ? strlen(*response_ptr) : 0;

    char *temp = realloc(*response_ptr, current_len + total_size + 1);
    if (!temp)
        return 0;

    *response_ptr = temp;
    memcpy(*response_ptr + current_len, contents, total_size);
    (*response_ptr)[current_len + total_size] = '\0';

    return total_size;
}
void src(char *url)
{
    CURL *curl;
    CURLcode res;
    char *response = NULL;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);

        if (response)
        {
            printf("%s\n", response);
            free(response);
        }

        curl_easy_cleanup(curl);
    }
}

void status(char *url)
{
  CURL *curl = curl_easy_init();
  if(curl) {
    CURLcode result;
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    result = curl_easy_perform(curl);
    if(result == CURLE_OK) {
      long response_code;
      curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
      printf("status: %ld\n", response_code);
    }

    curl_easy_cleanup(curl);
   }
}

void localhost()
{
  system("python -m http.server 8080");
}
