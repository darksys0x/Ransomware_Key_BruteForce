#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

uint8_t rotate_right(uint8_t num, int bits)
{
    return ((num >> bits) | (num << (8 - bits)));
}

void __cdecl encrypt_buffer(uint8_t* buffer, uint8_t* key)
{
    for (char i = 0; i < 8; i++)
    {
        // 1. add + i
        // 2. rotate right
        // 3. xor
        buffer[i] = key[i] ^ rotate_right(buffer[i] + i, i);
    }
}

uint32_t GetAlignedValue(uint32_t value, uint32_t alignment) 
{ 
    return (value + alignment - 1) & ~(alignment - 1); 
}

long GetFileSize(FILE* f)
{
    fseek(f, 0, SEEK_END); // seek to end of file
    long size = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file
    return size;
}

void CreateOutputFile(const char* filename, uint8_t* data, uint32_t size)
{
    FILE* output_file = fopen(filename, "wb+");
    if (output_file) {
        fwrite(data, 1, size, output_file);
        fclose(output_file);
    }
} 

int main() {
    const char* filename = "E:\\flare projects\\flare 2\\files\\ic_instagram.png";
    FILE* in_file = fopen(filename, "rb");
    if (!in_file) {
        printf("failed to open %s\n", filename);
        return 0;
    }
    long fileSize = GetFileSize(in_file);

    uint32_t alignedSize = GetAlignedValue(fileSize, 8);
    printf("file size = %u | aligned Size = %u\n", fileSize, alignedSize);

    uint8_t* fileData = (uint8_t*)malloc(alignedSize);

    fread(fileData, fileSize, 1, in_file);

    char key[8] = { 'a', '8', 'g', 't', 'o','p', 't','d' };

    uint8_t* data = fileData;
    uint32_t blocks = alignedSize / 8;
    for (uint32_t i = 0; i < blocks; i++) {
        encrypt_buffer(data, (uint8_t*)key);
        data += 8;
    }

    char outputFileName[300];
    sprintf(outputFileName, "%s.encrypted", filename);

    CreateOutputFile(outputFileName, fileData, fileSize);

    free(fileData);
    fclose(in_file);
    getchar();
    return 0;
}