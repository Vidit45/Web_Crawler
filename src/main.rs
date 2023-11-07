 extern crate reqwest;
use std::error::Error;
use async_recursion::async_recursion;
use regex::Regex;

// Function to retrieve HTML content from a given URL
async fn get_html(url: &str) -> Result<String, Box<dyn Error>> {
    let res = reqwest::get(url).await?.text().await?;
    Ok(res)
}

// Function to extract URLs from the HTML content of a webpage
fn extract_urls(html: &str, url: &str) -> Vec<String> {
    let mut urls = Vec::new();
    let mut base_url: String = "".to_string();
    let re = Regex::new(r#"<a href="(.*?)""#).unwrap();
    let re1 = Regex::new(r#"(^https?://[^/]+)"#).unwrap();

    // Extract the base URL from the provided URL
    if let Some(capture) = re1.captures(url) {
        if let Some(c) = capture.get(1) {
            base_url = c.as_str().to_string();
        }
    }

    // Iterate through all matched URL patterns in the HTML content
    for captures in re.captures_iter(html) {
        if let Some(capture) = captures.get(1) {
            let mut url_str = capture.as_str().to_string();
            // Append the base URL to relative URLs
            if url_str.starts_with("/") {
                url_str = format!("{}{}", base_url, url_str);
            }
            urls.push(url_str);
        }
    }
    // to optimize further, we can use the DOM to select all the anchor nodes with an href
    urls
}

// Recursive function to crawl through URLs up to a certain depth
#[async_recursion]
async fn crawl(url: &str, depth: i32) -> Result<(), Box<dyn Error>> {
    if depth <= 0 {
        return Ok(());
    }

    // Fetch HTML content from the current URL
    let html = get_html(url).await?;
    println!("extracting URLs");
    // Extract URLs from the HTML content
    let urls = extract_urls(&html, url);

    // Iterate through the extracted URLs and recursively crawl through them
    for u in urls {
        println!("Found URL: {}", u);
        crawl(&u, depth - 1).await?;
    }
    Ok(())
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let starting_url = "https://google.com"; // Replace with the starting URL
    let max_depth = 1; // Define the maximum depth for traversal

    // Initiate the crawling process with the specified parameters
    crawl(starting_url, max_depth).await?;

    Ok(())
}
