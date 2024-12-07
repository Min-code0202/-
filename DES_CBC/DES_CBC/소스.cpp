#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// �ʱ� ġȯ ���̺� (IP)
int IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// ���� ġȯ ���̺� (FP)
int FP[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// S-�ڽ�: DES���� 6��Ʈ�� 4��Ʈ�� ���̴� �ٽ� ��ȯ ���̺�
// S-�ڽ� ���� (8���� 4x16 S-�ڽ�)
int S_BOX[8][4][16] = {
    // S1
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    // S2
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    // S3
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    // S4
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    // S5
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    // S6
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    // S7
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    // S8
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

// Ȯ�� �Լ� E (32��Ʈ -> 48��Ʈ Ȯ��)
int E[] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1
};

// S-�ڽ� ��ȯ �Լ�
// �Էµ� 48��Ʈ�� 32��Ʈ�� �����ϴ� ����
void sbox(int* input, int* output) {
    for (int i = 0; i < 8; i++) {
        // �� 6��Ʈ ����� ����� S-�ڽ����� ���� ã�Ƽ� 4��Ʈ�� ��ȯ
        int row = (input[i * 6] << 1) | input[i * 6 + 5];  // ��: ù ��Ʈ�� ������ ��Ʈ�� ���
        int col = (input[i * 6 + 1] << 3) | (input[i * 6 + 2] << 2) | (input[i * 6 + 3] << 1) | input[i * 6 + 4];  // ��: �߰� 4��Ʈ�� ���
        int sbox_value = S_BOX[i][row][col];
        // 4��Ʈ ����� �ٽ� �迭�� ����
        for (int j = 0; j < 4; j++) {
            output[i * 4 + j] = (sbox_value >> (3 - j)) & 0x01;  // S-�ڽ� ����� 4��Ʈ�� ��ȯ
        }
    }
}

// Feistel �Լ�: DES�� �ٽ� �κ�. ���� 32��Ʈ�� Ȯ���ϰ� S-�ڽ��� ������� ���� 32��Ʈ�� XOR.
void feistel(int* left, int* right, int* round_key) {
    int expanded_right[48];  // ���� 32��Ʈ�� 48��Ʈ�� Ȯ��
    int sbox_output[32];    // S-�ڽ��� ��ģ �� 32��Ʈ ���

    // Ȯ�� �Լ� E, 32��Ʈ�� 48��Ʈ�� Ȯ��
    for (int i = 0; i < 48; i++) {
        expanded_right[i] = right[E[i] - 1];
    }

    // Ȯ��� ���� 48��Ʈ�� ���� Ű�� XOR
    for (int i = 0; i < 48; i++) {
        expanded_right[i] ^= round_key[i];
    }

    // S-�ڽ��� ����Ͽ� 48��Ʈ�� 32��Ʈ�� ����
    sbox(expanded_right, sbox_output);

    // ���� 32��Ʈ�� XOR�Ͽ� ��� �ݿ�
    for (int i = 0; i < 32; i++) {
        left[i] ^= sbox_output[i];
    }
}

// ���� Ű ���� �Լ�
void generate_round_key(int* master_key, int* round_key, int round) {
    // Ű�� �����Ͽ� ���
    for (int i = 0; i < 48; i++) {
        round_key[i] = master_key[i];  //48��Ʈ�� ���
    }
}

// �ʱ� ġȯ �Լ� (IP ���̺��� ���)
void initial_permutation(int* data) {
    int temp[64];
    for (int i = 0; i < 64; i++) {
        temp[i] = data[IP[i] - 1];
    }
    for (int i = 0; i < 64; i++) {
        data[i] = temp[i];
    }
}

// ���� ġȯ �Լ� (FP ���̺��� ���)
void final_permutation(int* data) {
    int temp[64];
    for (int i = 0; i < 64; i++) {
        temp[i] = data[FP[i] - 1];
    }
    for (int i = 0; i < 64; i++) {
        data[i] = temp[i];
    }
}


// DES ��ȣȭ �Լ�
// ��(64��Ʈ)�� ��ȣ��(64��Ʈ)���� ��ȯ
void des_encrypt(int* plaintext, int* ciphertext, int* key) {
    // �ʱ� ġȯ
    initial_permutation(plaintext);

    // 16 ���� Feistel ��Ʈ��ũ
    int left[32], right[32];
    for (int i = 0; i < 32; i++) {
        left[i] = plaintext[i];
        right[i] = plaintext[i + 32];
    }

    // 16 ���� Feistel ����
    for (int round = 0; round < 16; round++) {
        int round_key[48];
        generate_round_key(key, round_key, round); // �� ���帶�� Ű ����
        feistel(left, right, round_key); // Feistel �Լ� ����

        // ������ ���带 �����ϰ� �¿� ����
        if (round != 15) {
            int temp[32];
            memcpy(temp, left, sizeof(temp));
            memcpy(left, right, sizeof(left));
            memcpy(right, temp, sizeof(right));
        }
    }

    // �¿� ���� �� ���� ġȯ
    for (int i = 0; i < 32; i++) {
        plaintext[i] = left[i];
        plaintext[i + 32] = right[i];
    }

    // ���� ġȯ
    final_permutation(plaintext);

    // ��ȣ���� ��� �迭�� ����
    memcpy(ciphertext, plaintext, 64 * sizeof(int));
}

// DES ��ȣȭ �Լ� (��ȣȭ�� �ݴ� ����)
void des_decrypt(int* ciphertext, int* plaintext, int* key) {
    // �ʱ� ġȯ
    initial_permutation(ciphertext);

    int left[32], right[32];
    for (int i = 0; i < 32; i++) {
        left[i] = ciphertext[i];
        right[i] = ciphertext[i + 32];
    }

    // 16 ���带 �������� ���� (��ȣȭ)
    for (int round = 15; round >= 0; round--) {
        int round_key[48];
        generate_round_key(key, round_key, round); // ���� Ű ����
        feistel(left, right, round_key); // Feistel �Լ� ����

        // ������ ���带 �����ϰ� �¿� ����
        if (round != 0) {
            int temp[32];
            memcpy(temp, left, sizeof(temp));
            memcpy(left, right, sizeof(left));
            memcpy(right, temp, sizeof(right));
        }
    }

    // �¿� ���� �� ���� ġȯ
    for (int i = 0; i < 32; i++) {
        ciphertext[i] = left[i];
        ciphertext[i + 32] = right[i];
    }

    // ���� ġȯ
    final_permutation(ciphertext);

    // ���� ��� �迭�� ����
    memcpy(plaintext, ciphertext, 64 * sizeof(int));
}

// �е� �߰� �Լ� (PKCS5/7)
int add_padding(unsigned char* buffer, int length) {
    int pad_value = 8 - (length % 8); // ���� ����Ʈ�� ä�� ��
    for (int i = length; i < length + pad_value; i++) {
        buffer[i] = pad_value;
    }
    return length + pad_value; // ���� �е��� ��ü ���� ��ȯ
}

// �е� ���� �Լ� (PKCS5/7)
int remove_padding(unsigned char* buffer, int length) {
    int pad_value = buffer[length - 1];
    return length - pad_value; // �е� ���� �� ���� ������ ���� ��ȯ
}

// IV ���� �Լ� �߰�
void generate_iv(int* iv) {
    srand((unsigned int)time(NULL));
    for (int i = 0; i < 64; i++) {
        iv[i] = rand() % 2;  // 0 �Ǵ� 1�� ������ ��Ʈ ����
    }
}

// IV�� ���Ͽ� �����ϴ� �Լ�
void save_iv(FILE* file, int* iv) {
    unsigned char iv_bytes[8];
    for (int i = 0; i < 8; i++) {
        iv_bytes[i] = 0;
        for (int j = 0; j < 8; j++) {
            iv_bytes[i] |= (iv[i * 8 + j] << (7 - j));
        }
    }
    fwrite(iv_bytes, 1, 8, file);
}

// IV�� ���Ͽ��� �д� �Լ�
void read_iv(FILE* file, int* iv) {
    unsigned char iv_bytes[8];
    fread(iv_bytes, 1, 8, file);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            iv[i * 8 + j] = (iv_bytes[i] >> (7 - j)) & 0x01;
        }
    }
}

