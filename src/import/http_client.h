#pragma once

#include <string>

/**
 * @brief Represents a simple HTTP response used by import sources.
 */
struct HttpResponse
{
	/// Numeric HTTP status code (e.g. 200, 404).
	int status_code = 0;

	/// Raw response body as a string.
	std::string body;
};

/**
 * @brief Abstraction over an HTTP client used by remote import sources.
 *
 * Implementations can use any underlying HTTP library (e.g. libcurl,
 * Boost.Beast, etc.). Tests can provide a fake implementation.
 */
class IHttpClient
{
public:
	virtual ~IHttpClient() = default;

	/**
	 * @brief Perform an HTTP GET request for the given URL.
	 *
	 * @param url Target URL to fetch.
	 * @return HttpResponse containing status code and response body.
	 *
	 * Implementations may throw on fatal transport-level errors,
	 * but they are allowed to return non-2xx status codes as well.
	 */
	virtual HttpResponse get(const std::string &url) = 0;
};
