/* License
   -------
   Copyright (c) 2018 bnzis <https://github.com/bnzis>

   This file is part of the Bulb Interpreter and is released under the terms
   of the MIT/Expat License - see LICENSE. */
#ifndef BULB_HASHMAP_H
#define BULB_HASHMAP_H
#include <stdlib.h>
#include <string.h>
#include "core.h"
#include "xxhash.h"
/* Description
   =========== 
   An hashmap is used in the representation of environments: it contains an array of 
   objects associated to a key; to access the elements without searching them through 
   out  all the array an hashing algorithm is used. Obviously, we have to deal with
   collisions, a "cell" of the hashmap can contain more than one insertions so
   a cell is a list.  
   ```(("name" value) ("name1" value1) ...)```  
    
   bulbXXHash
   ---------- 
   Calculates the hash. */
unsigned long long bulbXXHash(const void* buffer, int length);

/* bulbHashmapAppend
   -----------------
   Adds the object to the hashmap associating it with the key. NOTE: it doesn't 
   check if there's already a stored key. It's also generally faster than 
   set(). */
void bulbHashmapAppend(bulbHashmap *map, char *key, bulbObj *obj);

/* bulbHashmapGet
   --------------
   Returns the object associated to said key, if the object is not available 
   NULL is returned. */
bulbObj *bulbHashmapGet(bulbHashmap *map, char *key);

/* bulbHashmapDelete
   -----------------
   Associates the key to its object inside the hashmap, verifies if there's 
   already a key and if so, it updates it. NOTE: the pointer is updated, but the
   object isn't deallocated. Generally slower than append(), but safer. */
void bulbHashmapSet(bulbHashmap *map, char *key, bulbObj *obj);

/* bulbHashmapDelete
   -----------------
   Deletes the key and the associated object from the hashmap.
   NOTE: the pointers are removed, while key and object aren't touched. */
void bulbHashmapDelete(bulbHashmap *map, char *key);

#endif /* BULB_HASHMAP_H */
