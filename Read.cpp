#include "Read.h"
#include "MBR.h"

#include "ui_extexplorer.h"

Read::Read()
{
    ScanSystem();
}

Read::~Read()
{
    list<Partition *>::iterator i;
    for(i = nparts.begin(); i != nparts.end(); i++)
    {
        delete *i;
    }

    nparts.clear();;
}

void Read::ScanSystem()
{
    char pathname[20];
    int ret;

    ndisks = GetDisks();
    LOG("�������� �������� �����: %d\n", ndisks);
    if(ndisks == 0)
    {
        LOG("������� ������� ������� �������� � ������� ������������\n");
    }

    for(int i = 0; i < ndisks; i++)
    {
        get_nthdevice(pathname, ndisks);
        LOG("\n���������� ����� %c\n", pathname[17]);
        ret = ScanPartitions(pathname, i);
        if(ret < 0)
        {
            LOG("������� ���������� %s!", pathname);
            continue;
        }
    }

    if(groups.empty())
        return;
}

list<Partition *> Read::GetPartitions()
{
    return nparts;
}

int Read::ScanEBR(HANDLE handle, uint64_t base, int sectsize, int disk, int i)
{
    unsigned char sector[SECTOR_SIZE];
    struct MBRpartition *part, *part1;
    Partition *partition;
    int ret;
    uint64_t  ebrBase, nextPart, ebr2=0;

    ebrBase = base;
    nextPart = base;
    while(1)
    {
        ret = ReadDisk(handle, sector, nextPart, 1, sectsize);
        if(ret < 0)
            return ret;

        if(ret < sectsize)
        {
            LOG("������� ������� EBR \n");
            return -1;
        }
        part = pt_offset(sector, 0);

        LOG("    �����: %d\n    �����: %.2f ��\n", i, (double)get_nr_sects(part)*sectsize/1024/1024/1024);

        switch(part->sys_ind)
        {
        case 0x06:
            LOG("    ��� FAT16\n\n");
            break;
        case 0x07:
            LOG("    ��� NTFS\n\n");
            break;
        case 0x0b:
            LOG("    ��� FAT32\n\n");
            break;
        case 0x0c:
            LOG("    ��� FAT32\n\n");
            break;
        case 0x82:
            LOG("    ��� SWAP\n\n");
            break;
        case 0x83:
            partition = new Partition(get_nr_sects(part), get_start_sect(part)+ ebrBase + ebr2, sectsize, handle);
            LOG("    ��� EXT\n\n");
            if(partition->is_valid)
            {
                partition->SetName("���� ", disk, i);
                nparts.push_back(partition);
            }
            else
            {
                delete partition;
            }
            break;
        default:
            LOG("      �������� ���\n\n");
            break;
        }

        i++;
        part1 = pt_offset(sector, 1);
        ebr2 = get_start_sect(part1);
        nextPart = (ebr2 + ebrBase);

        if(part1->sys_ind == 0)
            break;
    }
    return i;
}

int Read::ScanPartitions(char *path, int disk)
{
    unsigned char sector[SECTOR_SIZE];
    struct MBRpartition *part;
    Partition *partition;
    HANDLE handle;
    int sector_size;
    int ret, i, n = 0;

    handle = OpenDisk(path, &sector_size);
    if(handle < 0)
        return -1;

    ret = ReadDisk(handle,sector, 0, 1, sector_size);
    if(ret < 0)
        return ret;

    if(ret < sector_size)
    {
        LOG("������� ������� MBR � %s!\n", path);
        return -1;
    }

    if(!valid_part_table_flag(sector))
    {
        LOG("������� ������� ������ �� %s\n", path);
        LOG("Invalid End of sector marker");
        return -INVALID_TABLE;
    }

    /* First Scan primary Partitions */
    for(i = 0; i < 4; i++)
    {
        part = pt_offset(sector, i);
        if((part->sys_ind != 0x00) || (get_nr_sects(part) != 0x00))
        {
            switch(part->sys_ind)
            {
            case 0x05:
                ScanEBR(handle, get_start_sect(part), sector_size, disk, i);
                break;
            case 0x06:
                LOG("    �����: %d\n    �����: %.2f ��\n    ��� FAT16\n\n",
                    i, (double)get_nr_sects(part)*sector_size/1024/1024/1024);
                break;
            case 0x07:
                LOG("    �����: %d\n    �����: %.2f ��\n    ��� NTFS\n\n",
                    i, (double)get_nr_sects(part)*sector_size/1024/1024/1024);
                break;
            case 0x0b:
                LOG("    �����: %d\n    �����: %.2f ��\n    ��� FAT32\n\n",
                    i, (double)get_nr_sects(part)*sector_size/1024/1024/1024);
                break;
            case 0x0c:
                LOG("    �����: %d\n    �����: %.2f ��\n    ��� FAT32\n\n",
                    i, (double)get_nr_sects(part)*sector_size/1024/1024/1024);
                break;
            case 0x0f:
                ScanEBR(handle, get_start_sect(part), sector_size, disk, i);
                break;
            case 0x83:
                n++;
                LOG("    �����: %d\n    �����: %.2f ��\n",
                    i, (double)get_nr_sects(part)*sector_size/1024/1024/1024);
                partition = new Partition(get_nr_sects(part), get_start_sect(part), sector_size, handle);
                LOG("    ��� EXT\n\n");
                if(partition->is_valid)
                {
                    partition->SetName("���� ", disk, i);
                    nparts.push_back(partition);
                }
                else
                {
                    delete partition;
                }
                break;
            default:
                LOG("    �������� ���\n\n");
                break;
            }
        }
    }
    return 0;
}
