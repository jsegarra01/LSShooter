//
// Created by josep on 10/04/2022.
//

#include "Ext2Utilities.h"

Ext2 getExtInfo(FILE* fd) {
    Ext2 ext2;

    //I do this so I don't get the error "error: ‘ext2’ is used uninitialized in this function"
    fseek(fd, 1024, SEEK_SET);
    fread(&ext2.inode.s_inodes_count, 4, 1, fd);

    //getInode(fd, ext2);
    //getBlock(fd, ext2);
    //getVolume(fd, ext2);

    fseek(fd, 84 + 1024, SEEK_SET);
    fread(&ext2.inode.s_first_ino, 4, 1, fd);
    fread(&ext2.inode.s_inode_size, 2, 1, fd);

    fseek(fd, 40 + 1024, SEEK_SET);
    fread(&ext2.inode.s_inodes_per_group, 4, 1, fd);

    fseek(fd, 16 + 1024, SEEK_SET);
    fread(&ext2.inode.s_free_inodes_count, 4, 1, fd);

    fseek(fd, 24 + 1024, SEEK_SET);
    fread(&ext2.block.s_log_block_size, 4, 1, fd);

    fseek(fd, 4 + 1024, SEEK_SET);
    fread(&ext2.block.s_blocks_count, 4, 1, fd);
    fread(&ext2.block.s_r_blocks_count, 4, 1, fd);
    fread(&ext2.block.s_free_blocks_count, 4, 1, fd);

    fseek(fd, 20 + 1024, SEEK_SET);
    fread(&ext2.block.s_first_data_block, 4, 1, fd);

    fseek(fd, 32 + 1024, SEEK_SET);
    fread(&ext2.block.s_blocks_per_group, 4, 1, fd);
    fread(&ext2.block.s_frags_per_group, 4, 1, fd);

    fseek(fd, 44 + 1024, SEEK_SET);
    fread(&ext2.volume.s_mtime, 4, 1, fd);

    fseek(fd, 120 + 1024, SEEK_SET);
    fread(ext2.volume.s_volume_name, 16, 1, fd);


    fseek(fd, 64 + 1024, SEEK_SET);
    fread(&ext2.volume.s_lastcheck, 4, 1, fd);

    fseek(fd, 48 + 1024, SEEK_SET);
    fread(&ext2.volume.s_wtime, 4, 1, fd);

    return ext2;
}

void getInode(FILE* fd, Ext2 ext2) {

    fseek(fd, 84 + 1024, SEEK_SET);
    fread(&ext2.inode.s_first_ino, 4, 1, fd);
    fread(&ext2.inode.s_inode_size, 2, 1, fd);

    fseek(fd, 40 + 1024, SEEK_SET);
    fread(&ext2.inode.s_inodes_per_group, 4, 1, fd);

    fseek(fd, 16 + 1024, SEEK_SET);
    fread(&ext2.inode.s_free_inodes_count, 4, 1, fd);
}

void getBlock(FILE* fd, Ext2 ext2) {
    fseek(fd, 24 + 1024, SEEK_SET);
    fread(&ext2.block.s_log_block_size, 4, 1, fd);

    fseek(fd, 4 + 1024, SEEK_SET);
    fread(&ext2.block.s_blocks_count, 4, 1, fd);
    fread(&ext2.block.s_r_blocks_count, 4, 1, fd);
    fread(&ext2.block.s_free_blocks_count, 4, 1, fd);

    fseek(fd, 20 + 1024, SEEK_SET);
    fread(&ext2.block.s_first_data_block, 4, 1, fd);

    fseek(fd, 32 + 1024, SEEK_SET);
    fread(&ext2.block.s_blocks_per_group, 4, 1, fd);
    fread(&ext2.block.s_frags_per_group, 4, 1, fd);
}

