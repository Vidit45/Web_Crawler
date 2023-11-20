# Principles of Programming Languages - Group 23 - Web Crawlers in C++ vs Rust

## Milestone 1:

- Wrote our own implementations of basic web crawlers in C++ and Rust
	- They are given a target web address
	- They are given a maximum depth
	- These values are hardcoded initially for testing purposes
	- The crawler goes to each URL, hets the html, identifies other linked URLs on that webpage and then goes to those URLs recursively. 
- Ensured that all code being written is easily readable and highly maintainable in the future
- Basic implementations in C++ and Rust
- Created a plan for performance testing and benchmarking
- Implemented basic async funcitonality in Rust
- Created a plan for testing error handling
- Explored the use of VTune and Perf for profiling
- FlameGraph can be use to create interactive flame graphs from Perf data


## Milestone 2:

### Implementation changes

- Use the DOM to identify anchor tags with href attributes specifically instead of using regex matching
- Implement multithreading

### Performance testing and benchmarking:

- Average out over several tests
- Create a local dummy stub to return dummy values for testing purposes
- Create test cases using the Nighlty crate for Rust

### Comparison of concurrent processing in C++ and Rust:

- Thread creation
- Thread safety
- Thread spooling

### Reliability testing:

- Will have some testcases that are meant to fail
- Will test with malicious/abnormal links and crawler traps
- Will check for memory leaks and CPU usage
