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


#include <no8177.h>


element *no8177_element_new(element *previous, void *data, element *next) {
	element *_element = (element *)malloc(sizeof(element)); if (_element == NULL) return NULL;
	_element->previous = previous; if (previous != NULL) previous->next = _element;
	_element->data = data;
	_element->next = next; if (next != NULL) next->previous = _element;
	
	return _element;
}
element *no8177_element_first(element *_element) {
	if (_element == NULL) return NULL;
	
	element *_element0 = _element;
	while (_element0->previous != NULL) _element0 = _element0->previous;
	
	return _element0;
}
element *no8177_element_last(element *_element) {
	if (_element == NULL) return NULL;
	
	element *_element0 = _element;
	while (_element0->next != NULL) _element0 = _element0->next;
	
	return _element0;
}
int no8177_element_length(element *_element) {
	element *_element0 = no8177_element_first(_element);
	if (_element0 == NULL) return 0;
	
	int _nElements = 1;
	while (_element0->next != NULL) {_element0 = _element0->next; _nElements++;}
	
	return _nElements;
}
element *no8177_element_get(element *_element, int nth) {
	int _length = no8177_element_length(_element);
	if (_length == 0) return NULL;
	int _nth = nth; if (_nth == -1) _nth = _length - 1;
	if ((_nth < 0) || (_nth >= _length)) return NULL;
	
	element *_element0 = no8177_element_first(_element);
	if (_nth == 0) return _element0;
	for (_nth = 0; _nth < nth; _nth++) _element0 = _element0->next; return _element0; 
}
byte *no8177_element_row_to_string(element *_element, byte appendNullAtLast) {
	int _nElements = no8177_element_length(_element);
	int _length = 0;
	
	element *_element0 = no8177_element_first(_element);
	while (_element0 != NULL) {
		if (_element0->data != NULL) _length += strlen((byte *)_element0->data);
		_element0 = _element0->next;
	}
	
	byte *_string = (byte *)malloc((appendNullAtLast == 1) ? (_length + 1) : _length);
	if (_string == NULL) return NULL;
	
	int _nthString = 0; _element0 = no8177_element_first(_element);
	while (_element0 != NULL) {
		if (_element0->data != NULL) {
			int _nthInElement; for (_nthInElement = 0; _nthInElement < strlen((byte *)_element0->data); _nthInElement++) {
				_string[_nthString] = ((byte *)_element0->data)[_nthInElement];
				_nthString++;
			}
		}
		_element0 = _element0->next;
	}
	if (appendNullAtLast == 1) _string[_nthString] = '\0';
	
	return _string;
}
void no8177_element_drop(element *_element, int nth) {
	element *_element0 = no8177_element_get(_element, nth);
	if (_element0 == NULL) return;
	
	element *_elementLeft = _element0->previous;
	element *_elementRight = _element0->next;
	if (_element0->data != NULL) free(_element0->data);
	free(_element0);
	if (_elementLeft != NULL) _elementLeft->next = _elementRight;
	if (_elementRight != NULL) _elementRight->previous = _elementLeft;
}
void no8177_element_dispose(element *_element) {
	element *_element0 = no8177_element_first(_element);
	if (_element0 == NULL) return;
	
	while (_element0 != NULL) {
		if (_element0->data != NULL) free(_element0->data);
		element *_element1 = _element0->next;
		free(_element0);
		_element0 = _element1;
	}
}

