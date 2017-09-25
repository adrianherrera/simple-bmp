/*
 * simple_bmp.c
 *
 * A simple BMP parser to test S2E.
 *
 * Copyright (C) Adrian Herrera, 2017
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef S2E
#include <s2e/s2e.h>
#endif

typedef enum {
    BI_RGB = 0,
    BI_RLE8,
    BI_RLE4,
    BI_BITFIELDS,
    BI_JPEG,
    BI_PNG,

    BI_END,
} bmp_compression_method_t;

int main(int argc, char *argv[]) {
    FILE *file = NULL;

    uint8_t header_magic[2];
    uint32_t header_filesz;
    uint16_t header_creator1;
    uint16_t header_creator2;
    uint32_t header_offset;

    uint32_t dib_header_sz;
    uint32_t dib_width;
    uint32_t dib_height;
    uint16_t dib_nplanes;
    uint16_t dib_depth;
    uint32_t dib_compress_type;
    uint32_t dib_bmp_bytesz;
    uint32_t dib_hres;
    uint32_t dib_vres;
    uint32_t dib_ncolors;
    uint32_t dib_nimpcolors;

    uint8_t *raw_image_data = NULL;

    if (argc != 2) {
#ifdef S2E
        s2e_printf("USAGE: %s /path/to/bmp\n", argv[0]);
#else
        fprintf(stderr, "USAGE: %s /path/to/bmp\n", argv[0]);
#endif
        goto error;
    }

    if (!(file = fopen(argv[1], "rb"))) {
#ifdef S2E
        s2e_printf("ERROR: failed to open %s\n", argv[1]);
#else
        fprintf(stderr, "ERROR: failed to open %s\n", argv[1]);
#endif
        goto error;
    }

    /* Parse the file header */
    if (fread(header_magic, sizeof(uint8_t), 2, file) != 2) {
#ifdef S2E
        s2e_message("ERROR: failed to read header magic\n");
#else
        fprintf(stderr, "ERROR: failed to read header magic\n");
#endif
        goto error;
    }

    if (header_magic[0] != 'B' || header_magic[1] != 'M') {
#ifdef S2E
        s2e_message("ERROR: BMP magic is incorrect\n");
#else
        fprintf(stderr, "ERROR: BMP magic is incorrect\n");
#endif
        goto error;
    }

    if (fread(&header_filesz, sizeof(header_filesz), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read header file size\n");
#else
        fprintf(stderr, "ERROR: failed to read header file size\n");
#endif
        goto error;
    }

    if (fread(&header_creator1, sizeof(header_creator1), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read header creator1\n");
#else
        fprintf(stderr, "ERROR: failed to read header creator1\n");
#endif
        goto error;
    }

    if (fread(&header_creator2, sizeof(header_creator2), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read header creator2\n");
#else
        fprintf(stderr, "ERROR: failed to read header creator2\n");
#endif
        goto error;
    }

    if (fread(&header_offset, sizeof(header_offset), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read header offset\n");
#else
        fprintf(stderr, "ERROR: failed to read header offset\n");
#endif
        goto error;
    }

    /* Parse the DIB header */
    if (fread(&dib_header_sz, sizeof(dib_header_sz), 1, file) != 1) {
#ifdef S2E
        fprintf(stderr, "ERROR: failed to read DIB header size\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB header size\n");
#endif
        goto error;
    }

    if (fread(&dib_width, sizeof(dib_width), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB width\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB width\n");
#endif
        goto error;
    }

    if (fread(&dib_height, sizeof(dib_height), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB height\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB height\n");
#endif
        goto error;
    }

    if (fread(&dib_nplanes, sizeof(dib_nplanes), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB number of planes\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB number of planes\n");
#endif
        goto error;
    }

    if (fread(&dib_depth, sizeof(dib_depth), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB depth\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB depth\n");
#endif
        goto error;
    }

    if (fread(&dib_compress_type, sizeof(dib_compress_type), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB compression type\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB compression type\n");
#endif
        goto error;
    }

    if (dib_compress_type > BI_END) {
#ifdef S2E
        s2e_printf("ERROR: invalid compress type: %u\n", dib_compress_type);
#else
        fprintf(stderr, "ERROR: invalid compress type: %u\n", dib_compress_type);
#endif
        goto error;
    }

    if (fread(&dib_bmp_bytesz, sizeof(dib_bmp_bytesz), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB image size\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB image size\n");
#endif
        goto error;
    }

    if (fread(&dib_hres, sizeof(dib_hres), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB horizontal resolution\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB horizontal resolution\n");
#endif
        goto error;
    }

    if (fread(&dib_vres, sizeof(dib_vres), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB vertical resolution\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB vertical resolution\n");
#endif
        goto error;
    }

    if (fread(&dib_ncolors, sizeof(dib_ncolors), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB number of colors\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB number of colors\n");
#endif
        goto error;
    }

    if (fread(&dib_nimpcolors, sizeof(dib_nimpcolors), 1, file) != 1) {
#ifdef S2E
        s2e_message("ERROR: failed to read DIB number of important colors\n");
#else
        fprintf(stderr, "ERROR: failed to read DIB number of important colors\n");
#endif
        goto error;
    }

#ifdef S2E
    s2e_message("Finished parsing BMP headers");
#endif

    /* Allocate memory for the image data */
    raw_image_data = (uint8_t*) malloc(sizeof(uint8_t) * dib_bmp_bytesz);
    if (!raw_image_data) {
#ifdef S2E
        s2e_message("ERROR: failed to allocate memory for image data\n");
#else
        fprintf(stderr, "ERROR: failed to allocate memory for image data\n");
#endif
        goto error;
    }

    /* Read the actual image data */
    if (fseek(file, header_offset, SEEK_SET)) {
#ifdef S2E
        s2e_message("ERROR: failed to seek to image data\n");
#else
        fprintf(stderr, "ERROR: failed to seek to image data\n");
#endif
        goto error;
    }

    if (fread(raw_image_data, sizeof(uint8_t), dib_bmp_bytesz, file) != dib_bmp_bytesz) {
#ifdef S2E
        s2e_message("ERROR: failed to read raw image data\n");
#else
        fprintf(stderr, "ERROR: failed to read raw image data\n");
#endif
        goto error;
    }

    fclose(file);

#ifndef S2E
    printf("BMP INFO\n");
    printf("========\n\n");

    printf("File header\n");
    printf("***********\n");
    printf("    magic:  %c%c\n", header_magic[0], header_magic[1]);
    printf("file size:  %u\n", header_filesz);
    printf("creator 1:  %u\n", header_creator1);
    printf("creator 2:  %u\n", header_creator2);
    printf("   offset:  %u\n\n", header_offset);

    printf("DIB header\n");
    printf("**********\n");
    printf("          header size:  %u\n", dib_header_sz);
    printf("                width:  %u\n", dib_width);
    printf("               height:  %u\n", dib_height);
    printf("           no. planes:  %u\n", dib_nplanes);
    printf("                depth:  %u\n", dib_depth);
    printf("     compression type:  %u\n", dib_compress_type);
    printf("           image size:  %u\n", dib_bmp_bytesz);
    printf("horizontal resolution:  %u\n", dib_hres);
    printf("  vertical resolution:  %u\n", dib_vres);
    printf("           no. colors:  %u\n", dib_ncolors);
    printf(" no. important colors:  %u\n", dib_nimpcolors);
#endif

    return 0;

error:
    if (raw_image_data) {
        free(raw_image_data);
    }

    if (file) {
        fclose(file);
    }

    return 1;
}
