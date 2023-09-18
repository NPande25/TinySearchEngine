#!/bin/bash
#
# Nikhil Pande
# COSC 50, 23S
# Tiny Search Engine
#
# testing.sh - bash testing script for querier

make querier
rm -rf invalidQuery
rm -rf test1

PAGEDIR=../../shared/tse/output
VALGRIND="valgrind --leak-check=full --show-leak-kinds=all" 

# bad command line inputs

./querier

./querier oneargument

./querier onearg twoarg threearg four

./querier invalid_pagedir index/letters-3.index

./querier ../indexer index/letters-0.index # indexer is obviously not a crawler-produced directory

./querier $PAGEDIR/letters-3 index_fake/invalid_filename

./querier $PAGEDIR/letters-3 index



# bad query inputs

touch invalidQuery

echo "invalid!!" >> invalidQuery

echo "and invalid" >> invalidQuery

echo "invalid and" >> invalidQuery

echo "or invalid and" >> invalidQuery

echo "this seems ok and or not ok" >> invalidQuery

echo "and and and and and" >> invalidQuery

$VALGRIND ./querier $PAGEDIR/toscrape-2 $PAGEDIR/toscrape-2.index < invalidQuery

rm -rf invalidQuery


# real testing

PAGEDIRECTORY=$PAGEDIR/toscrape-2
INDEXFILE=$PAGEDIR/toscrape-2.index

touch test1

echo "soccer and match and family" >> test1

echo "boston or MArathon and race" >> test1

echo "ham and cheese" >> test1

echo "ALGORITHM" >> test1

echo "high and school" >> test1

echo "ketchup OR mustard   or relish" >> test1

echo "HaRvArD" >> test1

echo "priceless object or tattoo and cooking" >> test1

$VALGRIND ./querier $PAGEDIRECTORY $INDEXFILE < test1

rm -rf test1