byte no8177_string_starts_with(byte *string, byte *startsWith) {
	if ((string == NULL) || (startsWith == NULL)) return 0;
	if (strlen(string) < strlen(startsWith)) return 0;
	
	int _nth; for (_nth = 0; _nth < strlen(startsWith); _nth++) {if (string[_nth] != startsWith[_nth]) return 0;}
	
	return 1;
}
byte no8177_string_ends_with(byte *string, byte *endsWith) {
	if ((string == NULL) || (endsWith == NULL)) return 0;
	if (strlen(string) < strlen(endsWith)) return 0;
	
	int _nth; for (_nth = 0; _nth < strlen(endsWith); _nth++) {if (string[strlen(string) - 1 - _nth] != endsWith[strlen(endsWith) - 1 - _nth]) return 0;}
	
	return 1;
}
byte *no8177_string_pick(byte *string, int from, int to) {
	if (string == NULL) return NULL;
	if ((from < 0) || (from >= strlen(string))) return NULL;
	if ((to < -1) || (to < from) || (to >= strlen(string))) return NULL;
	
	int _to = to; if (_to == -1) _to = strlen(string) - 1;
	byte *_string = (byte *)malloc((to - from) + 2);
	if (_string == NULL) return NULL;
	int _nth; for (_nth = from; _nth <= to; _nth++) {_string[_nth - from] = string[_nth];}
	_string[_nth] = '\0';
	
	return _string;
}
byte *no8177_string_connect(int amount, ...) {
	/* Prerequisities. */
	int _length = 0; byte *_string;
	va_list _elements; int _nth;
	
	/* Get the total length. */
	va_start(_elements, amount);
		for (_nth = 0; _nth < amount; _nth++) {
			byte *_part = va_arg(_elements, byte *); if (_part == NULL) continue;
			_length += strlen(va_arg(_elements, byte *));
		}
	va_end(_elements);
	
	/* Build the string. */
	va_start(_elements, amount);
		_string = (byte *)malloc(_length + 1); if (_string == NULL) return NULL;
		int _here = 0;
		
		for (_nth = 0; _nth < amount; _nth++) {
			byte *_part = va_arg(_elements, byte *); if (_part == NULL) continue;
			int _nthPart; for (_nthPart = 0; _nthPart < strlen(_part); _nthPart++) {
				_string[_here] = _part[_nthPart];
				_here++;
			} 
		}
		_string[_here] = '\0';
	va_end(_elements);
	
	/* Done. */
	return _string;
}
element *no8177_string_split(byte *string, byte *by, byte retainBy) {
	/* No need to split if no string nor delimiter given. */
	if ((string == NULL) || (by == NULL)) return NULL;
	
	/* Prerequisities. */
	int _nElements = 0;
	element *_element = no8177_element_new(NULL, NULL, NULL);
	
	/* Split the string and put in a element. */
	int _nth; int _nthFrom = 0; int _nthTo = 0;
	for (_nth = 0; _nth < strlen(string); _nth++) {
		/* Skip if the byte does not match with the first byte of 'by'. */
		if (string[_nth] != by[0]) continue;
		
		/* Also skip if the next bytes does not match with the rest of 'by'. */
		int _nthBy; for (_nthBy = 0; _nthBy < strlen(by); _nthBy++) {
			if (string[_nth + _nthBy] != by[_nthBy]) break;
		}
		if (_nthBy < strlen(by)) continue;
		
		/* Append the slice in a element. */
		_nthTo = _nth - 1;
		if (_nthTo >= _nthFrom) {
			byte *_slice = (byte *)malloc((_nthTo - _nthFrom) + 2);
			if (_slice == NULL) {no8177_element_dispose(_element); return NULL;}
			
			int _nth1;
			for (_nth1 = _nthFrom; _nth1 <= _nthTo; _nth1++) _slice[_nth1 - _nthFrom] = string[_nth1];
			_slice[_nth1 - _nthFrom] = '\0';
			
			_element->data = (void *)_slice;
			_element->next = no8177_element_new(_element, NULL, NULL);
			if (_element->next == NULL) {no8177_element_dispose(_element); return NULL;}
			_element = _element->next;
			_nElements++;
		}
		if (retainBy == 1) {
			byte *_slice = (byte *)malloc(strlen(by) + 1);
			if (_slice == NULL) {no8177_element_dispose(_element); return NULL;}
			
			int _nth1;
			for (_nth1 = 0; _nth1 < strlen(by); _nth1++) _slice[_nth1] = by[_nth1];
			_slice[_nth1] = '\0';
			
			_element->data = (void *)_slice;
			_element->next = no8177_element_new(_element, NULL, NULL);
			if (_element->next == NULL) {no8177_element_dispose(_element); return NULL;}
			_element = _element->next;
			_nElements++;
		}
		_nthFrom = _nth + strlen(by);
		_nth += strlen(by) - 1;
	}
	
	/* Append the last slice if it exists. */
	if (_nthFrom < _nth) {
		_nthTo = _nth - 1;
		
		byte *_slice = (byte *)malloc((_nthTo - _nthFrom) + 2);
		if (_slice == NULL) {no8177_element_dispose(_element); return NULL;}
		
		int _nth1;
		for (_nth1 = _nthFrom; _nth1 <= _nthTo; _nth1++) _slice[_nth1 - _nthFrom] = string[_nth1];
		_slice[_nth1 - _nthFrom] = '\0';
		
		_element->data = (void *)_slice;
		_element->next = no8177_element_new(_element, NULL, NULL);
		if (_element->next == NULL) {no8177_element_dispose(_element); return NULL;}
		_element = _element->next;
		_nElements++;
	}
	
	return no8177_element_first(_element);
}
byte *no8177_string_replace(byte *string, byte *from, byte *to) {
	if (from == NULL) return NULL;
	
	element *_splitted = no8177_string_split(string, from, 1);
	if (_splitted == NULL) return NULL;
	
	element *_here = _splitted;
	int _length = 0;
	while (_here != NULL) {
		if (_here->data != NULL) {
			if (strcmp((byte *)_here->data, from) == 0) {
				free(_here->data); _here->data = NULL;
				if (to != NULL) _here->data = (void *)strdup(to);
			}
		}
		if (_here->data != NULL) _length += strlen((byte *)_here->data);
		_here = _here->next;
	}
	
	byte *_string = no8177_element_row_to_string(_splitted, 1);
	no8177_element_dispose(_splitted);
	
	return _string;
}

