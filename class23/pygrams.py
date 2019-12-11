#!/usr/bin/python3
import sys


with open("book.txt", "r") as file:
    hashtable = {};
    print("Hashtable started with " + str(sys.getsizeof(hashtable)) + " bytes at length 0")
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
                    print("Hashtable grew to " + str(old_ht_size) + " bytes at length " + str(len(hashtable)))
            last_word = word

key_found = False
for key in hashtable:
    if hashtable[key] >= 200:
        key_found = True
        print("Bigram " + key + " has count of " + str(hashtable[key]))

if not key_found:
    for key in hashtable:
        print("Bigram " + key + " has count of " + str(hashtable[key]))

print("Total of " + str(len(hashtable)) + " different bigrams recorded")