void getVolume(FILE* fd, Ext2 ext2) {
    fseek(fd, 44 + 1024, SEEK_SET);
    fread(ext2.volume.s_volume_name, 16, 1, fd);
    fread(&ext2.volume.s_mtime, 4, 1, fd);

    fseek(fd, 64 + 1024, SEEK_SET);
    fread(&ext2.volume.s_lastcheck, 4, 1, fd);

    fseek(fd, 48 + 1024, SEEK_SET);
    fread(&ext2.volume.s_wtime, 4, 1, fd);
}

void printExtInfo(Ext2 ext2) {
    printf("INFO INODE Inode\n"
           "Size: %hu\n"
           "Num Inodes: %d\n"
           "First Inode: %d\n"
           "Inodes Group: %d\n"
           "Free Inodes: %d\n\n"
           "INFO BLOCK\n"
           "Size Block: %d\n"
           "Reserved blocks: %d\n"
           "Free blocks: %d\n"
           "Total blocks: %d\n"
           "First block: %d\n"
           "Group blocks: %d\n"
           "Group blocks: %d\n\n"
           "INFO VOLUME\n"
           "Volume Name: %s\n"
           , ext2.inode.s_inode_size, ext2.inode.s_inodes_count, ext2.inode.s_first_ino, ext2.inode.s_inodes_per_group, ext2.inode.s_free_inodes_count,
           1024 << ext2.block.s_log_block_size, ext2.block.s_r_blocks_count, ext2.block.s_free_blocks_count, ext2.block.s_blocks_count, ext2.block.s_first_data_block, ext2.block.s_blocks_per_group, ext2.block.s_frags_per_group,
           ext2.volume.s_volume_name);
    timeToDate("Last Mounted: ", ext2.volume.s_mtime);
    timeToDate("Last Written: ", ext2.volume.s_wtime);
    timeToDate("Last Checked: ", ext2.volume.s_lastcheck);
}

unsigned int findExtFile(FILE* fd, char* filename, Ext2 ext2, int delete) {
    GroupDescriptorTable groupDescriptorTable;
    InodeTable inodeTable;
    unsigned int size = 0;

    if((1024 << ext2.block.s_log_block_size) == 1024) {
        fseek(fd, 2048, SEEK_SET);
        fread(&groupDescriptorTable, sizeof(GroupDescriptorTable), 1, fd);
    }
    else {
        fseek(fd, ext2.block.s_log_block_size, SEEK_SET);
        fread(&groupDescriptorTable, sizeof(GroupDescriptorTable), 1, fd);
    }

    //fseek(fd, 2048, SEEK_SET);
    //fread(&groupDescriptorTable, sizeof(GroupDescriptorTable), 1, fd);

    fseek(fd, groupDescriptorTable.bg_inode_table * (1024 << ext2.block.s_log_block_size) + 132, SEEK_SET);
    fread(&inodeTable.i_size, sizeof (unsigned int), 1, fd);

    fseek(fd, 20, SEEK_CUR);
    fread(&inodeTable.i_blocks, sizeof (unsigned int), 1, fd);

    //printf("block size: %d, iblocks: %d\n", 2 << ext2.block.s_log_block_size, inodeTable.i_blocks);

    fseek(fd, 8, SEEK_CUR);
    for (unsigned int i = 0; i < (inodeTable.i_blocks / (2 << ext2.block.s_log_block_size)); ++i) {
        fread(&inodeTable.i_block[i], sizeof (unsigned int), 1, fd);
    }

    for (unsigned int i = 0; i < (inodeTable.i_blocks / (2 << ext2.block.s_log_block_size)); ++i) {
        size = recursiveExtFind(fd, filename, ext2, inodeTable.i_block[i], groupDescriptorTable, inodeTable.i_size, delete);
        if(size != 0) return size;
    }
    return size;
}

