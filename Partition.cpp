#include "Read.h"

Partition::Partition(uint64_t size, uint64_t offset, int ssize, HANDLE phandle)
{
    int ret;
    total_sectors = size;
    relative_sect = offset;
    handle = phandle;
    sect_size = ssize;
    onview = false;
    inode_buffer = NULL;
    buffercache.setMaxCost(MAX_CACHE_SIZE);
    ret = Mount();
    if(ret < 0)
    {
        is_valid = false;
        return;
    }

    root = ReadInode(EXT2_ROOT_INO);
    if(!root)
    {
        is_valid = false;
        LOG("Cannot read the root of %s \n", linux_name.c_str());
        return;
    }

    root->file_name = linux_name;
    root->file_type = 0x02;
    is_valid = true;
}

Partition::~Partition()
{
    free (desc);
}

void Partition::SetName(const char *name, int disk, int partition)
{
    char dchar = '0' + disk;
    char pchar = '0' + partition;


    linux_name = name;
    linux_name.append(1, dchar);
    linux_name.append(" Розділ ");
    linux_name.append(1, pchar);
}

string &Partition::GetLinuxName()
{
    return linux_name;
}

int Partition::ReadBlock(uint64_t blocknum, void *buffer)
{
    char *newbuffer;
    int nsects = blocksize/sect_size;
    int ret;
    uint64_t sectno;

    newbuffer = buffercache.take(blocknum);
    if(!newbuffer)
    {
        newbuffer = new char[blocksize];
        if(!newbuffer)
            return -1;

        else
        {
            sectno = (uint64_t)((uint64_t)nsects * blocknum) + relative_sect;
        }
        ret = ReadDisk(handle, newbuffer, sectno, nsects, sect_size);
        if(ret < 0)
        {
            delete [] newbuffer;
            return ret;
        }
    }

    memcpy(buffer, newbuffer, blocksize);
    buffercache.insert(blocknum, newbuffer, 1);
    return 0;
}

int Partition::Mount()
{
    EXT2_SUPER_BLOCK sblock;
    int gSizes, gSizeb;
    char *tmpbuf;

    ReadDisk(handle, &sblock, relative_sect + 2, 2, sect_size);
    if(sblock.s_magic != EXT2_SUPER_MAGIC)
    {
        LOG("Bad Super Block. The drive %s is not ext2 formatted.\n", linux_name.c_str());
        return -1;
    }

    if(sblock.s_feature_incompat & EXT2_FEATURE_INCOMPAT_COMPRESSION)
    {
        LOG("File system compression is used which is not supported.\n");
    }
    blocksize = EXT2_BLOCK_SIZE(&sblock);
    inodes_per_group = EXT2_INODES_PER_GROUP(&sblock);
    inode_size = EXT2_INODE_SIZE(&sblock);

    LOG("    Розмір блоку: %d\n", blocksize);
    totalGroups = (sblock.s_blocks_count)/EXT2_BLOCKS_PER_GROUP(&sblock);
    gSizeb = (sizeof(EXT2_GROUP_DESC) * totalGroups);
    gSizes = (gSizeb / sect_size)+1;

    desc = (EXT2_GROUP_DESC *) calloc(totalGroups, sizeof(EXT2_GROUP_DESC));
    if(desc == NULL)
    {
        LOG("Not enough Memory: mount: desc: Exiting\n");
        exit(1);
    }

    if((tmpbuf = (char *) malloc(gSizes * sect_size)) == NULL)
    {
        LOG("Not enough Memory: mount: tmpbuf: Exiting\n");
        exit(1);
    }

    if((blocksize/sect_size) <= 2)
        ReadDisk(handle, tmpbuf, relative_sect + ((blocksize/sect_size) + 2), gSizes, sect_size);
    else
        ReadDisk(handle, tmpbuf, relative_sect + (blocksize/sect_size), gSizes, sect_size);

    memcpy(desc, tmpbuf, gSizeb);

    free(tmpbuf);

    return 0;
}