byte no8177_file_exists(byte *address) {
	if (access(address, F_OK) == 0) return 1;
	else return 0;
}
byte no8177_file_can_read(byte *address) {
	if (access(address, R_OK) == 0) return 1;
	else return 0;
}
byte no8177_file_can_write(byte *address) {
	if (access(address, W_OK) == 0) return 1;
	else return 0;
}
byte no8177_file_can_execute(byte *address) {
	if (access(address, X_OK) == 0) return 1;
	else return 0;
}
byte no8177_file_is_regular_file(byte *address, byte linkItself) {
	struct stat _tree;
	if (linkItself == 1) {if (lstat(address, &_tree) != 0) return 0;}
	else {if (stat(address, &_tree) != 0) return 0;}
	
	return (S_ISREG(_tree.st_mode)) ? 1 : 0;
}
byte no8177_file_is_directory(byte *address, byte linkItself) {
	struct stat _tree;
	if (linkItself == 1) {if (lstat(address, &_tree) != 0) return 0;}
	else {if (stat(address, &_tree) != 0) return 0;}
	
	return (S_ISDIR(_tree.st_mode)) ? 1 : 0;
}
byte no8177_file_is_symbolic_link(byte *address, byte linkItself) {
	struct stat _tree;
	if (linkItself == 1) {if (lstat(address, &_tree) != 0) return 0;}
	else {if (stat(address, &_tree) != 0) return 0;}
	
	return (S_ISLNK(_tree.st_mode)) ? 1 : 0;
}
int no8177_file_size(byte *address, byte linkItself) {
	struct stat _tree;
	
	if (linkItself == 1) {if (lstat(address, &_tree) != 0) return -1;}
	else {if (stat(address, &_tree) != 0) return -1;}
	
	return _tree.st_size;
}
byte *no8177_file_here() {return (byte *)strdup(getenv("PWD"));}
byte *no8177_file_home() {return (byte *)strdup(getenv("HOME"));}
byte *no8177_file_parent(byte *address) {
	if (address == NULL) return NULL;
	
	byte *_address = (byte *)strdup(address);
	if (strcmp(_address, "/") == 0) return _address;
	
	int _nth; for (_nth = strlen(_address) - 1; _nth > 0; _nth--) {if (_address[_nth] == '/') break;}
	if (_nth == 0) {free(_address); return (byte *)strdup("/");}
	else {
		byte *_parent = no8177_string_pick(_address, 0, _nth - 1);
		free(_address);
		
		return _parent;
	}
}
byte *no8177_file_clean_address(byte *address) {
	/* Nothing to do with NULL. */
	if (address == NULL) return NULL;
	
	/* // = / ;) */
	int _nth; for (_nth = 0; _nth < strlen(address); _nth++) {if (address[_nth] != '/') break;}
	if (_nth == strlen(address)) return (byte *)strdup("/");
	
	byte _startsWithSlash = (address[0] == '/') ? 1 : 0;
	element *_splitted = no8177_string_split(address, "/", 0); if (_splitted == NULL) return NULL;
	byte *_result;
	if (_startsWithSlash == 0) {
		if (strcmp((byte *)no8177_element_first(_splitted)->data, "~") == 0) {
			_result = no8177_file_home();
			_splitted = _splitted->next;
			no8177_element_drop(_splitted, 0);
		}
		else _result = no8177_file_here();
	}
	else _result = (byte *)strdup("/");
	if (_result == NULL) {no8177_element_dispose(_splitted); return NULL;}
	
	while (_splitted != NULL) {
		byte *_slice = (byte *)_splitted->data;
		if (_slice != NULL) {
			if (strcmp(_slice, "..") == 0) {
				byte *_result0 = no8177_file_parent(_result);
				if (_result != NULL) free(_result);
				_result = _result0;
			}
			else if (strcmp(_slice, ".") != 0) {
				byte *_result0;
				if (strcmp(_result, "/") == 0) _result0 = no8177_string_connect(2, _result, _slice);
				else _result0 = no8177_string_connect(3, _result, "/", _slice);
				if (_result != NULL) free(_result);
				_result = _result0;
			}
		}
		
		_splitted = _splitted->next;
		no8177_element_drop(_splitted, 0);
	}
	
	return _result;
}
element *no8177_file_children(byte *address) {
	if (address == NULL) return NULL;
	
	byte *_address = no8177_file_clean_address(address); if (_address == NULL) return NULL;
	DIR *_stream = opendir(_address); if (_stream == NULL) {free(_address); return NULL;}
	element *_children = no8177_element_new(NULL, NULL, NULL); if (_children == NULL) {closedir(_stream); free(_address); return NULL;}
	
	while (_stream != NULL) {
		struct dirent *_child = readdir(_stream);
		byte *_name = (byte *)strdup(_child->d_name);
		if (_name == NULL) {no8177_element_dispose(_children); closedir(_stream); free(_address); return NULL;}
		if ((strcmp(_name, ".") == 0) || (strcmp(_name, "..") == 0)) {free(_name); continue;}
		
		_children->data = (void *)no8177_string_connect(3, _address, (no8177_string_ends_with(_address, "/") == 1) ? NULL : "/", _name);
		if (_children->data == NULL) {no8177_element_dispose(_children); closedir(_stream); free(_address); return NULL;}
		
		_children = no8177_element_new(_children, NULL, NULL);
	}
	
	closedir(_stream);
	free(_address);
	
	return no8177_element_first(_children);
}
byte no8177_file_create(byte *address, byte type) {
	byte *_address = no8177_file_clean_address(address);
	if (_address == NULL) return 0;
	if (strcmp(_address, "/") == 0) {free(_address); return 0;}
	
	int _result;
	
	if (type == 1) {
		if (no8177_file_is_directory(address, 0) == 1) {free(_address); return 1;}
		else if (no8177_file_exists(address) == 1) {free(_address); return 0;}
	}
	else {
		if (no8177_file_is_regular_file(address, 0) == 1) {free(_address); return 1;}
		else if (no8177_file_exists(address) == 1) {free(_address); return 0;}
	}
	
	byte *_parent = no8177_file_parent(_address);
	if (_parent == NULL) {free(_address); return 0;}
	_result = no8177_file_create(_parent, 1);
	free(_parent);
	if (_result != 1) {free(_address); return 0;}
	
	if (type == 1) {
		_result = mkdir(_address, 0700);
		free(_address);
	}
	else {
		_result = open(_address, O_RDWR | O_CREAT, 0600);
		free(_address);
		if (_result != -1) {close(_result); _result = 0;}
	}
	
	return (_result == 0) ? 1 : 0;
}
byte *no8177_file_load(byte *address, byte *result) {
	if (address == NULL) {
		if (result != NULL) result[0] = 0;
		return NULL;
	}
	
	byte *_address = no8177_file_clean_address(address);
	if (_address == NULL) {
		if (result != NULL) result[0] = 0;
		return NULL;
	}
	int _length = no8177_file_size(_address, 0);
	if (_length == -1) {free(_address); if (result != NULL) result[0] = 0; return NULL;}
	else if (_length == 0) {free(_address); if (result != NULL) result[0] = 1; return NULL;}
	
	byte *_data = (byte *)malloc(_length + 1);
	if (_data == NULL) {free(_address); if (result != NULL) result[0] = 0; return NULL;}
	
	int _file = open(_address, O_RDONLY); if (_file != -1) {
		int _nth;
		for (_nth = 0; _nth < _length; _nth++) read(_file, &_data[_nth], 1);
		close(_file);
		_data[_nth] = '\0';
	}
	
	if (result != NULL) result[0] = (_file == -1) ? 0 : 1;
	free(_address);
	
	return _data;
}
byte no8177_file_save(byte *address, byte *data, byte overwrite) {
	if (address == NULL) return 0;
	
	byte *_address = no8177_file_clean_address(address);
	if (_address == NULL) return 0;
	if (no8177_file_create(_address, 0) != 1) {free(_address); return 0;}
	
	int _result;
	if (overwrite == 1) _result = open(_address, O_RDWR | O_CREAT | O_TRUNC, 0600);
	else _result = open(_address, O_RDWR | O_CREAT, 0600);
	if (_result != -1) {
		if (data != NULL) {int _nth; for (_nth = 0; _nth < strlen(data); _nth++) write(_result, &data[_nth], 1);}
		close(_result);
		_result = 0;
	}
	
	free(_address);
	
	return (_result == 0) ? 1 : 0;
}
byte no8177_file_delete(byte *file) {
	if (file == NULL) return 0;
	
	byte *_file = no8177_file_clean_address(file);
	if (_file == NULL) return 0;
	if (no8177_file_exists(_file) != 1) {free(_file); return 1;}
	printf("FILE %s\n", _file);
	
	int _result;
	if (no8177_file_is_directory(_file, 1) == 1) {
		DIR *_directory = opendir(_file); if (_directory != NULL) {
			struct dirent *_tree; while (_directory != NULL) {
				_tree = readdir(_directory); if (_tree == NULL) break;
				byte *_name = (byte *)strdup(_tree->d_name);
				if (_name == NULL) {closedir(_directory); free(_file); return 0;}
				else if ((strcmp(_name, ".") == 0) || (strcmp(_name, "..") == 0)) {free(_name); continue;}
				
				byte *_child = no8177_string_connect(3, _file, "/", _name); free(_name);
				if (_child == NULL) {closedir(_directory); free(_file); return 0;}
				_result = no8177_file_delete(_child); free(_child);
				if (_result != 1) break;
			}
			
			closedir(_directory);
		}
		
		if (_result == 1) _result = rmdir(_file);
	}
	else _result = unlink(_file);
	
	free(_file);
	
	return (_result == 0) ? 1 : 0;
}
byte no8177_file_copy(byte *from, byte *to, byte overwrite) {
	if ((from == NULL) || (to == NULL)) return 0;
	
	byte *_from = no8177_file_clean_address(from); if (_from == NULL) return 0;
	if (no8177_file_can_read(_from) != 1) {free(_from); return 0;}
	byte *_to = no8177_file_clean_address(to); if (_to == NULL) {free(_from); return 0;}
	if (strcmp(_from, _to) == 0) {free(_from); free(_to); return 0;}
	
	if ((overwrite == 1) && (no8177_file_delete(_to) != 1)) {free(_from); free(_to); return 0;}
	else if ((overwrite != 1) && (no8177_file_exists(_to) == 1)) {free(_from); free(_to); return 0;}
	
	int _result;
	
	byte *_parent = no8177_file_parent(_to); if (_parent == NULL) {free(_from); free(_to); return 0;}
	_result = no8177_file_create(_parent, 1); free(_parent);
	if (_result != 1) {free(_from); free(_to); return 0;}
	
	if (no8177_file_is_directory(_from, 0) == 1) {
		_result = no8177_file_create(_to, 1); if (_result != 1) {free(_from); free(_to); return 0;}
		DIR *_directory = opendir(_from); if (_directory != NULL) {
			struct dirent *_tree; while (_directory != NULL) {
				_tree = readdir(_directory); if (_tree == NULL) break;
				byte *_name = (byte *)strdup(_tree->d_name);
				if (_name == NULL) {closedir(_directory); free(_from); free(_to); return 0;}
				else if ((strcmp(_name, ".") == 0) || (strcmp(_name, "..") == 0)) {free(_name); continue;}
				
				byte *_childFrom = no8177_string_connect(3, _from, "/", _name);
				if (_childFrom == NULL) {closedir(_directory); free(_name); free(_from); free(_to); return 0;}
				byte *_childTo = no8177_string_connect(3, _to, "/", _name);
				free(_name);
				if (_childTo == NULL) {closedir(_directory); free(_childFrom); free(_from); free(_to); return 0;}
				_result = no8177_file_copy(_childFrom, _childTo, 0);
				free(_childFrom); free(_childTo);
				if (_result != 1) break;
			}
			
			closedir(_directory);
		}
	}
	else {
		byte _resultLoad[2]; _resultLoad[0] = 0;
		byte *_data = no8177_file_load(_from, &_resultLoad[0]); _result = _resultLoad[0];
		if (_result == 1) _result = no8177_file_save(_to, _data, 0);
		if (_data != NULL) free(_data);
	}
	
	free(_from); free(_to);
	
	return (_result == 1) ? 1 : 0;
}
byte no8177_file_move(byte *from, byte *to, byte overwrite) {
	if ((from == NULL) || (to == NULL)) return 0;
	
	byte *_from = no8177_file_clean_address(from); if (_from == NULL) return 0;
	if (no8177_file_can_read(_from) != 1) {free(_from); return 0;}
	byte *_to = no8177_file_clean_address(to); if (_to == NULL) {free(_from); return 0;}
	if (strcmp(_from, _to) == 0) {free(_from); free(_to); return 0;}
	
	if ((overwrite == 1) && (no8177_file_delete(_to) != 1)) {free(_from); free(_to); return 0;}
	else if ((overwrite != 1) && (no8177_file_exists(_to) == 1)) {free(_from); free(_to); return 0;}
	
	int _result;
	
	byte *_parent = no8177_file_parent(_to); if (_parent == NULL) {free(_from); free(_to); return 0;}
	_result = no8177_file_create(_parent, 1); free(_parent);
	if (_result != 1) {free(_from); free(_to); return 0;}
	
	if (rename(_from, _to) != 0) {
		if (errno == EXDEV) {
			_result = no8177_file_copy(_from, _to, 0);
			if (_result == 1) no8177_file_delete(_from);
		}
		else _result = 0;
	}
	
	free(_from); free(_to);
	
	return (_result == 1) ? 1 : 0;
}

