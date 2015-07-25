#ifndef __READ_H
#define __READ_H

#include <list>
#include <string>
#include <QCache>
#include <dirent.h>

#include "DisksOperations.h"
#include "FileSystem.h"

#define MAX_CACHE_SIZE          500
#define INVALID_TABLE           3
#define FILE_TYPE_PARTITON		0x7E
#define FILE_TYPE_DIR			2

/* Identifies the type of file by using i_mode of inode */
/* structure as input.									*/
#define EXT2_S_ISDIR(mode)	((mode & 0x0f000) == 0x04000)
#define EXT2_S_ISLINK(mode)	((mode & 0x0f000) == 0x0a000)
#define EXT2_S_ISBLK(mode)	((mode & 0x0f000) == 0x06000)
#define EXT2_S_ISSOCK(mode)	((mode & 0x0f000) == 0x0c000)
#define EXT2_S_ISREG(mode)	((mode & 0x0f000) == 0x08000)
#define EXT2_S_ISCHAR(mode)	((mode & 0x0f000) == 0x02000)
#define EXT2_S_ISFIFO(mode)	((mode & 0x0f000) == 0x01000)

/* Identifies the type of file by using file_type of 	*/
/* directory structure as input.						*/
#define EXT2_FT_ISDIR(mode)		(mode  == 0x2)
#define EXT2_FT_ISLINK(mode)	(mode  == 0x7)
#define EXT2_FT_ISBLK(mode)		(mode  == 0x4)
#define EXT2_FT_ISSOCK(mode)	(mode  == 0x6)
#define EXT2_FT_ISREG(mode)		(mode  == 0x1)
#define EXT2_FT_ISCHAR(mode)	(mode  == 0x3)
#define EXT2_FT_ISFIFO(mode)	(mode  == 0x5)
#define IS_PART(mode)			(mode == FILE_TYPE_PARTITION)

/* Mask values for the access rights. */
/* User */
#define EXT2_S_IRUSR			0x0100
#define EXT2_S_IWUSR			0x0080
#define EXT2_S_IXUSR			0x0040
/* Group */
#define EXT2_S_IRGRP			0x0020
#define EXT2_S_IWGRP			0x0010
#define EXT2_S_IXGRP			0x0008
/* Others */
#define EXT2_S_IROTH			0x0004
#define EXT2_S_IWOTH			0x0002
#define EXT2_S_IXOTH			0x0001

#define IS_BUFFER_END(p, q, bsize)	(((char *)(p)) >= ((char *)(q) + bsize))

using namespace std;

static __inline__ const char *get_type_string(int type)
{
	switch(type)
	{
		case 0x1:	return "Regular File";
		case 0x2:	return "Directory";
		case 0x3:	return "Character Device";
		case 0x4:	return "Block Device";
		case 0x5:	return "Fifo File";
		case 0x6:	return "Socket File";
		case 0x7:	return "Symbolic Link";
	}

	return "Unknown Type";
}

static __inline__ char *get_access(unsigned long mode)
{
	static char acc[9];
	acc[0] = (mode & EXT2_S_IRUSR)? 'r':'-';
	acc[1] = (mode & EXT2_S_IWUSR)? 'w':'-';
	acc[2] = (mode & EXT2_S_IXUSR)? 'x':'-';

	acc[3] = (mode & EXT2_S_IRGRP)? 'r':'-';
	acc[4] = (mode & EXT2_S_IWGRP)? 'w':'-';
	acc[5] = (mode & EXT2_S_IXGRP)? 'x':'-';

	acc[6] = (mode & EXT2_S_IROTH)? 'r':'-';
	acc[7] = (mode & EXT2_S_IWOTH)? 'w':'-';
	acc[8] = (mode & EXT2_S_IXOTH)? 'x':'-';

	acc[9] = '\0';
	return acc;
}

// forward declaration
class Partition;
class VolumeGroup;

class ExtFile {
public:
    uint32_t    inode_num;
    uint8_t     file_type;
    string      file_name;
    uint64_t     file_size;

    EXT2_INODE  inode;
    Partition *partition;
    bool onview;
};

typedef struct ext2dirent {
    EXT2_DIR_ENTRY *next;
    EXT2_DIR_ENTRY *dirbuf;
    ExtFile *parent;
    uint64_t read_bytes;
    uint64_t next_block;
} EXT2DIRENT;

class Partition {
    HANDLE  handle;
    int sect_size;
    uint64_t total_sectors;
    uint64_t relative_sect;
    string linux_name;

    int inodes_per_group;
    int inode_size;
    int blocksize;
    uint32_t totalGroups;
    EXT2_GROUP_DESC *desc;
    char *inode_buffer;
    uint64_t last_block;
    ExtFile *root;
    QCache <uint64_t , char > buffercache;
    
    int ReadBlock(uint64_t blocknum, void *buffer);
    uint32_t FileblockToLogical(EXT2_INODE *ino, uint32_t lbn);
    uint64_t ExtentToLogical(EXT2_INODE *ino, uint64_t lbn);
    uint64_t ExtentBinarySearch(EXT4_EXTENT_HEADER *header, uint64_t lbn, bool isallocated);
    int Mount();

public:
    bool onview;
    bool is_valid;

public:
    Partition(uint64_t, uint64_t, int ssise, HANDLE);
    ~Partition();

    void SetName(const char *, int , int);
    void SetImageName(const char *name) { linux_name.assign(name); }
    string &GetLinuxName();
    ExtFile *GetRoot() { return root; }
    int GetBlockSize() { return blocksize; }
    ExtFile *ReadInode(uint32_t inum);
    int ReadDataBlock(EXT2_INODE *ino, uint64_t lbn, void *buf);
    EXT2DIRENT *OpenDirectory(ExtFile *parent);
    ExtFile *ReadDirectory(EXT2DIRENT *);
    void CloseDirectory(EXT2DIRENT *);

};

class Read {
private:
    int ndisks;

    list <Partition *> nparts;

    int ScanEBR(HANDLE , uint64_t , int , int, int);
    int ScanPartitions(char *path, int);


public:
    Read();
    ~Read();

    list <VolumeGroup *> groups;

    void ScanSystem();
    list<Partition *> GetPartitions();
    list<VolumeGroup *> &get_volgroups() { return groups; }
    void add_partition(Partition *part) { nparts.push_back(part); }
};

#ifdef __cplusplus
extern "C"{
#endif

int log_init();
void log_exit();
int log(const char *msg, ...);

#ifdef __cplusplus
}
#endif

#define LOG	log

#endif
