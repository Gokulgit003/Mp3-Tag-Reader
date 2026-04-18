#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "header.h"

OperationType check_operation_type(char *argv[])
{
    if (strcmp(argv[1], "-v") == 0)
    {
        return view;
    }
    else if (strcmp(argv[1], "-e") == 0)
    {
        return edit;
    }
    else
    {
        return unsupported;
    }
}

void viewFile(char filename[])
{
    const char *strline = "----------------------------------------------------";
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Couldnt open %s\n", filename);
        return;
    }
    fseek(fp, 10, SEEK_SET);
    int i = 0;
    printf("\n---------------------SELECTED VIEW DETAILS---------------------\n");
    while (i < 6)
    {
        // store tag
        char tag_buffer[4];
        // to store size
        int size;
        char size_bytes[4];
        fread(tag_buffer, sizeof(char), 4, fp);

        fread(size_bytes, 1, 4, fp);
        size = (size_bytes[0] << 24) | (size_bytes[1] << 16) | (size_bytes[2] << 8) | (size_bytes[3]);
        size = size - 1;

        char flag[3];
        fread(flag, 1, 3, fp);
        char *tag_data = malloc(size);
        fread(tag_data, sizeof(char), size, fp);
        tag_data[size] = '\0';
        printDetails(tag_buffer, tag_data);
        free(tag_data);
        i++;
    }
    printf("\n%s\n", strline);
    printf("\n---------------------DETAILS DISPLAYED---------------------\n");
}

int main(int argc, char *argv[])
{
    const char *strline = "----------------------------------------------------";
    OperationType res = check_operation_type(argv);
    char *filename=argv[2];
    if (res == view)
    {
        if(view_procedure(filename)==v_success)
        {
            printf("View Part Successfully Completed\n");
        }
    }
    else if (res == edit)
    {
        if(editFile(argv)==e_success)
        {
            printf("Edit part Completed Successfully\n");
        }
    }
    else
    {
        printf("Unsupported Option\n");
    }
}
