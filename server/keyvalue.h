#ifndef __KEYVALUE_H
#define __KEYVALUE_H

struct data {
	char *key;
	char *value;
	struct data *pNext;
};

struct data *pHead;
/* * Returns the value associated with the key,
or NULL * if the key is not in the store. */
char *get(char *key) {
	struct data *iterator;
	for (iterator = pHead; iterator != NULL; iterator = iterator->pNext) {
		if (strcmp(iterator->key, key) == 0) return iterator->value;
	}
	return NULL;
}
/* * Puts the <key,value> pair into the store.
* If the key already existed, its previous value
* is discarded and replaced by the new one. */
void put(char *key, char *value) {
	struct data *iterator, *tmp;
	char *k, *v;
	for (iterator = pHead; iterator != NULL; iterator = iterator->pNext) {
		if (strcmp(iterator->key, key) == 0) {
			strcpy(iterator->value, value);
			return;
		}
		tmp = iterator;
	}
	iterator = (struct data*) malloc(sizeof(struct data));
	k = (char*) malloc(strlen(key) + 1);
	v = (char*) malloc(strlen(value) + 1);
	strcpy(k, key);
	strcpy(v, value);
	k[strlen(key) + 1] = k[strlen(key)] = v[strlen(value) + 1] = v[strlen(value)] = 0;
	iterator->key = k;
	iterator->value = v;
	iterator->pNext = NULL;
	if (pHead == NULL) pHead = iterator;
	else tmp->pNext = iterator;
}

void delKeys() {
	struct data *iterator, *tmp;
	char *k, *v;
	for (iterator = pHead; iterator != NULL; iterator = iterator->pNext) {

	}
	tmp = pHead;
	if (tmp != NULL) {
		do {
			iterator = tmp->pNext;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			tmp = iterator;
		} while (tmp != NULL);
	}
}
#endif
