# CS50 TSE Querier
## Design Spec
The TSE Querier is the final leg of the Tiny Search Engine, implemented during CS50 23S. The querier takes in a pageDirectory and an IndexFileName, and based on the passed in search queries it, returns a `counters_t*` struct describing the relevant documents. 


Based on our `query` implementation, we produce the following output when we run a query:
```
Query: boston or marathon and race
Matches 5 document(s) (ranked):
Score:    4 Doc:  112 http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/quench-your-own-thirst-business-lessons-learned-over-a-beer-or-two_629/index.html
Score:    2 Doc:  155 http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/brilliant-beacons-a-history-of-the-american-lighthouse_515/index.html
Score:    2 Doc:  305 http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/the-midnight-watch-a-novel-of-the-titanic-and-the-californian_640/index.html
Score:    2 Doc:  322 http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/the-art-forger_930/index.html
Score:    1 Doc:  552 http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue/forever-and-forever-the-courtship-of-henry-longfellow-and-fanny-appleton_894/index.html
```

We used the following modules to implement our querier:
* the `query_t` module held the structure for each query, inclueding the string of the query, its tokenized form, and its word count.
* the `Document_t*` module carries data for each printed document, which appears in front of the user after each search query.
We used these modules to help implement the querier.

## Functional decomposition into modules
### We anticipate the following modules or functions:

# 1. main, which parses arguments and initializes other modules;
# 2. querier, which builds an in-memory index from webpage files it finds in the pageDirectory. It then calls the necessary functions to return relevant webpages;
# 3. makeDocArray, which creates the array into which our documents are stored.
# 4. And some helper modules that provide data structures:

