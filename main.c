#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// bucket structure for chaining and to avoid collisions. Linear probing below
typedef struct Bucket {
    char *key;
    char *value;
    struct Bucket *next;
} Bucket;

typedef struct {
    Bucket **buckets;
    int size;
} HashTable;

Bucket* create_bucket(char *key, char *value) {
    Bucket *new_bucket = (Bucket*)malloc(sizeof(Bucket));

  // we duplicate the key & value for two reasons:
  // 1. we want hash table to have its own copy
  // 2. prevent pointers pointing to invalid locations in the memory
  //
  //
    new_bucket->key = strdup(key);
    new_bucket->value = strdup(value);
    new_bucket->next = NULL;
    return new_bucket;
}

HashTable* create_table(int size) {
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->buckets = (Bucket**)malloc(sizeof(Bucket*) * size);
    for (int i = 0; i < size; i++) {
        table->buckets[i] = NULL;
    }
    return table;
}

int hash(char *key, int size) {
    int hash = 0;

  // TODO:  change to unsigned int
    while (*key) {

    //  use modulo to keep the hash value within the range of the table size
        hash = (hash + *key) % size;
        key++;
    }
    return hash;
}

void insert(HashTable *table, char *key, char *value) {
    int index = hash(key, table->size);
    Bucket *new_bucket = create_bucket(key, value);
    new_bucket->next = table->buckets[index];
    table->buckets[index] = new_bucket;
}

char* retrieve(HashTable *table, char *key) {
    int index = hash(key, table->size);
    Bucket *bucket = table->buckets[index];
    while (bucket) {
        if (strcmp(bucket->key, key) == 0) {
            return bucket->value;
        }
        bucket = bucket->next;
    }
    return NULL;
}

void delete(HashTable *table, char *key) {
    int index = hash(key, table->size);
    Bucket *bucket = table->buckets[index];
    Bucket *prev = NULL;
    while (bucket) {
        if (strcmp(bucket->key, key) == 0) {
            if (prev) {
                prev->next = bucket->next;
            } else {
                table->buckets[index] = bucket->next;
            }
            free(bucket->key);
            free(bucket->value);
            free(bucket);
            return;
        }
        prev = bucket;
        bucket = bucket->next;
    }
}

void free_table(HashTable *table) {
    for (int i = 0; i < table->size; i++) {
        Bucket *bucket = table->buckets[i];
        while (bucket) {
            Bucket *temp = bucket;
            bucket = bucket->next;
            free(temp->key);
            free(temp->value);
            free(temp);
        }
    }
    free(table->buckets);
    free(table);
}

int main() {
    HashTable *table = create_table(10);

  // insertion
    insert(table, "nam", "test");
    insert(table, "age", "10");
    insert(table, "city", "somewhere");


  // retrieval
    printf("name: %s\n", retrieve(table, "name"));
    printf("age: %s\n", retrieve(table, "age"));
    printf("city: %s\n", retrieve(table, "city"));


  // deletion
    delete(table, "age");
    printf("age after deletion: %s\n", retrieve(table, "age"));

    free_table(table);

    

    return 0;
}
