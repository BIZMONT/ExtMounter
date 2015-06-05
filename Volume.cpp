#include <stdlib.h>
#include <sstream>
#include <QRegExp>

#include "Volume.h"


VolumeGroup::VolumeGroup(QString &id, QString &name, int seq, int size)
{
    uuid = id;
    volname = name;
    seqno = seq;
    extent_size = size;
}

VolumeGroup::~VolumeGroup()
{
    list<PhysicalVolume *>::iterator i;
    list<LogicalVolume *>::iterator j;
    for(i = pvolumes.begin(); i != pvolumes.end(); i++)
    {
        delete (*i);
    }

    for(j = lvolumes.begin(); j != lvolumes.end(); j++)
    {
        delete (*j);
    }
}

PhysicalVolume *VolumeGroup::find_physical_volume(QString &id)
{
    PhysicalVolume *pvol;
    list<PhysicalVolume *>::iterator i;

    for(i = pvolumes.begin(); i != pvolumes.end(); i++)
    {
        pvol = (*i);
        if(pvol->uuid.compare(id) == 0)
        {
            return pvol;
        }
    }

    return NULL;
}

PhysicalVolume *VolumeGroup::add_physical_volume(QString &id, uint64_t devsize, uint32_t start, uint32_t count, HANDLE file, uint64_t dsk_offset)
{
    PhysicalVolume *pvol;

    pvol = new PhysicalVolume(id, devsize, start, count, file, dsk_offset);
    if(!pvol)
        return NULL;

    pvolumes.push_back(pvol);
    return pvol;
}

LogicalVolume *VolumeGroup::find_logical_volume(QString &id)
{
    LogicalVolume *lvol;
    list<LogicalVolume *>::iterator i;

    for(i = lvolumes.begin(); i != lvolumes.end(); i++)
    {
        lvol = (*i);
        if(lvol->uuid.compare(id) == 0)
        {
            return lvol;
        }
    }

    return NULL;
}

LogicalVolume *VolumeGroup::add_logical_volume(QString &id, int count, QString &vname)
{
    LogicalVolume *lvol;

    lvol = new LogicalVolume(id, count, vname, this);
    if(!lvol)
        return NULL;

    lvolumes.push_back(lvol);
    return lvol;
}

void VolumeGroup::logical_mount()
{
    //EXT2_SUPER_BLOCK sblock;
    LogicalVolume *lvol;
    //PhysicalVolume *pvol;
    Partition *partition;
    lv_segment *seg;
    lv_segment *root = NULL;
    list<LogicalVolume *>::iterator i;
    list<lv_segment *>::iterator j;
    list<PhysicalVolume *>::iterator k;
    uint64_t start;
    int index = 0;

    for(i = lvolumes.begin(); i != lvolumes.end(); i++)
    {
        lvol = (*i);
        for(j = lvol->segments.begin(); j != lvol->segments.end(); j++)
        {
            seg = (*j);
            if(seg->start_extent == 0)
                root = seg;

            index = 0;
            for(k = pvolumes.begin(); k != pvolumes.end(); k++)
            {
                if(seg->stripe->stripe_pv == index)
                {
                    seg->pvolumes = (*k);
                    break;
                }
                index++;
            }
        }

        if(!root)
            continue;

        start = root->pvolumes->pe_start + root->pvolumes->offset + root->stripe->stripe_start_extent;
        partition = new Partition(root->pvolumes->dev_size, start, SECTOR_SIZE, root->pvolumes->handle, lvol);
        if(partition->is_valid)
        {
            QByteArray ba;
            ba = lvol->volname.toAscii();
            partition->set_image_name(ba.data());
            read->add_partition(partition);
        }
        else
        {
            delete partition;
        }
    }
}

PhysicalVolume::PhysicalVolume(QString &id, uint64_t devsize, uint32_t start, uint32_t count, HANDLE file, uint64_t dsk_offset)
{
    uuid = id;
    dev_size = devsize;
    pe_start = start;
    pe_count = count;
    handle = file;
    offset = dsk_offset;
}

LogicalVolume::LogicalVolume(QString &id, int nsegs, QString &vname, VolumeGroup *vol)
{
    uuid = id;
    segment_count = nsegs;
    this_group = vol;
    volname = vname;
}

LogicalVolume::~LogicalVolume()
{

}

uint64_t LogicalVolume::lvm_mapper(uint64_t sectno)
{
    lv_segment *seg;
    uint64_t sect_mapped = 0;
    uint32_t extent_no, extent_offset;
    list<lv_segment *>::iterator iterate;

    extent_no = sectno / this_group->extent_size;
    extent_offset = sectno % this_group->extent_size;

    for(iterate = segments.begin(); iterate != segments.end(); iterate++)
    {
        seg = (*iterate);
        if((extent_no >= seg->start_extent) && (extent_no < (seg->start_extent + seg->extent_count)))
        {
            sect_mapped = (uint64_t)(extent_no *  this_group->extent_size) + extent_offset;
            sect_mapped += seg->pvolumes->pe_start + seg->pvolumes->offset + seg->stripe->stripe_start_extent;
            break;
        }
    }

    if(sect_mapped == 0)
    {
        LOG("Error in LVM Mapping \n");
    }
    return sect_mapped;
}
