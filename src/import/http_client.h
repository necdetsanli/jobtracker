#pragma once

#include <string>

/**
 * @brief Simple HTTP response representation.
 */
struct HttpResponse
{
	/// Numeric HTTP status code (e.g. 200, 404).
	int status_code = 0;

	/// Raw response body as a string.
	std::string body;
};

/**
 * @brief Abstract HTTP client interface for performing requests.
 */
class IHttpClient
{
public:
	virtual ~IHttpClient() = default;

	/**
	 * @brief Perform a blocking HTTP GET request.
	 *
	 * @param url Target URL.
	 * @return HttpResponse with status code and body.
	 */
	virtual HttpResponse get(const std::string &url) = 0;
};

/**
 * @brief HTTP client implementation based on libcurl.
 *
 * This implementation uses libcurl's easy interface to perform
 * blocking HTTP GET requests.
 */
class LibcurlHttpClient : public IHttpClient
{
public:
	/**
	 * @brief Perform a GET request using libcurl.
	 *
	 * @param url Target URL.
	 * @return HttpResponse with status code and body.
	 *
	 * @throws std::runtime_error if libcurl fails to initialize or execute the request.
	 */
	HttpResponse get(const std::string &url) override;
};
