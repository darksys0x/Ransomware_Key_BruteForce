//#include<stdio.h>
//#include<stdint.h>
//
//
//uint8_t rotate_left(uint8_t num, int bits)
//{
//    return ((num << bits) | (num >> (8 - bits)));
//}
//
//uint8_t rotate_right(uint8_t num, int bits)
//{
//    return ((num >> bits) | (num << (8 - bits)));
//}
//
//
//void __cdecl  encrypt_buffer(uint8_t* buffer, uint8_t* key)
//{
//    for (char i = 0; i < 8; i++)
//    {
//        // 1. add + i
//        // 2. rotate right
//        // 3. xor
//        buffer[i] = key[i] ^ rotate_right(buffer[i] + i, i);
//    }
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
//
//
//void printfBuffer(char* buffer, int size) {
//    for (int i = 0; i < 8; i++) {
//        printf("%x ", (uint8_t)buffer[i]);
//    }
//    printf("\n");
//}
//
//int main() {
//
//    char buffer[8] = {0xFD, 0xFD, 0xFD, 0xFD, 
//                      0xFD, 0xFD, 0xFD, 0xFD};
//    char key[8] = { 'a', '8', 'g', 't', 'o','p', 't','d' };
//
//    encrypt_buffer((uint8_t*)buffer, (uint8_t*)key);
//    printf("encrypted buffer:\n");
//    printfBuffer(buffer, 8);
//
//    decrypt_buffer((uint8_t*)buffer, (uint8_t*)key);
//    printf("decrypted buffer:\n");
//    printfBuffer(buffer, 8);
//    getchar();
//    return 0;
//}