pair *no8177_database_new(pair *previous, void *a, void *b, pair *next) {
	pair *_pair = (pair *)malloc(sizeof(pair)); if (_pair == NULL) return NULL;
	_pair->previous = previous; if (previous != NULL) previous->next = _pair;
	_pair->a = a;
	_pair->b = b;
	_pair->next = next; if (next != NULL) next->previous = _pair;
	
	return _pair;
}
pair *no8177_database_first(pair *_pair) {
	if (_pair == NULL) return NULL;
	
	pair *_pair0 = _pair;
	while (_pair0->previous != NULL) _pair0 = _pair0->previous;
	
	return _pair0;
}
pair *no8177_database_last(pair *_pair) {
	if (_pair == NULL) return NULL;
	
	pair *_pair0 = _pair;
	while (_pair0->next != NULL) _pair0 = _pair0->next;
	
	return _pair0;
}
int no8177_database_length(pair *_pair) {
	pair *_pair0 = no8177_database_first(_pair);
	if (_pair0 == NULL) return 0;
	
	int _npairs = 1;
	while (_pair0->next != NULL) {_pair0 = _pair0->next; _npairs++;}
	
	return _npairs;
}
pair *no8177_database_get(pair *_pair, int nth) {
	int _length = no8177_database_length(_pair);
	if (_length == 0) return NULL;
	int _nth = nth; if (_nth == -1) _nth = _length - 1;
	if ((_nth < 0) || (_nth >= _length)) return NULL;
	
	pair *_pair0 = no8177_database_first(_pair);
	if (_nth == 0) return _pair0;
	for (_nth = 0; _nth < nth; _nth++) _pair0 = _pair0->next; return _pair0; 
}
void no8177_database_drop(pair *_pair, int nth) {
	pair *_pair0 = no8177_database_get(_pair, nth);
	if (_pair0 == NULL) return;
	
	pair *_pairLeft = _pair0->previous;
	pair *_pairRight = _pair0->next;
	if (_pair0->a != NULL) free(_pair0->a);
	if (_pair0->b != NULL) free(_pair0->b);
	free(_pair0);
	if (_pairLeft != NULL) _pairLeft->next = _pairRight;
	if (_pairRight != NULL) _pairRight->previous = _pairLeft;
}
void no8177_database_dispose(pair *_pair) {
	pair *_pair0 = no8177_database_first(_pair);
	if (_pair0 == NULL) return;
	
	while (_pair0 != NULL) {
		if (_pair0->a != NULL) free(_pair0->a);
		if (_pair0->b != NULL) free(_pair0->b);
		pair *_pair1 = _pair0->next;
		free(_pair0);
		_pair0 = _pair1;
	}
}
pair *no8177_database_from_string(byte *string) {
	if (string == NULL) return NULL;
	
	pair *_database = no8177_database_new(NULL, NULL, NULL, NULL);
	
	/* Split the string and put in a element. */
	int _nth; int _nthFrom = 0; int _nthTo = 0;
	byte _mode = 0; byte *_a; byte *_b;
	for (_nth = 0; _nth < strlen(string); _nth++) {
		byte _byte = string[_nth];
		
		if ((_nth = strlen(string) - 1) && (_byte != '\r') && (_byte != '\n')) { /* For the case that string does not ends with newline */
			if (_mode == 1) break;
			else if (_mode == 2) {free(_a); break;}
			else if (_mode == 3) {free(_a); break;}
			else if (_mode == 4) break;
			else break;
		}
		else {
			if (_mode == 1) { /* Mode: Key */
				if ((_byte == ' ') || (_byte == '\t')) {
					_nthTo = _nth - 1;
					_a = no8177_string_pick(string, _nthFrom, _nthTo);
					if (_a == NULL) {no8177_database_dispose(_database); return NULL;}
					_mode = 2;
				}
				else if ((_byte == '\r') || (_byte == '\n')) _mode = 0;
				/*else if (_byte == '#') _mode = 5;*/
				else {_nthFrom = _nth; _mode = 1;}
			}
			else if (_mode == 2) { /* Mode: Bridge */
				if ((_byte == ' ') || (_byte == '\t')) continue;
				else if ((_byte == '\r') || (_byte == '\n')) {free(_a); _mode = 0;}
				/*else if (_byte == '#') _mode = 5;*/
				else {_nthFrom = _nth; _mode = 3;}
			}
			else if (_mode == 3) { /* Mode: Value */
				/*if ((_byte == ' ') || (_byte == '\t')) continue;*/
				if ((_byte == '\r') || (_byte == '\n')) {
					_nthTo = _nth - 1;
					_b = no8177_string_pick(string, _nthFrom, _nthTo);
					if (_b == NULL) {free(_a); no8177_database_dispose(_database); return NULL;}
					
					pair *_pair = no8177_database_new(_database, NULL, NULL, NULL);
					if (_pair == NULL) {free(_a); free(_b); no8177_database_dispose(_database); return NULL;}
					_database->a = _a; _database->b = _b;
					_database = _database->next;
					
					_mode = 0;
				}
				/*else if (_byte == '#') _mode = 5;*/
				else continue;
			}
			else if (_mode == 4) { /* Mode: Comment */
				if ((_byte == '\r') || (_byte == '\n')) _mode = 0;
			}
			else if (_mode == 0) { /* Mode: Idle */
				if ((_byte == ' ') || (_byte == '\t')) continue;
				else if ((_byte == '\r') || (_byte == '\n')) continue;
				else if (_byte == '#') _mode = 4;
				else {_nthFrom = _nth; _mode = 1;}
			}
		}
	}
}
byte *no8177_database_to_string(pair *database) {
	if (database == NULL) return NULL;
	
	pair *_database = no8177_database_first(database);
	byte *_string = NULL;
	while (_database != NULL) {
		if ((_database->a != NULL) && (_database->b != NULL)) {
			byte *_string1 = no8177_string_connect(5, _string, (byte *)_database->a, " ", (byte *)_database->b, "\n");
			if (_string != NULL) free(_string); _string = _string1;
		}
		_database = _database->next;
	}
	
	return _string;
}
byte *no8177_database_get_value(pair *database, byte *key) {
	if (key == NULL) return NULL;
	
	pair *_database = database;
	while (_database != NULL) {
		if (strcmp((byte *)_database->a, key) == 0) return strdup((byte *)_database->b);
		
		_database = _database->next;
	}
	
	return NULL;
}
byte no8177_database_set_value(pair *database, byte *key, byte *value) {
	if (database == NULL) return 0;
	if ((key == NULL) || (value == NULL)) return 0;
	
	pair *_database = database;
	while (_database != NULL) {
		if (strcmp((byte *)_database->a, key) == 0) {
			byte *_valueNew = (byte *)strdup(value); if (_valueNew == NULL) return 0;
			if (_database->b != NULL) free(_database->b);
			_database->b = (void *)_valueNew;
			
			return 1;
		}
		
		_database = _database->next;
	}
	
	pair *_database1 = no8177_database_new(_database, key, value, NULL);
	
	return (_database1 != NULL) ? 1 : 0;
}


