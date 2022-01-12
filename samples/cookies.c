#include "http.h"
#include "debug.h"


int main(void) {
    struct http_opts opts = {
        .cookies = "key1=value1;key2=value2",
        .timeout_secs = 1,
    };
    struct http_response *resp = http_request("GET", "https://httpbin.org/cookies", &opts);
    response_print(resp);
    http_response_free(resp);
}