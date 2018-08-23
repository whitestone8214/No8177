/*
	Copyright (C) 2018 Minho Jo <whitestone8214@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>


#define VERSION_NO8177 7


typedef unsigned char byte;
typedef struct element element;
typedef struct pair pair;

struct element {element *previous; void *data; element *next;};
struct pair {pair *previous; void *a; void *b; pair *next;};


/* All returned values with pointer are dynamically allocated. Do not forget to unallocate it if no longer used! */

/* 'element' functions. */
element *no8177_element_new(element *previous, void *data, element *next); /* previous, data, next may be NULL */
element *no8177_element_first(element *_element);
element *no8177_element_last(element *_element);
int no8177_element_length(element *_element);
element *no8177_element_get(element *_element, int nth); /* Returns the element itself; -1 = The last element on a row */
byte *no8177_element_row_to_string(element *_element, byte appendNullAtLast); /* Connects entire row into one string */
byte no8177_element_insert(element *_element, int nth, void *data);
void no8177_element_drop(element *_element, int nth); /* Unallocates and removes one element in a row */
void no8177_element_dispose(element *_element); /* It also unallocates all elements that are connected with given one */

/* String functions. */
byte no8177_string_starts_with(byte *string, byte *startsWith);
byte no8177_string_ends_with(byte *string, byte *endsWith);
byte *no8177_string_pick(byte *string, int from, int to);
byte *no8177_string_connect(int amount, ...);
element *no8177_string_split(byte *string, byte *by, byte retainBy); /* All 'data' inside each elements except the last one is byte *; The last element's 'data' is NULL; If 'retainBy' is 1 it will also put in 'by' as element */
byte *no8177_string_replace(byte *string, byte *from, byte *to);

/* File functions. */
byte no8177_file_exists(byte *address); /* 1 = Exists, 0 = Not exist */
byte no8177_file_can_read(byte *address); /* 1 = Exists, 0 = Not exist */
byte no8177_file_can_write(byte *address); /* 1 = Exists, 0 = Not exist */
byte no8177_file_can_execute(byte *address); /* 1 = Exists, 0 = Not exist */
byte no8177_file_is_regular_file(byte *address, byte linkItself); /* 1 = Yes, 0 = No */
byte no8177_file_detail_type(byte *address); /* 0 = Unknown, 2 = ELF, 3 = PNG, 4 = JPEG, 5 = WebP, 6 = BMP, 7 = GIF, 8 = Matroska, 9 = Ogg, 10 = MP4, 11 = MOV, 12 = FLV */
byte no8177_file_is_directory(byte *address, byte linkItself); /* 1 = Yes, 0 = No */
byte no8177_file_is_symbolic_link(byte *address, byte linkItself); /* 1 = Yes, 0 = No */
int no8177_file_size(byte *address, byte linkItself); /* -1 = No such file */
byte *no8177_file_here();
byte *no8177_file_home();
byte *no8177_file_parent(byte *address);
byte *no8177_file_clean_address(byte *address); /* Cleans the obfuscated file address */
byte *no8177_file_name(byte *address);
element *no8177_file_children(byte *address); /* '.' and '..' is excluded; The last element's 'data' is NULL */
byte no8177_file_create(byte *address, byte type); /* 'type': 0 = File, 1 = Directory; Return value: 1 = Success, 0 = Failed */
byte *no8177_file_load(byte *address, byte *result); /* 'result': 1 = Success, 0 = Failed. Will try to write on result[0]. May be NULL */
byte no8177_file_save(byte *address, byte *data, byte overwrite); /* 'overwrite': 1 = Yes, 0 = No; Return value: 1 = Success, 0 = Failed */
byte no8177_file_delete(byte *file); /* 1 = Success, 0 = Failed */
byte no8177_file_copy(byte *from, byte *to, byte overwrite); /* 1 = Success, 0 = Failed; If 'overwrite' is 1 then it will remove existing file EVEN IF THE FILE IS DIRECTORY */
byte no8177_file_move(byte *from, byte *to, byte overwrite); /* 1 = Success, 0 = Failed; If 'overwrite' is 1 then it will remove existing file EVEN IF THE FILE IS DIRECTORY */

/* Key-value pair database functions. */
pair *no8177_database_new(pair *previous, void *a, void *b, pair *next);
pair *no8177_database_first(pair *_pair);
pair *no8177_database_last(pair *_pair);
int no8177_database_length(pair *_pair);
pair *no8177_database_get(pair *_pair, int nth);
void no8177_database_drop(pair *_pair, int nth);
void no8177_database_dispose(pair *_pair);
pair *no8177_database_from_string(byte *string); /* All 'a' and 'b' inside each pairs except the last one is byte *; The last pair's 'a' and 'b' is NULL */
byte *no8177_database_to_string(pair *database);
byte *no8177_database_get_value(pair *database, byte *key);
byte no8177_database_set_value(pair *database, byte *key, byte *value);
