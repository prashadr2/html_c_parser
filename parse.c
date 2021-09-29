#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CWORDBUF 5000

char** tag_remover(char* filename){
    int retsize = 1;
    char** ret = calloc(retsize,sizeof(char*));
    FILE* fp = fopen(filename,"r");
    if(fp == NULL) perror("file error --\n");
    char curword[CWORDBUF]; memset(curword,0,CWORDBUF);
    char curtag[7]; memset(curtag,0,7);
    int cwordindex = 0;
    int ctag = 0;
    while(1){
        int c = fgetc(fp);
        if(feof(fp)) break;
        if(!isprint(c)) continue;
        ctag = 0;
        memset(curtag,0,7);
        if(c == '<') {
            while(ctag <= 5){
                c = fgetc(fp);
                if(feof(fp)) break;
                if(c == '!') continue;
                if(!isalnum(c)) break;
                if(c == '>') break;
                curtag[ctag++] = c;
            }
            curtag[ctag] = '\0';
            char sc[] = "script";
            char st[] = "style";
            while(c != '>'){
                c = fgetc(fp);
                if(feof(fp)) break;
            }
            //advance to next script/style tag, look for ending slash!!
            if(strcmp(curtag,sc) == 0 || strcmp(curtag,st) == 0){
                int end = 1;
                while(end){
                    c = fgetc(fp);
                    if(feof(fp)) break;
                    if(c == '<'){
                        c = fgetc(fp);
                        if(feof(fp)) break;
                        if(c != '/') continue;
                        while(c != '>'){
                            c = fgetc(fp);
                            if(feof(fp)) break;
                        }
                        end = 0;
                    }
                }
            } else if(cwordindex !=0) {
                ret[retsize-1] = calloc(cwordindex+1,sizeof(char));
                curword[cwordindex] = '\0';
                strcpy(ret[retsize-1],curword);
                retsize++;
                ret = realloc(ret,retsize*sizeof(char*));
                memset(curword,0,CWORDBUF);
                cwordindex = 0;
            }
            continue;
        }
        if(c == ' '){
            if(cwordindex !=0) {
                ret[retsize-1] = calloc(cwordindex+1,sizeof(char));
                curword[cwordindex] = '\0';
                strcpy(ret[retsize-1],curword);
                retsize++;
                ret = realloc(ret,retsize*sizeof(char*));
                memset(curword,0,CWORDBUF);
                cwordindex = 0;
            }
            continue;
        }
        if(cwordindex == CWORDBUF) {
            curword[CWORDBUF-1]='\0';printf("OUT OF MEMORY ERROR!!!-->increaes CWORDBUF size\ncurword->%s\n",curword);
            break;
        }
        curword[cwordindex++] = c;
    }
    fclose(fp);
    ret[retsize-1] = NULL;
    return ret;
}

int main(int argc, char** argv){
    if(argc != 2) {
        fprintf(stderr,"Usage: ./a.o <html-filename>\n");
        return EXIT_FAILURE;
    }
    char** htmlrval = tag_remover(argv[1]);
    char** clear1 = htmlrval;
    char** clear2 = htmlrval;

    while(*htmlrval != NULL) {
        char* w =  *htmlrval; 
        while(*w != '\0'){
            unsigned int x = (unsigned int)(unsigned char)(*w);
            if (isprint(x)) putchar(x); else if (x == '\n') printf("\\n"); else printf("%02x", x);
            w++;
        }
        printf("\n");
        htmlrval++;
    }

    while(*(clear1++) != NULL) free(*clear1);
    free(clear2);

    return EXIT_SUCCESS;
}