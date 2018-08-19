#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_CHOICES 3
#define CHOICE_CHARS "abq"
#define DEFAULT_MAXCHARS_ONELINE 76

void decode(FILE *source, FILE *target);
void encode(FILE *source, FILE *target, int maxchars_oneline);
int encode_isend(FILE *source, char *ch, FILE *target);
int maxchars(void);
int get_choice(void);
void get_filename(char *filename);
int exist_choice(char ch);

const char table[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

int main(void)
{
    FILE *source, *target;
    char sourcename[50], targetname[50];
    while (1)
    {
        puts("\t\t\t*********************");
        puts("\t\t\t*      Base64       *");
        puts("\t\t\t* Decoder & Encoder *");
        puts("\t\t\t*********************");
        puts("\t\t\ta.Encode");
        puts("\t\t\tb.Decode");
        puts("\t\t\tq.quit");

        char choice = get_choice();
        if (choice == 'q')
            exit(EXIT_SUCCESS);

        get_filename(sourcename);
        if (!(source = fopen(sourcename, "r")))
        {
            printf("Error! Fail to open the file %s.\n", sourcename);
            continue;
        }

        switch (choice)
        {
        case 'a':
            strcpy(targetname, "encoded_");
            strcat(targetname, sourcename);
            target = fopen(targetname, "w");
            encode(source, target, maxchars());
            break;
        case 'b':
            strcpy(targetname, "decoded_");
            strcat(targetname, sourcename);
            target = fopen(targetname, "w");
            // decode(source, target);
            break;
        }

        fclose(source), fclose(target);
    }
    return 0;
}

int get_choice(void)
{
    char ch;
    puts("Enter the option:");
    while (!exist_choice(ch = getchar()))
    {
        puts("Error! Please enter the option again:");
        while (getchar() != '\n')
        {
            continue;
        }
    }
    while (getchar() != '\n')
    {
        continue;
    }
    return ch;
}

int exist_choice(char ch)
{
    char j[N_CHOICES] = CHOICE_CHARS;
    for (int i = 0; i < N_CHOICES; i++)
    {
        if (ch == j[i])
            return 1;
    }
    return 0;
}

void get_filename(char *filename)
{
    puts("Enter the filename to be encode(decode):");
    while ((scanf("%s", filename)) != 1)
    {
        puts("Error! Please enter again:");
        while (getchar() != '\n')
            continue;
    }
    while (getchar() != '\n')
        continue;
    return;
}

void decode(FILE *source, FILE *target)
{
}

void encode(FILE *source, FILE *target, int maxchars_oneline)
{
    char j[4];
    int n = 1;
    int n_equal;
    while (!(n_equal = encode_isend(source, j, target)))
    {
        for (int i = 0; i < 4; i++)
        {
            fputc(j[i], target);
            if (n++ % maxchars_oneline == 0)
            {
                fputc('\n', target);
                n = 1;
            }
        }
    }
    if (n_equal == 2)
    {
        fputc(j[0], target);
        fputc(j[1], target);
        fputc('=', target);
        fputc('=', target);
    }
    else if (n_equal == 1)
    {
        fputc(j[0], target);
        fputc(j[1], target);
        fputc(j[2], target);
        fputc('=', target);
    }

    return;
}

int encode_isend(FILE *source, char *ch, FILE *target)
{
    char temp[4] = {'\0'};
    int isend = 0;
    if ((temp[2] = fgetc(source)) == EOF)
    {
        isend = 0 == isend ? -1 : isend;
        temp[2] = '\0';
    }
    if ((temp[1] = fgetc(source)) == EOF)
    {
        isend = 0 == isend ? 2 : isend;
        temp[1] = '\0';
    }
    if ((temp[0] = fgetc(source)) == EOF)
    {
        isend = 0 == isend ? 1 : isend;
        temp[0] = '\0';
    }
    int32_t *s = (int32_t *)temp;

    ch[0] = table[(*s & 0xfc0000) >> 18];
    ch[1] = table[(*s & 0x3f000) >> 12];
    ch[2] = table[(*s & 0xfc0) >> 6];
    ch[3] = table[*s & 0x3f];

    return isend;
}

int maxchars(void)
{
    int n;
    puts("Enter the number of max characters one line(0 to default):");
    for (scanf("%d", &n); !isdigit(n) && (n < 0 || n > DEFAULT_MAXCHARS_ONELINE); scanf("%d", &n))
    {
        printf("Error! Enter a number(0~%d) again:", DEFAULT_MAXCHARS_ONELINE);
        while (getchar() != '\n')
        {
            continue;
        }
    }
    while (getchar() != '\n')
    {
        continue;
    }

    if (n == 0)
        n = DEFAULT_MAXCHARS_ONELINE;

    return n;
}