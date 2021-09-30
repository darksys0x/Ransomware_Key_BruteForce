//#include <stdio.h>
//#include <stdint.h>
//#include <stdlib.h>
//#include <memory.h>
//
//uint8_t rotate_left(uint8_t num, int bits)
//{
//    return ((num << bits) | (num >> (8 - bits)));
//}
//
//void __cdecl decrypt_buffer(uint8_t* buffer, uint8_t* key)
//{
//    // THIS IS FROM IDA
//    for (char i = 0; i < 8; i++)
//    {
//        // 1. xor
//        // 2. rotate left
//        // 3. subtract - i 
//        buffer[i] = rotate_left(key[i] ^ buffer[i], i) - i;
//    }
//}
//
//uint32_t GetAlignedValue(uint32_t value, uint32_t alignment)
//{
//    return (value + alignment - 1) & ~(alignment - 1);
//}
//
//long GetFileSize(FILE* f)
//{
//    fseek(f, 0, SEEK_END); // seek to end of file
//    long size = ftell(f); // get current file pointer
//    fseek(f, 0, SEEK_SET); // seek back to beginning of file
//    return size;
//}
//
//void CreateOutputFile(const char* filename, uint8_t* data, uint32_t size)
//{
//    FILE* output_file = fopen(filename, "wb+");
//    if (output_file) {
//        fwrite(data, 1, size, output_file);
//        fclose(output_file);
//    }
//}
//
//int main() {
//    //const char* filename = "E:\\flare projects\\flare 2\\files\\ic_instagram.png.encrypted";
//    const char* filename = "E:\\flare projects\\flare 2\\02_known (1)\\Files\\latin_alphabet.txt.encrypted";
//    FILE* in_file = fopen(filename, "rb");
//    if (!in_file) {
//        printf("failed to open %s\n", filename);
//        return 0;
//    }
//    long fileSize = GetFileSize(in_file);
//
//    uint32_t alignedSize = GetAlignedValue(fileSize, 8);
//    printf("file size = %u | aligned Size = %u\n", fileSize, alignedSize);
//
//    uint8_t* fileData = (uint8_t*)malloc(alignedSize);
//
//    fread(fileData, fileSize, 1, in_file);
//
//    uint8_t key[8] = { 0x4e, 0x6f, 0x31, 0x54, 0x72, 0x75, 0x73, 0x74 };
//
//    uint8_t* data = fileData;
//    uint32_t blocks = alignedSize / 8;
//    for (uint32_t i = 0; i < blocks; i++) {
//        decrypt_buffer(data, key);
//        data += 8;
//    }
//
//    char outputFileName[300];
//    sprintf(outputFileName, "%s.decrypted", filename);
//
//    CreateOutputFile(outputFileName, fileData, fileSize);
//
//    free(fileData);
//    fclose(in_file);
//    getchar();
//    return 0;
//}