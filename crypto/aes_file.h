#ifndef CRYPTO_FILE_EXAMPLE_H
#define CRYPTO_FILE_EXAMPLE_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "base64.h"
#include "crypto_ssl.h"

// Uncomment to write the encrypted file encoded in base 64
// #define CONVERT_TO_BASE64

char* encryptFile(char *filename);
void decryptFile(char *filename, char *encryptedFilename);

void writeFile(char *filename, unsigned char *file, size_t fileLength);
int readFile(char *filename, unsigned char **file);

char* appendToString(char *string, char *suffix);

#endif