// XOR ���� �Լ� (64��Ʈ)
void xor_blocks(int* block1, int* block2, int* result) {
    for (int i = 0; i < 64; i++) {
        result[i] = block1[i] ^ block2[i];
    }
}

// CBC ���� ������ ���� ��ȣȭ �Լ�
void encrypt_file(const char* input_file, const char* output_file, int* key) {
    FILE* in = fopen(input_file, "rb");
    FILE* out = fopen(output_file, "wb");

    if (!in || !out) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }

    // IV ���� �� ����
    int iv[64];
    generate_iv(iv);
    save_iv(out, iv);

    unsigned char buffer[8];
    int previous_block[64];  // ���� ��ȣ�� ���
    memcpy(previous_block, iv, sizeof(iv));  // ó������ IV�� ���

    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, 8, in)) > 0) {
        if (bytes_read < 8) {
            bytes_read = add_padding(buffer, bytes_read);
        }

        // �� ����� ��Ʈ �迭�� ��ȯ
        int plaintext[64];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                plaintext[i * 8 + j] = (buffer[i] >> (7 - j)) & 0x01;
            }
        }

        // CBC ���: ���� ��ȣ�� ��ϰ� XOR
        int xored_block[64];
        xor_blocks(plaintext, previous_block, xored_block);

        // DES ��ȣȭ
        int ciphertext[64];
        des_encrypt(xored_block, ciphertext, key);

        // ���� ��ȣ���� ���� ���带 ���� ����
        memcpy(previous_block, ciphertext, sizeof(ciphertext));

        // ��ȣ���� ����Ʈ�� ��ȯ�Ͽ� ���Ͽ� ����
        for (int i = 0; i < 8; i++) {
            buffer[i] = 0;
            for (int j = 0; j < 8; j++) {
                buffer[i] |= (ciphertext[i * 8 + j] << (7 - j));
            }
        }

        fwrite(buffer, 1, 8, out);
    }

    fclose(in);
    fclose(out);
}