#ifdef MODE_TEST
int main(int nElements, char **elements, char **conditions) {
	/*byte *_test1 = no8177_string_connect(5, "Blueberry ", "is ", "sweet ", "and ", "delicious.");
	printf("STRING %s\n", _test1);
	free(_test1);*/
	/*printf("STATUS %d\n", no8177_file_can_read("/home/user/test.txt"));*/
	/*printf("SIZE %d\n", no8177_file_size("/home/user/test.zip", 0));*/
	/*element *_element = no8177_string_split("Blueberry is sweet and delicious.", " ", 1); if (_element != NULL) {
		byte *_re = no8177_element_row_to_string(_element, 1);
		printf("STRING %s\n", _re);
		no8177_element_dispose(_element);
		free(_re);
	}*/
	/*byte *_replaced = no8177_string_replace("타닥타닥타닥타닥타닥타닥타닥", "닥", "다다닥");
	printf("STRING %s\n", _replaced);
	free(_replaced);*/
	/*byte *_cleaned = no8177_file_clean_address("///sd/f/fwe//d////bd/df/df///fgm//y,uy/,t//rtty/,/.y/./y/y./././/...//t/j/rt/r/.../../dfa");
	if (_cleaned != NULL) {printf ("STRING %s\n", _cleaned); free(_cleaned);}*/
	/*no8177_file_create("undocumented/video/doohickey", 0);*/
	/*printf("DATA START\n");
	byte *_data = no8177_file_load("~/data/data1.txt", NULL); if (_data != NULL) {
		printf("%s\n", _data);
		free(_data);
	}
	printf("DATA END\n");*/
	/*no8177_file_save("summer/rocks", "KAFOSDAWEOGO", 0);*/
	/*no8177_file_delete("dhcpcd-7.0.1");*/
	/*no8177_file_copy("~/external/workbench/hercules/xinit/xinit-1.3.4", "xinit-backup", 0);*/
	no8177_file_move("../hercules/zenity/zenity-3.26.9999-1-x86_64.pkg.tar", "zenitia.tar", 0);
	
	return 0;
}
#endif
