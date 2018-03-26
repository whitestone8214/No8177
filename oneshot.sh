#!/bin/bash

# To build: ./oneshot.sh build
# To install: ./oneshot.sh install [root-directory]
# To link: ./oneshot.sh link [root-directory]
# To unlink: ./oneshot.sh unlink
# To uninstall: ./oneshot.sh uninstall

# To test: ./oneshot.sh test


_versionNo8177="1"
_editionNo8177="1"
_destinationNo8177="packages/no8177/${_versionNo8177}-${_editionNo8177}"

if (test "$1" = "build"); then
	rm -f libno8177.so
	
	gcc no8177.c -I. -shared -fPIC -o libno8177.so
elif (test "$1" = "install"); then
	$0 uninstall || exit 1
	
	rm -rf $2/${_destinationNo8177} || exit 1
	mkdir -p $2/${_destinationNo8177}/{assets,headers} || exit 1
	
	cp -f libno8177.so $2/${_destinationNo8177} || exit 1
	cp -f no8177.h $2/${_destinationNo8177}/headers || exit 1
	cp -f no8177.pc $2/${_destinationNo8177}/assets
elif (test "$1" = "link"); then
	$0 unlink || exit 1
	
	ln -sf $2/${_destinationNo8177}/libno8177.so /lib || exit 1
	ln -sf $2/${_destinationNo8177}/headers/no8177.h /usr/include || exit 1
	ln -sf $2/${_destinationNo8177}/assets/no8177.pc /lib/pkgconfig || exit 1
elif (test "$1" = "unlink"); then
	rm -f /lib/libno8177.so /usr/include/no8177.h /lib/pkgconfig/no8177.pc || exit 1
elif (test "$1" = "uninstall"); then
	rm -rf $2/${_destinationNo8177} || exit 1
	
elif (test "$1" = "test"); then
	rm -f no8177_test
	
	gcc -DMODE_TEST -I. no8177.c -fPIC -o no8177_test || exit 1
	./no8177_test
fi