// CBC ���� ������ ���� ��ȣȭ �Լ�
void decrypt_file(const char* input_file, const char* output_file, int* key) {
    FILE* in = fopen(input_file, "rb");
    FILE* out = fopen(output_file, "wb");

    if (!in || !out) {
        printf("������ �� �� �����ϴ�.\n");
        return;
    }

    // IV �б�
    int iv[64];
    read_iv(in, iv);

    unsigned char buffer[8];
    int previous_block[64];  // ���� ��ȣ�� ���
    memcpy(previous_block, iv, sizeof(iv));  // ó������ IV�� ���

    // ���� ũ�� Ȯ�� (IV ����)
    fseek(in, 0, SEEK_END);
    size_t file_size = ftell(in) - 8;  // IV ũ��(8����Ʈ) ����
    fseek(in, 8, SEEK_SET);  // IV �������� �б� ����

    size_t total_bytes_read = 0;

    while ((fread(buffer, 1, 8, in)) > 0) {
        total_bytes_read += 8;

        // ���� ��ȣ�� ����� ��Ʈ �迭�� ��ȯ
        int current_ciphertext[64];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                current_ciphertext[i * 8 + j] = (buffer[i] >> (7 - j)) & 0x01;
            }
        }

        // DES ��ȣȭ
        int decrypted_block[64];
        des_decrypt(current_ciphertext, decrypted_block, key);

        // CBC ���: ���� ��ϰ� XOR
        int plaintext[64];
        xor_blocks(decrypted_block, previous_block, plaintext);

        // ���� ��ȣ���� ���� ���带 ���� ����
        memcpy(previous_block, current_ciphertext, sizeof(current_ciphertext));

        // ���� ����Ʈ�� ��ȯ
        for (int i = 0; i < 8; i++) {
            buffer[i] = 0;
            for (int j = 0; j < 8; j++) {
                buffer[i] |= (plaintext[i * 8 + j] << (7 - j));
            }
        }

        // ������ ����̸� �е� ����
        size_t write_size = 8;
        if (total_bytes_read == file_size) {
            write_size = remove_padding(buffer, 8);
        }

        fwrite(buffer, 1, write_size, out);
    }

    fclose(in);
    fclose(out);
}

