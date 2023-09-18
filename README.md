# CS50 TSE
## Nikhil Pande (NPande25)
### May 2023

This repository contains my Tiny Search Engine, a project implemented in C that contains three unique subsystems:
* `crawler`, which takes a root URL and recursively crawls through the linked webpages
* `indexer`, which takes a crawler-produced directory and indexes every word
* `querier`, which, using the directories and indexes, takes an inputted query and returns the most relevant webpages, ranked according to relevance.

All specific functionality and limitations of this design are well-documented in `README`, `IMPLEMENTATION`, and `DESIGN` markdown files.

To compile, run `make`. To clean the subdirectories and the object files, run `make clean`.
