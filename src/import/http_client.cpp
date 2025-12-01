#include "import/http_client.h"

#include <curl/curl.h>

#include <stdexcept>
#include <string>

namespace
{
	std::size_t write_to_string_callback(char *ptr, std::size_t size, std::size_t nmemb, void *userdata)
	{
		const std::size_t total = size * nmemb;
		if (userdata == nullptr || ptr == nullptr)
		{
			return 0;
		}

		auto *buffer = static_cast<std::string *>(userdata);
		buffer->append(ptr, total);
		return total;
	}
}

HttpResponse LibcurlHttpClient::get(const std::string &url)
{
	CURL *curl = curl_easy_init();
	if (!curl)
	{
		throw std::runtime_error("Failed to initialize libcurl");
	}

	std::string body;
	long status_code = 0;

	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &body);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

	const CURLcode res = curl_easy_perform(curl);
	if (res != CURLE_OK)
	{
		std::string message = "libcurl request failed: ";
		message += curl_easy_strerror(res);
		curl_easy_cleanup(curl);
		throw std::runtime_error(message);
	}

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

	curl_easy_cleanup(curl);

	HttpResponse response{};
	response.status_code = static_cast<int>(status_code);
	response.body = std::move(body);
	return response;
}
