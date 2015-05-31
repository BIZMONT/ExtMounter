#ifndef __PLATFORM_H
#define __PLATFORM_H

#ifdef WIN32
#include <windows.h>
#include <stdint.h>

 typedef HANDLE FileHandle;
#ifdef _MSC_VER
 typedef unsigned char uint8_t;
 typedef unsigned short uint16_t;
 typedef unsigned int uint32_t;
#endif
 
 #define FILE_DELIM		"\\"
#else
 typedef int FileHandle;
 #define FILE_DELIM	"/"
#endif

 typedef uint64_t lloff_t;

typedef unsigned int uint;
typedef unsigned long ulong;

#define SECTOR_SIZE             512

#if defined(__GNUC__)
 #define PACKED __attribute__((packed))
#else
 #define PACKED
#endif

#ifndef INLINE
  #if defined(_MSC_VER)
   #define INLINE __inline
  #elif defined(__GNUC__)
   #define INLINE __inline__
  #endif /* Compiler */
#endif /* INLINE */

#ifdef __cplusplus
extern "C"{
#endif


FileHandle open_disk(const char *, int *);
int get_ndisks();
void close_disk(FileHandle);
int read_disk(FileHandle hnd, void *ptr, lloff_t sector, int nsects, int sectorsize);
int write_disk(FileHandle hnd, void *ptr, lloff_t sector, int nsects, int sectorsize);
int get_nthdevice(char *path, int ndisks);

#ifdef __cplusplus
}
#endif


#endif