EXT2DIRENT *Partition::OpenDirectory(ExtFile *parent)
{
    EXT2DIRENT *dirent;

    if(!parent)
        return NULL;

    dirent = new EXT2DIRENT;
    dirent->parent = parent;
    dirent->next = NULL;
    dirent->dirbuf = NULL;
    dirent->read_bytes = 0;
    dirent->next_block = 0;

    return dirent;
}

ExtFile *Partition::ReadDirectory(EXT2DIRENT *dirent)
{
    string filename;
    ExtFile *newEntry;
    char *pos;
    int ret;

    if(!dirent)
        return NULL;
    if(!dirent->dirbuf)
    {
        dirent->dirbuf = (EXT2_DIR_ENTRY *) new char[blocksize];
        if(!dirent->dirbuf)
            return NULL;
        ret = ReadDataBlock(&dirent->parent->inode, dirent->next_block, dirent->dirbuf);
        if(ret < 0)
            return NULL;

        dirent->next_block++;
    }

    again:
    if(!dirent->next)
        dirent->next = dirent->dirbuf;
    else
    {
        pos = (char *) dirent->next;
        dirent->next = (EXT2_DIR_ENTRY *)(pos + dirent->next->rec_len);
        if(IS_BUFFER_END(dirent->next, dirent->dirbuf, blocksize))
        {
            dirent->next = NULL;
            if(dirent->read_bytes < dirent->parent->file_size)
            {
                ret = ReadDataBlock(&dirent->parent->inode, dirent->next_block, dirent->dirbuf);
                if(ret < 0)
                    return NULL;

                dirent->next_block++;
                goto again;
            }
            return NULL;
        }
    }

    dirent->read_bytes += dirent->next->rec_len;
    filename.assign(dirent->next->name, dirent->next->name_len);
    if((filename.compare(".") == 0) ||
       (filename.compare("..") == 0))
        goto again;


    newEntry = ReadInode(dirent->next->inode);
    if(!newEntry)
    {
        LOG("Помилка читання Inode %d батьківський inode %d.\n", dirent->next->inode, dirent->parent->inode_num);
        return NULL;
    }

    newEntry->file_type = dirent->next->filetype;
    newEntry->file_name = filename;

    return newEntry;
}

void Partition::CloseDirectory(EXT2DIRENT *dirent)
{
    delete [] dirent->dirbuf;
    delete dirent;
}

ExtFile *Partition::ReadInode(uint32_t inum)
{
    uint32_t group, index, blknum;
    int inode_index, ret = 0;
    ExtFile *file = NULL;
    EXT2_INODE *src;

    if(inum == 0)
        return NULL;

    if(!inode_buffer)
    {
        inode_buffer = (char *)malloc(blocksize);
        if(!inode_buffer)
            return NULL;
    }

    group = (inum - 1) / inodes_per_group;

    if(group > totalGroups)
    {
        LOG("Помилка читання Inode %X. Непрвльний номер Inode\n", inum);
        return NULL;
    }

    index = ((inum - 1) % inodes_per_group) * inode_size;
    inode_index = (index % blocksize);
    blknum = desc[group].bg_inode_table + (index / blocksize);


    if(blknum != last_block)
        ret = ReadBlock(blknum, inode_buffer);

    file = new ExtFile;
    if(!file)
    {
        LOG("Не знайдено шлях до файлу \n");
        return NULL;
    }
    src = (EXT2_INODE *)(inode_buffer + inode_index);
    file->inode = *src;
    file->inode_num = inum;
    file->file_size = (uint64_t) src->i_size | ((uint64_t) src->i_size_high << 32);
    if(file->file_size == 0)
    {
        LOG("Inode %d with file size 0\n", inum);
    }
    file->partition = (Partition *)this;
    file->onview = false;

    last_block = blknum;

    return file;
}

int Partition::ReadDataBlock(EXT2_INODE *ino, uint64_t lbn, void *buf)
{
    uint64_t block;

    if(INODE_HAS_EXTENT(ino))
        block = ExtentToLogical(ino, lbn);
    else
        block = FileblockToLogical(ino, lbn);

    if(block == 0)
        return -1;

    return ReadBlock(block, buf);
}

