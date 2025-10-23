#include <stdio.h>
#include <string.h>

static const char kCommentMarker = ";";


void CommentFinder(char* workline) {
    char* offset_for_comment = strchr(workline, kCommentMarker);
    offset_for_comment = '\0';
    return;
}

int main {
    char* string[] = "tralalalo  ;comment";
    CommentFinder(string);
    printf("%s\n", string);
    return 0;
}

