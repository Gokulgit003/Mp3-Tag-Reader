#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "types.h"
#include "header.h"

static const char *get_frame_id(char *opt)
{
    if (strcmp(opt, "-t") == 0) return "TIT2"; // Title
    if (strcmp(opt, "-a") == 0) return "TPE1"; // Artist
    if (strcmp(opt, "-A") == 0) return "TALB"; // Album
    if (strcmp(opt, "-y") == 0) return "TYER"; // Year
    if (strcmp(opt, "-m") == 0) return "TCON"; // Music/Genre
    if (strcmp(opt, "-c") == 0) return "COMM"; // Comment
    return NULL;
}

Status editFile(char *argv[])
{
    if (!argv[2] || !argv[3] || !argv[4])
    {
        printf("Usage: ./mp3tool -e -t \"New Title\" song.mp3\n");
        return e_failure;
    }

    const char *frame_to_edit = get_frame_id(argv[2]);
    if (!frame_to_edit)
    {
        printf("Unsupported edit option: %s\n", argv[2]);
        return e_failure;
    }

    FILE *fp_src  = fopen(argv[4], "rb");
    FILE *fp_dest = fopen("test.mp3", "wb");

    if (!fp_src || !fp_dest)
    {
        printf("Error opening files\n");
        return e_failure;
    }

    // Copy ID3 header (10 bytes)
    char header[10];
    fread(header, 1, 10, fp_src);
    fwrite(header, 1, 10, fp_dest);

    // Process first 6 frames
    for (int i = 0; i < 6; i++)
    {
        char tag[5];
        char size_bytes[4];
        char flags[2];

        fread(tag, 1, 4, fp_src);
        tag[4] = '\0';

        fread(size_bytes, 1, 4, fp_src);
        fread(flags, 1, 2, fp_src);

        int size = (size_bytes[0] << 24) |
                   (size_bytes[1] << 16) |
                   (size_bytes[2] << 8)  |
                   (size_bytes[3]);

        char *data = malloc(size);
        fread(data, 1, size, fp_src);

        // If this is the frame user wants to edit
        if (strcmp(tag, frame_to_edit) == 0)
        {
            printf("Editing frame %s...\n", tag);

            const char *new_text = argv[3];
            int new_size = strlen(new_text) + 1;  // +1 for encoding byte

            // Write tag
            fwrite(tag, 1, 4, fp_dest);

            // Write new size
            char new_size_bytes[4];
            new_size_bytes[0] = (new_size >> 24) & 0xFF;
            new_size_bytes[1] = (new_size >> 16) & 0xFF;
            new_size_bytes[2] = (new_size >> 8)  & 0xFF;
            new_size_bytes[3] = (new_size)       & 0xFF;
            fwrite(new_size_bytes, 1, 4, fp_dest);

            // Write flags
            fwrite(flags, 1, 2, fp_dest);

            // Write encoding byte + new text
            fputc(0x00, fp_dest);          // ISO-8859-1 encoding
            fwrite(new_text, 1, strlen(new_text), fp_dest);
        }
        else
        {
            // Copy frame as-is
            fwrite(tag, 1, 4, fp_dest);
            fwrite(size_bytes, 1, 4, fp_dest);
            fwrite(flags, 1, 2, fp_dest);
            fwrite(data, 1, size, fp_dest);
        }

        free(data);
    }

    // Copy remaining audio data
    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), fp_src)) > 0)
    {
        fwrite(buffer, 1, bytes, fp_dest);
    }

    fclose(fp_src);
    fclose(fp_dest);

    //printf("Edit completed successfully. Output file: test.mp3\n");
    return e_success;
}
