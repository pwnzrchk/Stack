#include "fileFunc.h"


long long FileSize(FILE* refFile) {
    struct stat fileFeature;
    fstat(fileno(refFile), &fileFeature);
    return fileFeature.st_size;
}

//=================================================================================================================================================

fileFunErr_t Plenumation(fileInfo* refFileArch) {
    assert(refFileArch -> file_name != NULL);
    
    FILE* file = fopen(refFileArch -> file_name, "r");
    if (!file) {
        fprintf(stderr, "Open file ERR in plenumation, in file %s\n", refFileArch -> file_name);
        return OPEN_PLUM_ERR;
    }
    refFileArch -> file_size = FileSize(file);
    refFileArch -> buffer = (char*)calloc((size_t)((refFileArch -> file_size) + 1), sizeof(char));

    if (refFileArch -> buffer == NULL) {
        fprintf(stderr, "Calloc error for text_buffer\n");
        return NULL_PTR_PLUM;
    }
    if (refFileArch -> buffer != NULL) {
        refFileArch -> buffer[(refFileArch -> file_size)] = '\0';
    }

    if ((long long)fread(refFileArch -> buffer, sizeof(char), (size_t)refFileArch -> file_size, file) != refFileArch -> file_size) {
        printf("Read file error\n");
        return FREAD_PLUM_ERR;
    }
    refFileArch -> str_count = (size_t)countSymb(refFileArch -> buffer, '\n', (size_t)refFileArch -> file_size) + 1;
    fclose(file);
    return NO_PLUM_ERR;
 }

//=================================================================================================================================================

fileFunErr_t Distributor(fileInfo* refFileArch) {

    assert(refFileArch -> file_size > 0);
    assert(refFileArch -> file_name != NULL);
    assert(refFileArch -> str_count > 0);

    if (refFileArch -> str_count == 0) return INC_PLUM_SIZE;

    refFileArch -> pointerBuffer = (char**)calloc(refFileArch -> str_count, sizeof(char*));
    if (!(refFileArch -> pointerBuffer)) {
        fprintf(stderr, "Calloc error for ptr_buffer\n");
        free(refFileArch -> pointerBuffer);
        return CALLOC_PLUM_ERR;
    }

    size_t stringsAmount = 0;
    refFileArch -> pointerBuffer[stringsAmount++] = refFileArch -> buffer;
    for (size_t i = 0; i < (size_t)refFileArch -> file_size && refFileArch -> buffer[i] != '\0'; i++) {
        if(refFileArch -> buffer[i] == '\n') {
            if (refFileArch -> buffer[i+1] != '\0') {
                refFileArch -> buffer[i] = '\0';
                refFileArch -> pointerBuffer[stringsAmount++] = &(refFileArch -> buffer[i+1]);
            } else break;
        }
    }
    return NO_PLUM_ERR;
}



