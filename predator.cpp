 #include <stdio.h>
 #include <sys/stat.h>

#include "countFun.H"

 typedef struct{
    const char* file_name;
    size_t str_count;
    long long file_size;
    int* buffer;
 }fileInfo;

long long FileSize(FILE* refFile){
    struct stat fileFeature;
    fstat(fileno(refFile), &fileFeature);
    return fileFeature.st_size;
}


 void AssParser(fileInfo* refFileArch, FILE* file) {
    refFileArch -> file_size = FileSize(file);
    refFileArch -> buffer = (int*)calloc((size_t)((refFileArch -> file_size) + 1), sizeof(int));
    if (refFileArch -> buffer != NULL) {
        refFileArch -> buffer[(refFileArch -> file_size)] = '\0';
    }

    if (fread(refFileArch -> buffer, sizeof(char), refFileArch -> file_size, file) != refFileArch -> file_size) {
        printf("Read file error\n");
    }

    refFileArch -> str_count = countSymb(refFileArch -> buffer, '\n', refFileArch -> file_size);
 }

//NUTTellirovanie

 int main(void) {

    fileInfo mainFile = {"wannaASS.txt", 0, 0, 0};
    FILE* filename = fopen(mainFile.file_name, "r");

    AssParser(&mainFile, filename);




 }


