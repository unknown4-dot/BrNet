#include "brnet.h"

#include <netdb.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

void getip(char *domain)
{
    struct hostent *host;
    host = gethostbyname(domain);
    if (host == NULL)
    {
        printf("IP: NULL\n");
        return;
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
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

        res = curl_easy_perform(curl);

        if (res == CURLE_OK && response)
        {
            printf("%s\n", response);
        }
        else
        {
            fprintf(stderr, "Failed to fetch %s: %s\n", url, curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
        free(response);
    }

    curl_global_cleanup();
}
