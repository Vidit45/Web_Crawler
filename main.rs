extern crate reqwest;
// use select::document::Document;
// use select::predicate::Name;
use std::error::Error;
use async_recursion::async_recursion;
use regex:: Regex;
async fn get_html(url: &str) -> Result<String, Box<dyn Error>> {
    let res = reqwest::get(url).await?.text().await?;
    Ok(res)
}

fn extract_urls(html: &str, url: &str) -> Vec<String> {
    //let document = Document::from(html);
    let mut urls = Vec::new();
    let mut base_url: String="".to_string();
    let re = Regex::new(r#"<a href="(.*?)""#).unwrap();
    let re1 = Regex::new(r#"(^https?://[^/]+)"#).unwrap();

    if let Some(capture) = re1.captures(url) {
         if let Some(c) = capture.get(1) {
            base_url = c.as_str().to_string();
         }
    }

    for captures in re.captures_iter(html)
    {
        if let Some(capture) = captures.get(1)
        {
            let mut url_str = capture.as_str().to_string();
            println!("url_str={}", url_str);
            println!("base_url={}", base_url);
            if url_str.starts_with("/")
            {
                url_str = format!("{}{}", base_url, url_str);
            }
            urls.push(url_str);
        }
    }
    // to optimise further we can use the DOM to select all the anchor nodes with an href 
    urls
}

#[async_recursion]
async fn crawl(url: &str, depth: i32) -> Result<(), Box<dyn Error>> {
    if depth <= 0 {
        return Ok(());
    }

    let html = get_html(url).await?;
    println!("extracting URLs");
    let urls = extract_urls(&html, url);
    
    for u in urls {
        count_urls = count_urls + 1;
        println!("{} Found URL: {}",count_urls, u);
        crawl(&u, depth - 1).await?;
    }
    Ok(())
}

#[tokio::main]
async fn main() -> Result<(), Box<dyn Error>> {
    let starting_url = "https://google.com"; // Replace with the starting URL
    let max_depth = 2; // Define the maximum depth for traversal

    crawl(starting_url, max_depth).await?;

    Ok(())
}

