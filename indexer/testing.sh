#!/bin/sh
# Nikhil Pande
# COSC 50, 23S
# Tiny Search Engine
#
# testing.sh - testing file for indexer
#

make indexer

rm -rf index
mkdir index

PAGEDIR=../../shared/tse/crawldata



# test cases

./indexer

./indexer oneargument

./indexer onearg twoarg threearg four

./indexer invalid_pagedir index/letters-3.index

./indexer index index/letters-3.index # index is not a crawler directory

./indexer $PAGEDIR/letters-3 index_fake/invalid_filename

./indexer $PAGEDIR/letters-3 index

# create a read only file
touch read_only
chmod ugo=rx read_only
./indexer $PAGEDIR/letters-3 read_only
rm -f read_only

# usages

# wikipedia_2
./indexer $PAGEDIR/wikipedia_2 index/wikipedia-2.index
# use shared indexcmp function to check if there are any differences with shared index
../../shared/tse/indexcmp ../../shared/tse/indices/wikipedia_2.ndx index/wikipedia-2.index
# use indextest to validate directory
./indextest index/wikipedia-2.index index/wikipedia-2-test.index
# use indexcmp to ensure equivalence
../../shared/tse/indexcmp index/wikipedia-2.index index/wikipedia-2-test.index


# toscrape-1
./indexer $PAGEDIR/toscrape-1 index/toscrape-1.index
# indexcmp
../../shared/tse/indexcmp ../../shared/tse/indices/toscrape-1.ndx index/toscrape-1.index
# indextest
./indextest index/toscrape-1.index index/toscrape-1-test.index
# indexcmp
../../shared/tse/indexcmp index/toscrape-1.index index/toscrape-1-test.index


# indexer valgrind
valgrind --leak-check=full --show-leak-kinds=all ./indexer $PAGEDIR/letters-3 index/letters-3.index

# indextest
valgrind --leak-check=full --show-leak-kinds=all ./indextest index/letters-3.index index/letters-3-test.index

