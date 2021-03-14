# Exercise 4

## Install the example programs of the UNIX Network Programming,Volume 2:Interprocess Communications" by Stevens in a Linux system (into your home directory). A gzip'ed tar archive is available at <http://www.cs.helsinki.fi/u/kojo/vst/unpv22e.tar.gz> Compile the basic library (lib directory). What changes, if any, are needed to compile the basic library of the example code?
---
* Trying to compile lib throws compiler errors
```bash
gcc -g -O2 -D_REENTRANT -Wall -D_POSIX_PTHREAD_SEMANTICS   -c -o daemon_inetd.o daemon_inetd.c
In file included from unpipc.h:7:0,
                 from daemon_inetd.c:1:
../config.h:56:17: error: two or more data types in declaration specifiers
 #define uint8_t unsigned char    /* <sys/types.h> */
                 ^
../config.h:56:26: error: two or more data types in declaration specifiers
 #define uint8_t unsigned char    /* <sys/types.h> */
                          ^
../config.h:57:18: error: two or more data types in declaration specifiers
 #define uint16_t unsigned short    /* <sys/types.h> */
                  ^
../config.h:57:27: error: two or more data types in declaration specifiers
 #define uint16_t unsigned short    /* <sys/types.h> */
                           ^
../config.h:58:18: error: two or more data types in declaration specifiers
 #define uint32_t unsigned int    /* <sys/types.h> */
                  ^
../config.h:58:27: error: two or more data types in declaration specifiers
 #define uint32_t unsigned int    /* <sys/types.h> */
                           ^
<builtin>: recipe for target 'daemon_inetd.o' failed
make: *** [daemon_inetd.o] Error 1
```

* config.h file contains data type declarations for uint*_t that are allready defined in our system
* Removing these three lines allows us to compile

```makefile
#define	uint8_t unsigned char				/* <sys/types.h> */
#define	uint16_t unsigned short				/* <sys/types.h> */
#define	uint32_t unsigned int				/* <sys/types.h> */
```

