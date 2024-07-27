#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <regex>
using namespace std;

// Function to perform an HTTP GET request using cURL
size_t writeCallback(char* contents, size_t size, size_t nmemb, string* data) {
    // Appending the contents retrieved from the URL to a string
    data->append(contents, size * nmemb);
    return size * nmemb;
}

// Function to retrieve HTML content from a given URL
string getHTML(const string& url) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    // Initialize cURL
    curl = curl_easy_init();
    if (curl) {
        // Set cURL options
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        // Perform the HTTP request
        res = curl_easy_perform(curl);
        // Clean up cURL resources
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

// Function to extract URLs from the HTML content of a webpage
vector<string> extractUrls(const string& html) {
    vector<string> urls;
    regex url_regex("<a href=\"(.*?)\"");
    // Find all matches of the URL pattern in the HTML content
    sregex_iterator it(html.begin(), html.end(), url_regex);
    sregex_iterator end;
    for (; it != end; ++it) {
        smatch match = *it;
        // Add the matched URL to the vector of URLs
        urls.push_back(match.str(1));
    }
    return urls;
}

// Function to crawl through URLs recursively up to a certain depth
void crawl(const string& url, int depth) {
    if (depth <= 0) {
        return;
    }
    // Retrieve HTML content from the current URL
    string html = getHTML(url);
    // Extract URLs from the HTML content
    vector<string> urls = extractUrls(html);
    // Iterate through the extracted URLs
    for (const string &u : urls) {
        // Print the found URL 
        cout << "Found URL: " << u << endl;
        // Recursively crawl through the found URL with reduced depth
        crawl(u, depth - 1);
    }
}

// Main function to initiate the crawling process
int main() {
    // Define the starting URL for the web crawling process
    string startingURL = "https://google.com"; // Enter the starting URL here
    // Define the maximum depth for traversal
    int maxDepth = 1;

    // Initiate the crawling process with the specified parameters
    crawl(startingURL, maxDepth);

    return 0;
}