// ���� �� �Լ�
int compare_files(const char* file1, const char* file2) {
    FILE* f1 = fopen(file1, "rb");
    FILE* f2 = fopen(file2, "rb");

    if (!f1 || !f2) {
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        printf("������ �� �� �����ϴ�.\n");
        return -1;
    }

    // ���� ũ�� ��
    fseek(f1, 0, SEEK_END);
    fseek(f2, 0, SEEK_END);
    long size1 = ftell(f1);
    long size2 = ftell(f2);

    if (size1 != size2) {
        printf("���� ũ�Ⱑ �ٸ��ϴ�.\n");
        printf("���� ���� ũ��: %ld bytes\n", size1);
        printf("��ȣȭ�� ���� ũ��: %ld bytes\n", size2);
        fclose(f1);
        fclose(f2);
        return 0;
    }

    // ���� �����͸� ó������ �ǵ���
    fseek(f1, 0, SEEK_SET);
    fseek(f2, 0, SEEK_SET);

    // ���� ũ�� ���� (64KB)
    const size_t BUFFER_SIZE = 65536;
    unsigned char* buffer1 = (unsigned char*)malloc(BUFFER_SIZE);
    unsigned char* buffer2 = (unsigned char*)malloc(BUFFER_SIZE);

    if (!buffer1 || !buffer2) {
        printf("�޸� �Ҵ� ����\n");
        if (buffer1) free(buffer1);
        if (buffer2) free(buffer2);
        fclose(f1);
        fclose(f2);
        return -1;
    }

    int is_identical = 1;
    size_t total_bytes = 0;
    size_t bytes_read1, bytes_read2;

    // ���� ������ ���� ���� ��
    while (1) {
        bytes_read1 = fread(buffer1, 1, BUFFER_SIZE, f1);
        bytes_read2 = fread(buffer2, 1, BUFFER_SIZE, f2);

        if (bytes_read1 != bytes_read2) {
            is_identical = 0;
            break;
        }

        if (bytes_read1 == 0) {
            break;
        }

        if (memcmp(buffer1, buffer2, bytes_read1) != 0) {
            is_identical = 0;
            // ����ġ�ϴ� ��ġ ã��
            for (size_t i = 0; i < bytes_read1; i++) {
                if (buffer1[i] != buffer2[i]) {
                    printf("ù ��° ����ġ ��ġ: %zu byte\n", total_bytes + i);
                    printf("���� ���� ��: 0x%02X\n", buffer1[i]);
                    printf("��ȣȭ�� ���� ��: 0x%02X\n", buffer2[i]);
                    break;
                }
            }
            break;
        }

        total_bytes += bytes_read1;
    }

    free(buffer1);
    free(buffer2);
    fclose(f1);
    fclose(f2);

    return is_identical;
}

int main() {
    int key[64] = {
        0,1,1,0, 0,1,1,0,  // 1����Ʈ (�и�Ƽ ��Ʈ ����)
        1,0,1,0, 1,0,1,0,  // 2����Ʈ
        0,1,1,0, 0,1,1,0,  // 3����Ʈ
        1,0,1,0, 1,0,1,0,  // 4����Ʈ
        0,1,1,0, 0,1,1,0,  // 5����Ʈ
        1,0,1,0, 1,0,1,0,  // 6����Ʈ
        0,1,1,0, 0,1,1,0,  // 7����Ʈ
        1,0,1,0, 1,0,1,0   // 8����Ʈ (�и�Ƽ ��Ʈ ����)
    };

    // ����ڷκ��� �ؽ�Ʈ �Է� �ޱ�
    printf("��ȣȭ�� �ؽ�Ʈ�� �Է��ϼ��� (�Է� �Ϸ� �� �� �ٿ��� Ctrl+Z (Windows)�� �����ּ���):\n");

    // plaintext.txt ���� ���� �Ǵ� ����
    FILE* plain_file = fopen("plaintext.txt", "w");
    if (!plain_file) {
        printf("plaintext.txt ������ ������ �� �����ϴ�.\n");
        return 1;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        fputs(buffer, plain_file);
    }
    fclose(plain_file);

    printf("\nDES CBC ��� ��ȣȭ/��ȣȭ ����\n");

    // �� ������ ��ȣȭ
    printf("���� ��ȣȭ ��...\n");
    encrypt_file("plaintext.txt", "ciphertext.dat", key);
    printf("��ȣȭ �Ϸ�\n");

    // ��ȣ�� ������ ��ȣȭ
    printf("���� ��ȣȭ ��...\n");
    decrypt_file("ciphertext.dat", "decrypted.txt", key);
    printf("��ȣȭ �Ϸ�\n");

    // ���� ��
    printf("\n���� �� ���� ����...\n");
    int result = compare_files("plaintext.txt", "decrypted.txt");

    if (result == 1) {
        printf("���� ����: ���� ���ϰ� ��ȣȭ�� ������ ������ ��ġ�մϴ�.\n");
    }
    else if (result == 0) {
        printf("���� ����: ������ ��ġ���� �ʽ��ϴ�.\n");
    }
    else {
        printf("���� ����: ���� �� �� ������ �߻��߽��ϴ�.\n");
    }

    return 0;
}