#include <dirent.h>

#include "Read.h"
#include "Partition.h"
#include "LVM.h"

#define  EXT2	0x83
#define  LINUX_LVM	0x8e

Read::Read()
{
    scan_system();
}

Read::~Read()
{
    clear_partitions();
}

void Read::scan_system()
{
    char pathname[20];
    int ret;

    ndisks = get_ndisks();
    LOG_INFO("Кількість фізичних дисків: %d\n", ndisks);
    LOG_INFO("=============================\n");

    for(int i = 0; i < ndisks; i++)
    {
        get_nthdevice(pathname, ndisks);
        LOG_INFO("\nСканування %s\n", pathname);
        ret = scan_partitions(pathname, i);
        if(ret < 0)
        {
            LOG_INFO("Помилка сканування %s!", pathname);
            continue;
        }
    }

    // Now Mount the LVM Partitions
    if(groups.empty())
        return;

    list<VolumeGroup *>::iterator i;
    VolumeGroup *grp;
    for(i = groups.begin(); i != groups.end(); i++)
    {
        grp = (*i);
        grp->logical_mount();
    }
}

void Read::clear_partitions()
{
    list<Partition *>::iterator i;
    for(i = nparts.begin(); i != nparts.end(); i++)
    {
        delete *i;
    }

    nparts.clear();
}

list<Partition *> Read::get_partitions()
{
    return nparts;
}

/* Reads The Extended Partitions */
int Read::scan_ebr(FileHandle handle, lloff_t base, int sectsize, int disk)
{
    unsigned char sector[SECTOR_SIZE];
    struct MBRpartition *part, *part1;
    Partition *partition;
    int logical = 4, ret;
    lloff_t  ebrBase, nextPart, ebr2=0;

    ebrBase = base;
    nextPart = base;
    while(1)
    {
        ret = read_disk(handle, sector, nextPart, 1, sectsize);
        if(ret < 0)
            return ret;

        if(ret < sectsize)
        {
            LOG("Помилка читання EBR \n");
            return -1;
        }
        part = pt_offset(sector, 0);
        LOG("index %d ID %X size %Ld \n", logical, part->sys_ind, get_nr_sects(part));

        if(part->sys_ind == EXT2)
        {
            partition = new Partition(get_nr_sects(part), get_start_sect(part)+ ebrBase + ebr2, sectsize, handle, NULL);
            if(partition->is_valid)
            {
                partition->set_linux_name("/dev/sd", disk, logical);
                nparts.push_back(partition);
            }
            else
            {
                delete partition;
            }
        }

        if(part->sys_ind == LINUX_LVM)
        {
            LOG("LVM Physical Volume found disk %d partition %d\n", disk, logical);
            LVM lvm(handle, get_start_sect(part)+ ebrBase + ebr2, this);
            lvm.scan_pv();
        }

        part1 = pt_offset(sector, 1);
        ebr2 = get_start_sect(part1);
        nextPart = (ebr2 + ebrBase);

        logical++;
        if(part1->sys_ind == 0)
            break;
    }
    return logical;
}

/* Scans The partitions */
int Read::scan_partitions(char *path, int diskno)
{
    unsigned char sector[SECTOR_SIZE];
    struct MBRpartition *part;
    Partition *partition;
    FileHandle handle;
    int sector_size;
    int ret, i;

    handle = open_disk(path, &sector_size);
    if(handle < 0)
        return -1;

    ret = read_disk(handle,sector, 0, 1, sector_size);
    if(ret < 0)
        return ret;

    if(ret < sector_size)
    {
        LOG("Помилка читання MBR з %s!\n", path);
        return -1;
    }

    if(!valid_part_table_flag(sector))
    {
        LOG("Partition Table Error on %s\n", path);
        LOG("Invalid End of sector marker");
        return -INVALID_TABLE;
    }

    /* First Scan primary Partitions */
    for(i = 0; i < 4; i++)
    {
        part = pt_offset(sector, i);
        if((part->sys_ind != 0x00) || (get_nr_sects(part) != 0x00))
        {
            LOG("index %d ID %X size %Ld \n", i, part->sys_ind, get_nr_sects(part));

            if(part->sys_ind == EXT2)
            {
                partition = new Partition(get_nr_sects(part), get_start_sect(part), sector_size, handle, NULL);
                if(partition->is_valid)
                {
                    partition->set_linux_name("/dev/sd", diskno, i);
                    nparts.push_back(partition);
                    LOG("На диску %d знайдено розділ %d\n", diskno, i);
                }
                else
                {
                    delete partition;
                }
            }

            if(part->sys_ind == LINUX_LVM)
            {
                LOG("LVM Physical Volume found disk %d partition %d\n", diskno, i);
                LVM lvm(handle, get_start_sect(part), this);
                lvm.scan_pv();
            }
            else if((part->sys_ind == 0x05) || (part->sys_ind == 0x0f))
            {
                scan_ebr(handle, get_start_sect(part), sector_size, diskno);
            }
        }
    }

    return 0;
}
