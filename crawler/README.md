# CS50 TSE Crawler
## Nikhil Pande: Lab 4

The `crawler` subsystem of the tiny search engine utilizes takes a root URL, its starting point, and recursively crawls that page and all linked pages up to a certain depth, documenting the pages in a directory. It is implemented primarily in the `crawler.c` file, in which it uses a `bag` to track all the pages it finds and must scan, as well as a `hashtable` to track those that it has already visited.

### Usage
The `crawler.c` file implements this program, using the following functions:
```c
static void parseArgs(const int argc, char* argv[], char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);
static void logr(const char *word, const int depth, const char *url);
```
`parseArgs` validates and parses the arguments into usable format

`crawl` does the actual work of crawling the webpages from the source URL, adding each new URL to a bag and extracting from the bag up to a certain `maxDepth`

`pageScan` scans each webpage that is extracted from the bag, adding the HTML to the webpage and identifying internal linked URLs

`logr` was an optional function that logs the progress of the crawler, printing actions at each relevant step.

### Files
* `Makefile` - to compile
* `crawler.c` - implements crawler
* `README.md` - to document
* `testing.sh` - to test crawler subsystem
* `testing.out` - output of `testing.sh`

### Compilation
To compile, call `make`. To run `testing.sh`, do `bash -v ./testing.sh &> testing.out`. To run a valgrind analysis, do `make valgrind`. To run the crawler, use this structure in the command line: `./crawler seedURL pageDirectory maxDepth`

### Assumptions
No extra assumptions beyond those documented in the specs

### Testing
Since implementing the full set of tests outlined in the implementation spec would take too long to run and produce a lengthy testing.out file, I used a smaller set of tests for Lab 4 to test my crawler subsystem. I tested using the three TSE playgrounds, `letters`, `toscrape`, and `wikipedia`. With these, I ran the following tests:
* I first tested crawler with faulty inputs, such as invalid number of arguments, null arguments, etc
* `letters` with depths 0, 3, and 10
* `toscrape` with depths 0 and 1
* `wikipedia` with depths 0 and 1
* `valgrind` run using the `letters` playground at depth 2
Some of these tests have corresponding output files in the `shared/tse/crawldata` folder. For these tests, I used `diff -r` to confirm that the output files of my crawls were identical to those in the shared folder.

