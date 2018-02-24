Calculates the hash.
```c
unsigned long long bulbXXHash(const void* buffer, int length);
```
Adds the object to the hashmap associating it with the key. NOTE: it doesn't 
check if there's already a stored key. It's also generally faster than 
set().
```c
void bulbHashmapAppend(bulbHashmap *map, char *key, bulbObj *obj);
```
Returns the object associated to said key, if the object is not available 
NULL is returned.
```c
bulbObj *bulbHashmapGet(bulbHashmap *map, char *key);
```
Associates the key to its object inside the hashmap, verifies if there's 
already a key and if so, it updates it. NOTE: the pointer is updated, but the
object isn't deallocated. Generally slower than append(), but safer.
```c
void bulbHashmapSet(bulbHashmap *map, char *key, bulbObj *obj);
```
Deletes the key and the associated object from the hashmap.
NOTE: the pointers are removed, while key and object aren't touched.
```c
void bulbHashmapDelete(bulbHashmap *map, char *key);
```
