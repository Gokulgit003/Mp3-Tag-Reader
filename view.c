#include "types.h"
#include "header.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>


void printDetails(char *tag_buffer, char *tag_data)
{
    if (strcmp("TPE1", tag_buffer) == 0)
    {
        printf("ARTIST: %s\n", tag_data);
    }

    if (strcmp("TIT2", tag_buffer) == 0)
    {
        printf("TITLE: %s\n", tag_data);
    }

    if (strcmp("TALB", tag_buffer) == 0)
    {
        printf("ALBUM: %s\n", tag_data);
    }

    if (strcmp("TYER", tag_buffer) == 0)
    {
        printf("YEAR: %s\n", tag_data);
    }

    if (strcmp("TCON", tag_buffer) == 0)
    {
        printf("MUSIC: %s\n", tag_data);
    }

    if (strcmp("COMM", tag_buffer) == 0)
    {
        printf("COMPOSER: %s\n", tag_data);
    }
}


Status view_procedure(char *filename)
{
    const char *strline = "----------------------------------------------------";
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Couldnt open %s\n", filename);
        return v_failure;
    }
    if(fseek(fp, 10, SEEK_SET)!=0)
    {
        printf("Error: Failed to skip the 10 bytes\n");
        return v_failure;
    }
    printf("\n---------------------SELECTED VIEW DETAILS---------------------\n");
    for(int i=0;i<6;i++)
    {
        // store tag
        char tag[5];
        // to store size
        char size_bytes[4];
        fread(tag, sizeof(char), 4, fp);
        tag[4]='\0';
        fread(size_bytes, 1, 4, fp);
        int size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | (size_bytes[3]);
        fseek(fp,3,SEEK_CUR);
        char *tag_data=malloc(size);
        fread(tag_data, 1,size-1,fp);
        tag_data[size-1]='\0';
        printDetails(tag, tag_data);
    }
    printf("\n%s\n", strline);
    printf("\n---------------------DETAILS DISPLAYED---------------------\n");
    fclose(fp);
    return v_success;
}
