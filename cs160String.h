/* File: cs160String.h
 * simple String with capacity structure
 */
typedef struct {
	char * string;
	unsigned int capacity;
} STRING;

extern STRING * newString(int capacity);
extern int stringLen(STRING * str);
extern int expandCapacity(STRING * str, int newCapacity);
extern int stringCat(STRING * str, STRING *add);
extern int stringDebugID(int id);
extern int stringFree(STRING * str);

