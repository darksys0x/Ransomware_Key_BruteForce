#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

uint8_t rotate_left(uint8_t num, int bits)
{
    return ((num << bits) | (num >> (8 - bits)));
}

void __cdecl decrypt_buffer(uint8_t* buffer, uint8_t* key)
{
    // THIS IS FROM IDA
    for (char i = 0; i < 8; i++)
    {
        // 1. xor
        // 2. rotate left
        // 3. subtract - i 
        buffer[i] = rotate_left(key[i] ^ buffer[i], i) - i;
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

// png header 8 bytes at top
const uint8_t png_header_bytes[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };

bool GetKeyFirst4Bytes(uint8_t* fileData, uint8_t* decryptedBuffer, uint8_t* key) {
    memcpy(decryptedBuffer, fileData, 8); // reset decryptedBuffer to original encrypted byte
    decrypt_buffer(decryptedBuffer, key);
    bool decrypted = true;
    for (int i = 0; i < 4; i++) {
        if (decryptedBuffer[i] != png_header_bytes[i]) {
            decrypted = false;
            break;
        }
    }
    if (decrypted) {
        printf("yes, buffer is decrypted!!! First 4 bytes of key:\n");
        printf("key = 0x%x, 0x%x, 0x%x, 0x%x\n",
            key[0], key[1], key[2], key[3]);
    }
    return decrypted;
}


bool GetKeyLast4Bytes(uint8_t* fileData, uint8_t* decryptedBuffer, uint8_t* key) {
    memcpy(decryptedBuffer, fileData, 8); // reset decryptedBuffer to original encrypted byte
    decrypt_buffer(decryptedBuffer, key);
    bool decrypted = true;
    for (int i = 4; i < 8; i++) {
        if (decryptedBuffer[i] != png_header_bytes[i]) {
            decrypted = false;
            break;
        }
    }
    if (decrypted) {
        printf("yes, buffer is decrypted!!! last 4 bytes of key:\n");
        printf("key = 0x%x, 0x%x, 0x%x, 0x%x\n",
            key[4], key[5], key[6], key[7]);
    }
    return decrypted;
}

void DecryptAndGetKeyFirst4Bytes(uint8_t* fileData, uint8_t* decryptedBuffer) {
    for (int a = 0; a < 256; a++) {
        uint8_t key[8] = { a,0,0,0, 0,0,0,0 };
        if (GetKeyFirst4Bytes(fileData, decryptedBuffer, key)) return;
        for (int b = 0; b < 256; b++) {
            uint8_t key[8] = { a,b,0,0, 0,0,0,0 };
            if (GetKeyFirst4Bytes(fileData, decryptedBuffer, key)) return;
            for (int c = 0; c < 256; c++) {
                uint8_t key[8] = { a,b,c,0, 0,0,0,0 };
                if (GetKeyFirst4Bytes(fileData, decryptedBuffer, key)) return;
                for (int d = 0; d < 256; d++) {
                    uint8_t key[8] = { a,b,c,d, 0,0,0,0 };
                    if (GetKeyFirst4Bytes(fileData, decryptedBuffer, key)) return;
                }
            }
        }
    }
}

void DecryptAndGetKeyLast4Bytes(uint8_t* fileData, uint8_t* decryptedBuffer) {
    for (int a = 0; a < 256; a++) {
        uint8_t key[8] = { 0,0,0,0, a,0,0,0 };
        if (GetKeyLast4Bytes(fileData, decryptedBuffer, key)) return;
        for (int b = 0; b < 256; b++) {
            uint8_t key[8] = { 0,0,0,0, a,b,0,0 };
            if (GetKeyLast4Bytes(fileData, decryptedBuffer, key)) return;
            for (int c = 0; c < 256; c++) {
                uint8_t key[8] = { 0,0,0,0, a,b,c,0 };
                if (GetKeyLast4Bytes(fileData, decryptedBuffer, key)) return;
                for (int d = 0; d < 256; d++) {
                    uint8_t key[8] = { 0,0,0,0, a,b,c,d };
                    if (GetKeyLast4Bytes(fileData, decryptedBuffer, key)) return;
                }
            }
        }
    }
}

int main() { 
    const char* filename = "E:\\flare projects\\flare 2\\02_known (1)\\Files\\capa.png.encrypted";
    FILE* in_file = fopen(filename, "rb");
    if (!in_file) {
        printf("failed to open %s\n", filename);
        return 0;
    }
    long fileSize = GetFileSize(in_file);

    uint32_t alignedSize = GetAlignedValue(fileSize, 8);
    printf("file size = %u | aligned Size = %u\n", fileSize, alignedSize);

    uint8_t* fileData = (uint8_t*)malloc(alignedSize);
    uint8_t* decryptedBuffer = (uint8_t*)malloc(alignedSize);

    fread(fileData, fileSize, 1, in_file);

    DecryptAndGetKeyFirst4Bytes(fileData, decryptedBuffer);
    DecryptAndGetKeyLast4Bytes(fileData, decryptedBuffer);

    free(fileData);
    fclose(in_file);
    getchar();
    return 0;
}