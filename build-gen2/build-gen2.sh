#!/bin/bash

TMP_DIR=./build-gen2

expand() {
    file=$1
    echo;
    echo '### Building '$file
    cat <<EOF > ${TMP_DIR}/$file
    typedef int size_t;
    typedef struct FILE FILE;
    extern FILE *stdout;
    extern FILE *stderr;

    typedef struct {
    int gp_offset;
    int fp_offset;
    void *overflow_arg_area;
    void *reg_save_area;
    } __va_elem;

    typedef __va_elem va_list[1];

    extern void va_end(__va_elem *ap);

    extern void printf(char *fmt, ...);
    extern void fprintf(FILE *s, char *fmt, ...);
    extern void vfprintf(FILE *s, char *fmt, __va_elem *ap, ...);

    FILE *fopen(char *pathname, char *mode);
    char *strerror(int errnum);
    void exit(int rc);

    int *__errno_location();
    int fseek(FILE *stream, ...); // original: int fseek(FILE *stream, long int offset, int whence);
    int ftell(FILE *stream);  // original: long int ftell(FILE *stream);
    int fread(void *ptr, int size, int nmemb, FILE *stream);  // original: long fread(void *ptr, long size, long nmemb, FILE *stream);
    int fclose(FILE *stream);

    void *calloc(int nmemb, int size);  // original: void *calloc(long nmemb, long size);

    _Bool isdigit(char c);
    _Bool isspace(char c);
    int strlen(char *);
    char *strstr(char *, char *);
    extern int strncmp (const char *__s1, const char *__s2, size_t __n);
    int memcmp(void *, void *, int);
    void *realloc(void *, int);
    int strtol(const char *s, char **endptr, int base); // original: long strtol(const char *s, char **endptr, int base);
EOF

    if [ "$file" = "container.c" ];
    then
        sed -i 's/extern void va_end(__va_elem \*ap);//g' ${TMP_DIR}/$file
        cat <<EOF >> ${TMP_DIR}/$file
        void va_end(__va_elem *ap) {}
EOF
    fi
    grep -v '^#' ./mimicc.h >> ${TMP_DIR}/$file  ## expand header
    grep -v '^#' ./$file >> ${TMP_DIR}/$file  ## expand code, removing any macros (#include etc.)
    sed -i 's/\berrno\b/*__errno_location()/g' ${TMP_DIR}/$file  ## replace errno by builtin-function
    sed -i 's/\bNULL\b/0/g' ${TMP_DIR}/$file  ## replace NULL by 0
    sed -i 's/\bSEEK_END\b/2/g' ${TMP_DIR}/$file  ## replace SEEK_END by 2
    sed -i 's/\bSEEK_SET\b/0/g' ${TMP_DIR}/$file  ## replace SEEK_SET by 0

    ./mimicc ${TMP_DIR}/$file > ${TMP_DIR}/${file%.c}.s
    gcc -c -o ${TMP_DIR}/${file%.c}.o ${TMP_DIR}/${file%.c}.s
}

expand main.c
expand codegen.c
expand parse.c
expand lex.c
expand container.c

echo;
echo '### Linking...'
gcc -static -o mimicc-gen2 ${TMP_DIR}/*.o