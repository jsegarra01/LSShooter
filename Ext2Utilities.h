//
// Created by josep on 10/04/2022.
//

#ifndef SHOOTER_EXT2UTILITIES_H
#define SHOOTER_EXT2UTILITIES_H

#include "Logic.h"

typedef struct Inode {
    unsigned short s_inode_size;
    unsigned int s_inodes_count;
    unsigned int s_first_ino;
    unsigned int s_inodes_per_group;
    unsigned int s_free_inodes_count;
} Inode;

typedef struct Block {
    unsigned int s_log_block_size;
    unsigned int s_blocks_count;
    unsigned int s_r_blocks_count;
    unsigned int s_free_blocks_count;
    unsigned int s_first_data_block;
    unsigned int s_blocks_per_group;
    unsigned int s_frags_per_group;
} Block;

typedef struct Volume {
    char s_volume_name[16];
    unsigned int s_mtime;
    unsigned int s_lastcheck;
    unsigned int s_wtime;
} Volume;

typedef struct Ext2{
    Inode inode;
    Block block;
    Volume volume;
} Ext2;

typedef struct GroupDescriptorTable {
    unsigned int bg_block_bitmap;
    unsigned int bg_inode_bitmap;
    unsigned int bg_inode_table;
    unsigned short bg_free_blocks_count;
    unsigned short bg_free_inodes_count;
    unsigned short bg_used_dirs_count;
    unsigned short bg_pad;
} GroupDescriptorTable;

typedef struct InodeTable{
    unsigned int i_size;
    unsigned int i_blocks;
    unsigned int i_block[30];
} InodeTable;

typedef struct DirectoryEntryTable{
    unsigned int inode;
    unsigned short rec_len;
    char name_len;
    char file_type;
    char name[256];
}  DirectoryEntryTable;



Ext2 getExtInfo(FILE* fd);

void printExtInfo(Ext2 ext2);

void getInode(FILE* fd, Ext2 ext2);

void getBlock(FILE* fd, Ext2 ext2);

void getVolume(FILE* fd, Ext2 ext2);

unsigned int findExtFile(FILE* fp, char* filename, Ext2 ext2, int delete);

unsigned short recursiveExtFind(FILE* fd, char* filename, Ext2 ext2, unsigned int i_block, GroupDescriptorTable groupDescriptorTable, unsigned int size, int delete);

#endif //SHOOTER_EXT2UTILITIES_H
