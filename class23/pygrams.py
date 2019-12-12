#!/usr/bin/python3
import sys

hashtable = {};
print(f"Hashtable started with", sys.getsizeof(hashtable), "bytes at length", len(hashtable))
with open("book.txt", "r") as file:
    last_word = ""
    bigram = ""
    for line in file:
        new_line = ""
        for c in line:
            if c.isalpha():
                new_line += c
            else:
                new_line += " "
        words = new_line.split()
        for word in words:
            if last_word:
                bigram = last_word + " " + word
                if bigram in hashtable:
                    hashtable[bigram] += 1
                else:
                    old_ht_size = sys.getsizeof(hashtable)
                    hashtable[bigram] = 1
                    new_ht_size = sys.getsizeof(hashtable)
                    if new_ht_size > old_ht_size:
                        print("Hashtable grew to", new_ht_size, "bytes at length", len(hashtable))
            last_word = word

key_found = False
for key in hashtable:
    if hashtable[key] >= 200:
        key_found = True
        print(f"Bigram '{key}' has count of {hashtable[key]}")

if not key_found:
    for key in hashtable:
        print(f"Bigram '{key}' has count of {hashtable[key]}")

print(f"Total of {len(hashtable)} different bigrams recorded")
