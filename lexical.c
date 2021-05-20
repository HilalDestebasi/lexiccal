#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAXCHAR 10000
typedef enum
{
    F,
    T
} bool;

bool isDigit(bool digit, char *fprint)
{
    if (digit == T)
    {
        strcat(fprint, ")\n");
    }
    return F;
}

char controlKeyword(char string[MAXCHAR])
{
    if (strcasecmp(string, "break") == 0 || strcasecmp(string, "case") == 0 || strcasecmp(string, "char") == 0 || strcasecmp(string, "const") == 0 || strcasecmp(string, "continue") == 0 || strcasecmp(string, "do") == 0 || strcasecmp(string, "else") == 0 || strcasecmp(string, "enum") == 0 || strcasecmp(string, "float") == 0 || strcasecmp(string, "for") == 0 || strcasecmp(string, "goto") == 0 || strcasecmp(string, "if") == 0 || strcasecmp(string, "int") == 0 || strcasecmp(string, "long") == 0 || strcasecmp(string, "record") == 0 || strcasecmp(string, "return") == 0 || strcasecmp(string, "static") == 0 || strcasecmp(string, "while") == 0)
    {
        return 'T';
    }
    return 'F';
}

bool isIdentifier(bool identifier, char *string, char *fprint)
{
    if (identifier == T)
    {
        if (controlKeyword(string) == 'T')
        {
            strcat(fprint, "Keyword(");
        }
        else
        {
            strcat(fprint, "Identifier(");
        }

        strcat(fprint, string);
        strcpy(string, "");
        strcat(fprint, ")\n");
        return F;
    }
    return F;
}

void isDigit_isIdentifier(bool *digit, bool *identifier, char *string, char *fprint)
{
    *digit = isDigit(*digit, fprint);
    *identifier = isIdentifier(*identifier, string, fprint);
}

