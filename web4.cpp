#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <regex>

int count_urls=0;

// Function to perform an HTTP GET request using cURL
size_t writeCallback(char* contents, size_t size, size_t nmemb, std::string* data) {
    data->append(contents, size * nmemb);
    return size * nmemb;
}

std::string getHTML(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

// Function to extract URLs from a webpage
std::vector<std::string> extractUrls(const std::string& html) {
    std::vector<std::string> urls;
    std::regex url_regex("<a href=\"(.*?)\"");
    std::sregex_iterator it(html.begin(), html.end(), url_regex);
    std::sregex_iterator end;
    for (; it != end; ++it) {
        std::smatch match = *it;
        urls.push_back(match.str(1));
    }
    return urls;
}

// Function to crawl through URLs recursively
void crawl(const std::string& url, int depth) {
    if (depth <= 0) {
        return;
    }
    std::string html = getHTML(url);
    std::vector<std::string> urls = extractUrls(html);
    for (const std::string &u : urls) {
        count_urls++;
        std::cout << count_urls << "-" << "Found URL: " << u << std::endl;
        crawl(u, depth - 1);
    }
}

int main() {
    std::string startingURL = "https://google.com"; // Enter the starting URL here
    int maxDepth = 2; // Define the maximum depth for traversal

    crawl(startingURL, maxDepth);

    return 0;
}
