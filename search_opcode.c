#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int recordLen() {
    // get record nums in opcode.txt
    FILE* ptr;
    char str[50];
    ptr = fopen("opcode.txt", "r");
 
    if (NULL == ptr) {
        printf("file can't be opened \n");
    }
 
    int record_len = 0;
 
    // counting the length of record 
    while (fgets(str, 50, ptr) != NULL) {
        record_len++;
    }
    fclose(ptr);

    return record_len;
}

char ** readFile(int record_len)
{
    FILE* ptr;
    char str[50];
    ptr = fopen("opcode.txt", "r");
    int index = 0;
    char ** all_str = malloc(record_len * sizeof(char*));

    // storing each record
    while (fgets(str, 50, ptr) != NULL) {
        all_str[index++] = strdup(str);
    }

    fclose(ptr);
    return all_str;
}

int getCodeOpcode(char * c) {
    int code = 0;
    for (int i = 0;i < strlen(c);i++) {
        if (c[i] == ' ') {
            break;
        }
        code += (int)c[i];
    }
    return code;
}

char ** hashTable(char ** all_str, int record_len) {
    // make and return a hash table
    char ** hash_table = malloc(record_len * sizeof(char*));
    hash_table[0] = NULL;
    hash_table[1] = NULL;

    for (int i = 0;i < record_len;i++) {
        int val = getCodeOpcode(all_str[i]);
        int index = val % record_len;
        if (hash_table[index] == NULL) { // the bucket not have value
            hash_table[index] = all_str[i];
        }
        else { // probing other empty bucket
            // linear probing the empty bucket
            int empty_bucket = -1;
            for (int i = 0;i < record_len;i++) {
                if (hash_table[i] == NULL) {
                    empty_bucket = i;
                    break;
                }
            }

            // check whether have empty bucket
            if (empty_bucket == -1) {
                printf("no more empty bucket\n");
            }
            else {
                hash_table[empty_bucket] = all_str[i];
            }
        }
    }

    return hash_table;
}

int cmpMnemonic(char * str, char * c, int record_len) {
    for (int i = 0;i < record_len;i++) {
        if (str[i] == ' ') {
            if (i != strlen(c)-1) { // not same length, then not same string
                // minus a char of new line
                return 1; // not same string
            }
            break;
        }
        if (str[i] != c[i]) {
            return 1; // not same string
        }
    }
    return 0; // str and c are same string
}

char * getOpcode(char * c) { // get opcode part of each line
    char * opcode = malloc(strlen(c) * sizeof(char));
    int start_opcode = 0;
    char * temp = malloc(sizeof(char));
    for (int i = 0;i < strlen(c);i++) {
        if (c[i] == ' ') {
            start_opcode = 1;
            continue;
        }
        if (start_opcode) {
            temp[0] = c[i]; // temp is a char pointer to temp store the char
            strcat(opcode, temp); // concat two string
        }
    }
    return opcode;
}

int getCode(char * c) {
    int code = 0;
    // last char is new line, so ignore it
    for (int i = 0;i < strlen(c)-1;i++) {
        code += (int)c[i];
    }
    return code;
}

void find(char * mnemonic, char ** all_str, int record_len) {
    // find relative opcode and output
    char ** hash_table = hashTable(all_str, record_len); // make a hash table with data in opcode.txt
    int mnemonic_index = getCode(mnemonic);
    mnemonic_index = mnemonic_index % record_len;
    int temp_mnemonic_index = mnemonic_index;

    while (cmpMnemonic(hash_table[mnemonic_index], mnemonic, record_len)) {
        mnemonic_index++;
        if (mnemonic_index == temp_mnemonic_index) {
            // stop finding when back to start index
            //printf("can not found the opcode of %s\n", mnemonic);
            printf("invalid");
            return;
        }
        if (mnemonic_index >= record_len) {
            // find from 0
            mnemonic_index = 0;
        }
    }

    // output
    //printf("the relative opcode of %s = %s", mnemonic, getOpcode(hash_table[mnemonic_index]));     
    printf("opCode : %s\n", getOpcode(hash_table[mnemonic_index]));   
}

int main() {
    int record_len = recordLen();
    char ** all_str = readFile(record_len);

    // input menmonic
    char mnemonic[20];
    printf("search (Input a mnemonic) : ");
    fgets(mnemonic, 20, stdin);

    // find relative opcode and output
    find(mnemonic, all_str, record_len);

    return 0;
}