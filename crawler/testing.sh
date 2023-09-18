#!/bin/sh
#
# testing.sh - testing file for the crawler subsystem
#
# Nikhil Pande
# COSC 50, 23S
# Professor Palmer
#
# Testing file for the crawler subsystem.
# Testing is documented more thoroughly in the README.md file

rm -rf data
mkdir data
mkdir data/testing
make crawler

# main URL
URL=http://cs50tse.cs.dartmouth.edu/tse/letters/index.html

# directory with answers to compare with
COMPARE=../../shared/tse/crawldata

# try crawler with four args
./crawler $URL data/testing 3 fourth_arg

# Null URL
./crawler NULL data/testing 4

# external URL 
./crawler https://amazon.com/ data/testing/ 2

# invalid directory
./crawler $URL not_directory 2

# out of range maxDepth
./crawler $URL data/testing -3

./crawler $URL data/testing 100

# maxDepth not integer
./crawler $URL data/testing not_int

# everything NULL
./crawler NULL NULL NULL

# testing on CS50 website letters (depths 0, 1, 10)
mkdir data/letters-0
mkdir data/letters-3
mkdir data/letters-10

# depth 0 -- also check file to ensure URL \n depth \n HTML format
./crawler $URL data/letters-0 0
cat data/letters-0/1

# depth 3
./crawler $URL data/letters-3 3

# depth 10
./crawler $URL data/letters-10 10

# will print nothing if no difference
diff -r $COMPARE/letters-0 data/letters-0
diff -r $COMPARE/letters-3 data/letters-3

# # testing on toscrape
mkdir data/toscrape-0
mkdir data/toscrape-1
URL=http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html

# depth 0
./crawler $URL data/toscrape-0 0

# depth 1
./crawler $URL data/toscrape-1 1

# see if any difference
diff -r $COMPARE/toscrape-0 data/toscrape-0
diff -r $COMPARE/toscrape-1 data/toscrape-1

# testing on wikipedia
mkdir data/wikipedia-0
mkdir data/wikipedia-1
URL=http://cs50tse.cs.dartmouth.edu/tse/wikipedia
# ^note: the cs50 output in the shared folder has no '/' at the end of the URL, as there should be. But for the purposes
# of comparison, I left it out as well so the two outputs would be the same

# depth 0
./crawler $URL data/wikipedia-0 0

# depth 1
./crawler $URL data/wikipedia-1 1

# see if any dfference with shared output
diff -r $COMPARE/wikipedia_1 data/wikipedia-0

#VALGRIND
# prints valgrind output for letters with depth 2
make valgrind


exit 0

