#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <openssl/md5.h>
#include <locale.h>

char *hash;
int len;
clock_t begin;
float tempo;
int fim = 0;
char res[10];
const char charset[] = "@$#?!=+%abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const int charset_size = sizeof(charset) - 1;
void compara_md5(char *string);
void aiQueBruto(char* str, int index);
void bruto_seq();
void vai(int nthreads);
void apd_tranquilo();
void inicia_csv();

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Use: %s -hash -n_letras [-nthreads]\n", argv[0]);
        exit(1);
    }
    hash = argv[1];
    len = (long)strtol(argv[2], NULL, 10);
    char *oldLocale = setlocale(LC_NUMERIC, NULL);
    setlocale(LC_NUMERIC, "");

    if (argc >= 4) {
        int nthreads = strtol(argv[3], NULL, 10);
        if (nthreads == 0) {
            printf("Método utilizado: sequencial\n");
            bruto_seq();
        } else {
            printf("Método utilizado: paralelismo com %i threads\n", nthreads);
            vai(nthreads);
        }
        if (fim) printf("String: %s\nTempo: %f\n", res, tempo);
        else printf("String não encontrada!\n");
    } else {
        apd_tranquilo();
    }
    setlocale(LC_NUMERIC, oldLocale);
    return 0;
}

void compara_md5(char *string) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, string, strlen(string));
    MD5_Final(digest, &ctx);

    char mdString[33];
    for (int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    if (strcmp(mdString, hash) == 0) {
        fim = 1;
        tempo = (double)(clock() - begin) / CLOCKS_PER_SEC;
        strcpy(res, string);
    }
}

void aiQueBruto(char* str, int index) {
    for (int i = 0; i < charset_size; ++i) {
        if (!fim) {
            str[index] = charset[i];
            if (index == len - 1) {
                str[len] = '\0';
                compara_md5(str);
            } else aiQueBruto(str, index + 1);
        }
    }
}

void bruto_seq() {
    begin = clock();
    char *str = malloc(len + 1);
    aiQueBruto(str, 0);
    free(str);
}

void vai(int nthreads) {
    int p = 0;
    begin = clock();
    omp_set_num_threads(nthreads);
    #pragma omp parallel private(p)
    {
        #pragma omp for schedule(dynamic)
        for (p = 0; p < charset_size; ++p) {
            if (!fim) {
                char *str = malloc(len + 1);
                str[0] = charset[p];
                aiQueBruto(str, 1);
                free(str);
            }
        }
    }
}

void apd_tranquilo() {
    inicia_csv();
    FILE *fp;
    fp = fopen("resultado.csv", "a");
    for (int x = 1; x <= 5; x++) {
        printf("========= Execução: %i =========\n", x);
        fprintf(fp, "%i", x);
        bruto_seq();
        fprintf(fp, ",\"%f\"", tempo);
        printf("Sequencial  | Tempo: %f\n", tempo);
        fim = 0;
        for (int y = 1; y <= 64; y = y * 2) {
            vai(y);
            fprintf(fp, ",\"%f\"", tempo);
            printf("Threads: %2i | Tempo: %f\n", y, tempo);
            fim = 0;
        }
        fprintf(fp, "\n");
    }
    printf("===============================\nString: %s\n", res);
    fclose(fp);
}

void inicia_csv() {
    FILE *fp;
    fp = fopen("resultado.csv", "w");
    fprintf(fp, ",Sequencial");
    for (int y = 1; y <= 64; y = y * 2) {
        fprintf(fp, ",%i", y);
    }
    fprintf(fp, "\n");
    fclose(fp);
}
