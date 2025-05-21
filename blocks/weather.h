#include <curl/curl.h>

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
  memcpy(userdata, ptr, size * nmemb);
  return size * nmemb;
}

void weather(void *p, char *s) {
  CURL *curl = p;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, s);
  CURLcode res = curl_easy_perform(curl);
}

void *weather_init() {
  static char url[128] = "https://wttr.in/Berlin?format=+%c+%t+%f+%w";
  CURL *curl;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  return curl;
}

void weather_close(void *p) {
  CURL *curl = p;
  curl_easy_cleanup(curl);
}
