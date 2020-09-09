#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "http.h"

#define WRITE_BUFFER_INIT   ((struct http_write_buffer){0})

static size_t write_callback(void *contents, size_t size, size_t count, void *_buffer) {
    struct http_write_buffer *buffer = (struct http_write_buffer *)_buffer;
    size_t new_size = size * count;
    char *new_memory = realloc(buffer->data, buffer->size + new_size + 1);
    if(new_memory == NULL)
        return 0;
    buffer->data = new_memory;
    memcpy(&buffer->data[buffer->size], contents, new_size);
    buffer->size += new_size;
    buffer->data[buffer->size] = '\0';
    return new_size;
}

struct http_response *http_request(const char *method, const char *url, struct http_opts *opts) {
    if(!method)
        return NULL;
    if(!url)
        return NULL;

    struct http_response *result = malloc(sizeof *result);
    CURL *curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method);
    curl_easy_setopt(curl, CURLOPT_URL, url);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &result->headers);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result->content);

    if(opts) {
        /* TODO: set more options */
    }

    result->headers = WRITE_BUFFER_INIT;
    result->content = WRITE_BUFFER_INIT;

    result->curl_code = curl_easy_perform(curl);
    result->curl_error = curl_easy_strerror(result->curl_code);

    curl_easy_cleanup(curl);
    return result;
}

void http_response_free(struct http_response *this) {
    if(this->headers.data)
        free(this->headers.data);
    if(this->content.data)
        free(this->content.data);
    free(this);
}