unsigned short recursiveExtFind(FILE* fd, char* filename, Ext2 ext2, unsigned int i_block, GroupDescriptorTable groupDescriptorTable, unsigned int size, int delete) {
    DirectoryEntryTable directoryEntryTable[20];
    unsigned int auxSize = 0;
    unsigned short total_rec_len = 0;
    int i = 0;

    fseek(fd, i_block * (1024 << ext2.block.s_log_block_size) , SEEK_SET);

    while (total_rec_len < (1024 << ext2.block.s_log_block_size)) {
        fread(&directoryEntryTable[i].inode, sizeof (unsigned int), 1, fd);
        fread(&directoryEntryTable[i].rec_len, sizeof (unsigned short), 1, fd);
        fread(&directoryEntryTable[i].name_len, sizeof (char), 1, fd);
        fread(&directoryEntryTable[i].file_type, sizeof (char), 1, fd);
        fread(&directoryEntryTable[i].name, directoryEntryTable[i].name_len, 1, fd);
        directoryEntryTable[i].name[(int) directoryEntryTable[i].name_len] = '\0';

        total_rec_len = total_rec_len + directoryEntryTable[i].rec_len;

        fseek(fd, total_rec_len + i_block * (1024 << ext2.block.s_log_block_size), SEEK_SET);
        //printf("Reading from: %d\n", total_rec_len + i_block * 1024);
        //printf("2n: %hu, i: %d, name: %s, type: %hu\n", total_rec_len, i, directoryEntryTable[i].name, directoryEntryTable[i].file_type);

        if(directoryEntryTable[i].file_type == 1) {
            if(!strcmp(directoryEntryTable[i].name, filename)) {
                if (delete == 0) {
                    int offset = groupDescriptorTable.bg_inode_table * (1024 << ext2.block.s_log_block_size) + (1024 << ext2.block.s_log_block_size) * ext2.block.s_blocks_per_group * ((directoryEntryTable[i].inode -1) / ext2.inode.s_inodes_per_group);
                    directoryEntryTable[i].inode = directoryEntryTable[i].inode % ext2.inode.s_inodes_per_group;

                    fseek(fd, offset + (directoryEntryTable[i].inode * 128) + 4 -128, SEEK_SET);
                    fread(&size, sizeof (unsigned int), 1, fd);
                    printf("File found, it is %d bytes in size.\n", size);
                }
                else {
                    if (i > 0) {

                        unsigned short auxRecLen = directoryEntryTable[i-1].rec_len;
                        directoryEntryTable[i-1].rec_len += directoryEntryTable[i].rec_len;
                        directoryEntryTable[i].inode = 0x00;
                        directoryEntryTable[i].rec_len = 0x00;
                        directoryEntryTable[i].file_type = 0x00;

                        for (int j = 0; j < directoryEntryTable[i].name_len; j++) {
                            directoryEntryTable[i].name[j] = 0x00;
                        }

                        unsigned short name_len = directoryEntryTable[i].name_len;
                        directoryEntryTable[i].name_len = 0x00;

                        int offset = 0;
                        for(int k = 0; k < (i -1); k++) {
                            offset += directoryEntryTable[k].rec_len;
                        }

                        fseek(fd, i_block * (1024 << ext2.block.s_log_block_size) + offset + 4, SEEK_SET);
                        fwrite(&directoryEntryTable[i-1].rec_len, sizeof (unsigned short), 1, fd);
                        offset += auxRecLen;

                        fseek(fd, i_block * (1024 << ext2.block.s_log_block_size) + offset, SEEK_SET);
                        fwrite(&directoryEntryTable[i].inode, sizeof(unsigned int) , 1, fd);
                        fwrite(&directoryEntryTable[i].rec_len, sizeof (unsigned short), 1, fd);
                        fwrite(&directoryEntryTable[i].name_len, sizeof (char), 1, fd);
                        fwrite(&directoryEntryTable[i].file_type, sizeof (char), 1, fd);
                        fwrite(&directoryEntryTable[i].name, name_len, 1, fd);

                    }
                    else {
                        directoryEntryTable[i+1].rec_len += directoryEntryTable[i].rec_len;

                        directoryEntryTable[i].rec_len = 0x00;
                        directoryEntryTable[i].inode = 0x00;
                        directoryEntryTable[i].rec_len = 0x00;
                        directoryEntryTable[i].file_type = 0x00;

                        for (int j = 0; j < directoryEntryTable[i].name_len; j++) {
                            directoryEntryTable[i].name[j] = 0x00;
                        }

                        unsigned short name_len = directoryEntryTable[i].name_len;
                        directoryEntryTable[i].name_len = 0x00;


                        int offset = 0;
                        for(int k = 0; k < i; k++) {
                            offset += directoryEntryTable[k].rec_len;
                        }

                        fseek(fd, i_block * (1024 << ext2.block.s_log_block_size) + offset, SEEK_SET);
                        fwrite(&directoryEntryTable[i+1].inode, sizeof(unsigned int) , 1, fd);
                        fwrite(&directoryEntryTable[i+1].rec_len, sizeof (unsigned short), 1, fd);
                        fwrite(&directoryEntryTable[i+1].name_len, sizeof (char), 1, fd);
                        fwrite(&directoryEntryTable[i+1].file_type, sizeof (char), 1, fd);
                        fwrite(&directoryEntryTable[i+1].name, directoryEntryTable[i+1].name_len, 1, fd);

                        fwrite(&directoryEntryTable[i].inode, sizeof(unsigned int) , 1, fd);
                        fwrite(&directoryEntryTable[i].rec_len, sizeof (unsigned short), 1, fd);
                        fwrite(&directoryEntryTable[i].name_len, sizeof (char), 1, fd);
                        fwrite(&directoryEntryTable[i].file_type, sizeof (char), 1, fd);
                        fwrite(&directoryEntryTable[i].name, name_len, 1, fd);

                    }
                    printf("File removed succesfully\n");
                }
                return 1;
            }
        }

        i++;
    }

    for (int j = 0; j < i; j++) {
        if(directoryEntryTable[j].file_type == 2 && strcmp(directoryEntryTable[j].name, ".") != 0 && strcmp(directoryEntryTable[j].name, "..") != 0) {
            InodeTable inodeTable;

            int offset = groupDescriptorTable.bg_inode_table * (1024 << ext2.block.s_log_block_size) + (1024 << ext2.block.s_log_block_size) * ext2.block.s_blocks_per_group * ((directoryEntryTable[j].inode -1) / ext2.inode.s_inodes_per_group);
            directoryEntryTable[j].inode = directoryEntryTable[j].inode % ext2.inode.s_inodes_per_group;

            fseek(fd, offset + (directoryEntryTable[j].inode * 128) + 4 -128, SEEK_SET);
            //printf("offset of %s in: %d\n", directoryEntryTable[j].name, offset + (directoryEntryTable[j].inode * 128) + 4 -128);
            fread(&inodeTable.i_size, sizeof (unsigned int), 1, fd);

            fseek(fd, 20, SEEK_CUR);
            fread(&inodeTable.i_blocks, sizeof (unsigned int), 1, fd);

            fseek(fd, 8, SEEK_CUR);
            for (unsigned int k = 0; k < (inodeTable.i_blocks /( 2 << ext2.block.s_log_block_size)); k++) {
                fread(&inodeTable.i_block[k], sizeof (unsigned int), 1, fd);
            }
            //printf("block size: %d, iblocks: %d\n", 2 << ext2.block.s_log_block_size, inodeTable.i_blocks);

            for (unsigned int k = 0; k < (inodeTable.i_blocks / (2 << ext2.block.s_log_block_size)); k++) {
                auxSize = recursiveExtFind(fd, filename, ext2, inodeTable.i_block[k], groupDescriptorTable, inodeTable.i_size, delete);
                if(auxSize != 0) return auxSize;
            }
        }
    }

    return 0;
}

