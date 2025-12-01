#pragma once

#include <string>
#include <unordered_map>

#include "import/http_client.h"

/**
 * @brief Simple in-memory HTTP client used only in unit tests.
 */
class FakeHttpClient : public IHttpClient
{
public:
	/**
	 * @brief Configure the response returned for a given URL.
	 *
	 * @param url       URL that will be requested.
	 * @param response  Response that should be returned from get(url).
	 */
	void set_response(const std::string &url, const HttpResponse &response)
	{
		responses_[url] = response;
	}

	/**
	 * @brief Perform a GET request for the given URL.
	 *
	 * If no explicit response is configured for the URL, this returns a
	 * 404 response with an empty body.
	 */
	HttpResponse get(const std::string &url) override
	{
		const auto it = responses_.find(url);
		if (it != responses_.end())
		{
			return it->second;
		}

		HttpResponse fallback{};
		fallback.status_code = 404;
		fallback.body.clear();
		return fallback;
	}

private:
	std::unordered_map<std::string, HttpResponse> responses_;
};
