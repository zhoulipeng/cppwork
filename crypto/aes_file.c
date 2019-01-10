#include <linux/limits.h>
#include "gperftools/heap-profiler.h"
#include "aes_file.h"

// Note: This isn't a good way to encrypt large file (anything that can't be read into
// memory in a single buffer). A better approach for this is to read in one block at a type,
// encrypt it, write it to a file and so on.

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr, "No file argument supplied.\n");
    return 1;
  }
  // If you don't use gperftools heapprofiling, please comment next line
  HeapProfilerStart("main");
  Crypto();

  char encryptedFilename[NAME_MAX] = { 0 };
  // Append .enc to the filename
  strcpy(encryptedFilename, argv[1]);
  strcat(encryptedFilename, ".enc");
  encryptFile(argv[1], encryptedFilename);
  // Append .dec to the filename
  char decryptedFilename[NAME_MAX] = { 0 };
  strcpy(decryptedFilename, argv[1]);
  strcat(decryptedFilename, ".dec");
  decryptFile(encryptedFilename, decryptedFilename);
  freeCrypto();
  // If you don't use gperftools heapprofiling, please comment next line
  HeapProfilerStop();

  return 0;
}

int encryptFile(char *filename, char *encryptedFilename) {
  // Read the file to encrypt
  unsigned char *file;
  size_t fileLength = readFile(filename, &file);
  printf("%d bytes to be encrypted\n", (int)fileLength);

  // Encrypt the file
  unsigned char *encryptedFile;
  int encryptedFileLength = aesEncrypt((const unsigned char*)file, fileLength, &encryptedFile);

  if(encryptedFileLength == -1) {
    fprintf(stderr, "Encryption failed\n");
    exit(1);
  }
  printf("%d bytes encrypted\n", encryptedFileLength);

  // Write the encrypted file to its own file
  writeFile(encryptedFilename, encryptedFile, encryptedFileLength);
  printf("Encrypted file written to \"%s\"\n", encryptedFilename);

  free(file);
  return 0;
}

void decryptFile(char *encryptedFilename, char *decryptedFilename) {
  // Read the encrypted file back
  unsigned char *file;
  size_t fileLength = readFile(encryptedFilename, &file);

  // Decrypt the encrypted file
  unsigned char *decryptedFile;
  int decryptedFileLength = aesDecrypt(file, fileLength, &decryptedFile);

  if(decryptedFileLength == -1) {
    fprintf(stderr, "Decryption failed\n");
    exit(1);
  }
  printf("%d bytes decrypted\n", (int)decryptedFileLength);

  // Write the decrypted file to its own file
  writeFile(decryptedFilename, decryptedFile, decryptedFileLength);
  printf("Decrypted file written to \"%s\"\n", decryptedFilename);

  free(decryptedFile);
  free(file);
}

void writeFile(char *filename, unsigned char *file, size_t fileLength) {
  FILE *fd = fopen(filename, "wb");
  if(fd == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
    exit(1);
  }

  size_t bytesWritten = fwrite(file, 1, fileLength, fd);

  if(bytesWritten != fileLength) {
    fprintf(stderr, "Failed to write file\n");
    exit(1);
  }

  fclose(fd);
}

int readFile(char *filename, unsigned char **file) {
  FILE *fd = fopen(filename, "rb");
  if(fd == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
    exit(1);
  }

  // Determine size of the file
  fseek(fd, 0, SEEK_END);
  size_t fileLength = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  // Allocate space for the file
  *file = (unsigned char*)malloc(fileLength);
  if(*file == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    exit(1);
  }

  // Read the file into the buffer
  size_t bytesRead = fread(*file, 1, fileLength, fd);

  if(bytesRead != fileLength) {
    fprintf(stderr, "Error reading file\n");
    exit(1);
  }

  fclose(fd);

  return fileLength;
}