uint64_t Partition::ExtentBinarySearch(EXT4_EXTENT_HEADER *header, uint64_t lbn, bool isallocated)
{
    EXT4_EXTENT *extent;
    EXT4_EXTENT_IDX *index;
    EXT4_EXTENT_HEADER *child;
    uint64_t physical_block = 0;
    uint64_t block;

    if(header->eh_magic != EXT4_EXT_MAGIC)
    {
        LOG("Invalid magic in Extent Header: %X\n", header->eh_magic);
        return 0;
    }
    extent = EXT_FIRST_EXTENT(header);
    if(header->eh_depth == 0)
    {        
        for(int i = 0; i < header->eh_entries; i++)
        {         
            if((lbn >= extent->ee_block) &&
               (lbn < (extent->ee_block + extent->ee_len)))
            {
                physical_block = ext_to_block(extent) + lbn;
                physical_block = physical_block - (uint64_t)extent->ee_block;
                if(isallocated)
                    delete [] header;
                return physical_block;
            }
            extent++;
        }
        return 0;
    }

    index = EXT_FIRST_INDEX(header);
    for(int i = 0; i < header->eh_entries; i++)
    {
        if(lbn >= index->ei_block)
        {
            child = (EXT4_EXTENT_HEADER *) new char [blocksize];
            block = idx_to_block(index);
            ReadBlock(block, (void *) child);

            return ExtentBinarySearch(child, lbn, true);
        }
    }

    if(isallocated)
        delete [] header;

    return physical_block;
}

uint64_t Partition::ExtentToLogical(EXT2_INODE *ino, uint64_t lbn)
{
    uint64_t block;
    struct ext4_extent_header *header;

    header = get_ext4_header(ino);
    block = ExtentBinarySearch(header, lbn, false);

    return block;
}

uint32_t Partition::FileblockToLogical(EXT2_INODE *ino, uint32_t lbn)
{
    uint32_t block, indlast, dindlast;
    uint32_t tmpblk, sz;
    uint32_t *indbuffer;
    uint32_t *dindbuffer;
    uint32_t *tindbuffer;

    if(lbn < EXT2_NDIR_BLOCKS)
    {
        return ino->i_block[lbn];
    }

    sz = blocksize / sizeof(uint32_t);
    indlast = sz + EXT2_NDIR_BLOCKS;
    indbuffer = new uint32_t [sz];
    if((lbn >= EXT2_NDIR_BLOCKS) && (lbn < indlast))
    {
        block = ino->i_block[EXT2_IND_BLOCK];
        ReadBlock(block, indbuffer);
        lbn -= EXT2_NDIR_BLOCKS;
        block = indbuffer[lbn];
        delete [] indbuffer;
        return block;
    }

    dindlast = (sz * sz) + indlast;
    dindbuffer = new uint32_t [sz];
    if((lbn >= indlast) && (lbn < dindlast))
    {
        block = ino->i_block[EXT2_DIND_BLOCK];
        ReadBlock(block, dindbuffer);

        tmpblk = lbn - indlast;
        block = dindbuffer[tmpblk/sz];
        ReadBlock(block, indbuffer);

        lbn = tmpblk % sz;
        block = indbuffer[lbn];

        delete [] dindbuffer;
        delete [] indbuffer;
        return block;
    }

    tindbuffer = new uint32_t [sz];
    if(lbn >= dindlast)
    {
        block = ino->i_block[EXT2_TIND_BLOCK];
        ReadBlock(block, tindbuffer);

        tmpblk = lbn - dindlast;
        block = tindbuffer[tmpblk/(sz * sz)];
        ReadBlock(block, dindbuffer);

        block = tmpblk / sz;
        lbn = tmpblk % sz;
        block = dindbuffer[block];
        ReadBlock(block, indbuffer);
        block = indbuffer[lbn];

        delete [] tindbuffer;
        delete [] dindbuffer;
        delete [] indbuffer;

        return block;
    }
    return 0;
}
