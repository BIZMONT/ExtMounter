#ifndef __LVM_H
#define __LVM_H

#include <stdint.h>
#include <QString>

#include "Read.h"

#define LVM_SIGLEN	8
#define LVM_MAGIC_LEN	8
#define UUID_LEN	32

/* Structure to hold Physical Volumes (PV) label*/
typedef struct pv_label_header {
        char        pv_name[LVM_SIGLEN];   // Physical volume signature
        uint64_t    pv_sector_xl;          // sector number of this label
        uint32_t    pv_crc;                // CRC value
        uint32_t    pv_offset_xl;          // Offset from start of struct to contents
        char        pv_vermagic[LVM_MAGIC_LEN]; // Physical Volume version "LVM2 001"
        char        pv_uuid[UUID_LEN];
        uint64_t    pv_unknown1[5];             // documentation lacks for lvm
        uint64_t    pv_labeloffset;             // location of the label
} __attribute__ ((__packed__)) PV_LABEL_HEADER;

typedef struct pv_label {
    uint32_t        pv_magic;
    char            pv_sig[4];          // signature
    uint64_t        unknown1[2];
    uint64_t        pv_offset_low;
    uint64_t        unknown2;
    uint64_t        pv_offset_high;
    uint64_t        pv_length;
} __attribute__ ((__packed__)) PV_LABEL;

class VolumeGroup;

class LVM {
private:
    FileHandle pv_handle;
    lloff_t pv_offset;
    char uuid[UUID_LEN + 1];
    QString pv_metadata;
    Read *read;
public:
    LVM(FileHandle handle, lloff_t offset, Read *rd);
    ~LVM();

    int scan_pv();
    int parse_metadata();
    VolumeGroup *find_volgroup(QString &uuid);
    VolumeGroup *add_volgroup(QString &uuid, QString &name, int seq, int size);
};


class PhysicalVolume {
public:
    lloff_t dev_size;
    FileHandle handle;
    uint32_t pe_start, pe_count;
    lloff_t offset;     // offset from the start of disk to lvm volume
    QString uuid;
    PhysicalVolume(QString &id, lloff_t devsize, uint32_t start, uint32_t count, FileHandle file, lloff_t dsk_offset);
};

// Multiple stripes NOT Implemented: we only support linear for now.
struct stripe {
    int stripe_pv;
    uint32_t stripe_start_extent;
};

class lv_segment {
public:
    uint32_t start_extent;
    uint32_t extent_count;
    struct stripe *stripe;
    PhysicalVolume *pvolumes;

    lv_segment(uint32_t start, uint32_t count)
    {
        start_extent = start;
        extent_count = count;
    }
};

class LogicalVolume {
    int segment_count;
    VolumeGroup *this_group;
public:
    QString uuid;
    QString volname;
    std::list <lv_segment *> segments;
    //std::list <PhysicalVolume *> pvolumes;

    LogicalVolume(QString &id, int nsegs, QString &vname, VolumeGroup *);
    ~LogicalVolume();
    lloff_t lvm_mapper(lloff_t block);
};

class VolumeGroup {
public:
    QString volname;
    QString uuid;
    int extent_size;
    int seqno;
    int max_lv, max_pv;
    std::list <PhysicalVolume *> pvolumes;
    std::list <LogicalVolume *> lvolumes;
    Read *read;

public:
    VolumeGroup(QString &id, QString &name, int seq, int size);
    ~VolumeGroup();
    PhysicalVolume *find_physical_volume(QString &id);
    PhysicalVolume *add_physical_volume(QString &id, lloff_t devsize, uint32_t start, uint32_t count, FileHandle file, lloff_t dsk_offset);
    LogicalVolume *find_logical_volume(QString &id);
    LogicalVolume *add_logical_volume(QString &id, int count, QString &vname);
    void logical_mount();
    void set_ext2read(Read *ext2) { read = ext2; }
};

#ifdef __cplusplus
extern "C"{
#endif
int scan_lvm(FileHandle handle, lloff_t offset);

#ifdef __cplusplus
}
#endif

#endif
