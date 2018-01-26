#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int debugID = -1;

typedef struct {
	char * string;
	unsigned int capacity;
} STRING;

STRING * newString(int capacity)
{
	STRING * rv;
	rv = calloc(1,sizeof(STRING));
	if (capacity > 0) {
		rv -> string = calloc( (capacity + 1),sizeof(char));
		rv -> capacity = capacity;
	}
	return rv;
}
int stringLen(STRING * str)
{
	if (str == NULL || str -> string == NULL)
		return 0;
	str -> string[str -> capacity] = '\0';
	return strlen(str -> string);
}

int expandCapacity(STRING * str, int newCapacity)
{
	if (str == NULL || newCapacity <= 0)
		return -1;
	if (debugID >= 0)
		fprintf(stderr, "(ID %d) expanding capacity of %x (%d) to %d\n", debugID, str,str->capacity,newCapacity);
	str -> string = realloc(str -> string, newCapacity + 1);
	str -> string [str -> capacity] = '\0';
	str -> capacity = newCapacity;
	return newCapacity;
}

int stringCat(STRING * str, STRING *add)
{
	int tot;
	if (str == NULL || add == NULL)
		return 0;
	if ( (tot = (stringLen(str)+stringLen(add)) )> str->capacity)
		expandCapacity(str, tot); 
	strncat(str -> string, add -> string, str -> capacity);
	return strlen(str->string);
}

int stringDebugID(int id)
{
	debugID = id;
	return id;
}

int stringFree(STRING * str)
{
	if (str == NULL) return 0;
	if (str -> string != NULL) free (str -> string);
	if (debugID >= 0)
		fprintf(stderr, "(ID %d) free %x\n",debugID,str);
	free(str);
	return 0;
}

