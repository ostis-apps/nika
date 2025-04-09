#include "common/http/sc_http.hpp"

#include <curl/curl.h>

bool gIsInitialised = false;

void ScHttp::Init()
{
  if (!gIsInitialised)
  {
    curl_global_init(CURL_GLOBAL_ALL);
    gIsInitialised = true;
  }
}

void ScHttp::Shutdown()
{
  if (gIsInitialised)
  {
    curl_global_cleanup();
    gIsInitialised = false;
  }
}
