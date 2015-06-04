#ifndef DISKSOPERATIONS_H
#define DISKSOPERATIONS_H

#include <windows.h>
#include <winioctl.h>
#include <stdint.h>
#include <fcntl.h>

#define DEVICE	"\\\\.\\PhysicalDrive0"
#define SECTOR_SIZE 512

HANDLE OpenDisk(const char *, int *);
int GetDisks();
int ReadDisk(HANDLE hnd, void *ptr, uint64_t sector, int nsects, int sectorsize);
int get_nthdevice(char *path, int ndisks);

#endif