char *lexleyelimBakalim(char *line)
{
    int l = 0;
    char *write = "";
    char fprint[MAXCHAR];
    strcpy(fprint, "");
    bool eol = F;
    bool quotation = F;
    bool identifier = F;
    bool comment = F;
    char string[MAXCHAR];
    bool keyword = F;
    bool negativeNumber = F;
    bool digit = F;
    bool operatorControl = F;
    int par = 0;
    int squareBracket = 0;
    int curlyBracket = 0;
    int constLength = 0;
    int idenLenght = 0;
    while (line[l] != '\0' && line[l] != '\n')
    {
        if (line[l] == '(' && line[l + 1] == '*')
        {
            if (comment == F)
            {
                comment = T;
            }
        }
        if (line[l] == '*' && line[l + 1] == ')')
        {
            comment = F;
        }
        if (comment == F && line[l] == '"')
        {
            if (quotation == F)
            {
                quotation = T;
                strcat(fprint, "String(");
                l++;
            }
            else
            {
                quotation = F;
                strcat(fprint, ")\n");
            }
        }
        if (comment == F && quotation == F)
        {
            if (line[l] == ';')
            {
                isDigit_isIdentifier(&digit, &identifier, string, fprint);
                eol = T;
                strcat(fprint, "End of Line\n");
            }
            else if (line[l] == '(' && line[l + 1] != '*')
            {
                isDigit_isIdentifier(&digit, &identifier, string, fprint);
                strcat(fprint, "LeftPar\n");
                par++;
            }
            else if (line[l] == '[')
            {
                isDigit_isIdentifier(&digit, &identifier, string, fprint);
                strcat(fprint, "LeftSquareBracket\n");
                squareBracket++;
            }
            else if (line[l] == ']')
            {
                isDigit_isIdentifier(&digit, &identifier, string, fprint);
                strcat(fprint, "RightSquareBracket\n");
                squareBracket--;
            }
            else if (line[l] == '{')
            {
                isDigit_isIdentifier(&digit, &identifier, string, fprint);
                strcat(fprint, "LeftCurlyBracket\n");
                curlyBracket++;
            }
            else if (line[l] == '}')
            {
                isDigit_isIdentifier(&digit, &identifier, string, fprint);
                strcat(fprint, "RightCurlyBracket\n");
                curlyBracket--;
            }
            else if (line[l] == ')')
            {
                if (l > 1)
                {

                    if (line[l - 1] != '*')
                    {
                        isDigit_isIdentifier(&digit, &identifier, string, fprint);
                        strcat(fprint, "RightPar\n");
                        par--;
                    }
                }
                else
                {
                    par--;
                }
            }
            else if ((line[l] == '+' && line[l + 1] == '+') || (line[l] == '-' && line[l + 1] == '-') || (line[l] == ':' && line[l + 1] == '='))
            {
                isDigit_isIdentifier(&digit, &identifier, string, fprint);
                strcat(fprint, "Operator(");
                strncat(fprint, &line[l], 1);
                strncat(fprint, &line[l + 1], 1);
                strcat(fprint, ")\n");
                if (l > 0)
                {
                    if (((line[l] == '+' && line[l + 1] == '+') || (line[l] == '-' && line[l + 1] == '-')) && (isalpha(line[l - 1]) || isdigit(line[l - 1]) || line[l - 1] == '_') && (isalpha(line[l + 2]) || isdigit(line[l + 2]) || line[l + 2] == '_'))
                    {
                        operatorControl = T;
                    }
                }
                l++;
            }
            else if (line[l] == '+' || line[l] == '-' || (line[l] == '*' && line[l + 1] != ')') || line[l] == '/')
            {
                isDigit_isIdentifier(&digit, &identifier, string, fprint);
                strcat(fprint, "Operator(");
                strncat(fprint, &line[l], 1);
                strcat(fprint, ")\n");
            }
            else if (isalpha(line[l]))
            {
                if (identifier == F)
                {
                    identifier = T;
                }
                strncat(string, &line[l], 1);
                idenLenght++;
            }
            else if (identifier != T && isdigit(line[l]))
            {
                if (digit == F)
                {
                    digit = T;
                    if (l == 1)
                    {
                        if (line[l - 1] != '-')
                            strcat(fprint, "IntConst(");
                        else
                        {
                            negativeNumber = T;
                        }
                    }
                    else if (l == 0 || l == 2)
                    {
                        strcat(fprint, "IntConst(");
                    }
                    else if (l > 2)
                    {
                        if (line[l - 1] == '-' && line[l - 2] == '-' && line[l - 3] == '-')
                            negativeNumber = T;
                        else
                        {
                            strcat(fprint, "IntConst(");
                        }
                    }
                }
                strcpy(string, "");
                strncat(fprint, &line[l], 1);
                constLength++;
            }
            else if (identifier == T)
            {
                if (line[l] == ' ')
                {
                    if (controlKeyword(string) == 'T')
                    {
                        strcat(fprint, "Keyword(");
                        strcat(fprint, string);
                        strcpy(string, "");
                        strcat(fprint, ")\n");
                    }
                    else
                    {
                        strncat(string, &line[l], 1);
                    }
                }
                else
                {
                    strncat(string, &line[l], 1);
                }
            }
        }
        else if (comment == F)
        {
            strncat(fprint, &line[l], 1);
        }
        l++;
    }
    write = fprint;
    if (curlyBracket != 0 || par != 0 || squareBracket != 0)
    {
        write = "Error Bracket\n";
    }
    else if (operatorControl == T)
    {
        write = "Error operator\n";
    }
    else if (negativeNumber == T)
    {
        write = "Error negative number\n";
    }
    else if (constLength >= 9)
    {
        write = "Error Integer length\n";
    }
    else if (idenLenght >= 19)
    {
        write = "Error identifier length\n";
    }
    else if (quotation == T)
    {
        write = "Error quotation\n";
    }
    else if (eol == F)
    {
        write = "Error eol\n";
    }
    else if (comment == T)
    {
        write = "Error comment end\n";
    }
    return write;
}

void create_file(FILE *fp, char *wFileName)
{
    FILE *out;
    char *line = NULL;
    char str[MAXCHAR];
    out = fopen(wFileName, "w");
    while (fgets(str, MAXCHAR, fp) != NULL)
    {
        fprintf(out, "%s", lexleyelimBakalim(str));
    }

    fclose(out);
}

void read_file(char *rFileName, char *wFileName)
{
    FILE *fp;
    fp = fopen(rFileName, "r");
    if (fp == NULL)
        printf("E");
    create_file(fp, wFileName);
    fclose(fp);
}

int main(void)
{
    char *rFileName = "code.ceng";
    char *wFileName = "code.lex";
    read_file(rFileName, wFileName);
}