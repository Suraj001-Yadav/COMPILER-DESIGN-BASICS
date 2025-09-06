#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
#define KEYWORD 1
#define OPERATOR 2
#define IDENTIFIER 3
#define NUMBER 4
#define DELIMITER 5

// List of keywords
char *keywords[] = {
    "int", "float", "char", "double", "if", "else", "while", 
    "for", "return", "void", "main", "printf", "scanf"
};

// List of operators
char operators[] = {'+', '-', '*', '/', '=', '<', '>', '!', '&', '|'};

// Function prototypes
int isKeyword(char *word);
int isOperator(char ch);
int isDelimiter(char ch);
void analyze(FILE *file);

int main() {
    char filename[100];
    FILE *file;
    
    printf("============================================\n");
    printf("        SIMPLE LEXICAL ANALYZER\n");
    printf("============================================\n");
    
    printf("Enter the input file name: ");
    scanf("%s", filename);
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file '%s'\n", filename);
        printf("Creating a sample input file...\n");
        
        // Create a sample input file
        file = fopen("sample_input.txt", "w");
        if (file) {
            fprintf(file, "int main() {\n");
            fprintf(file, "    int a = 10;\n");
            fprintf(file, "    float b = 20.5;\n");
            fprintf(file, "    if (a < b) {\n");
            fprintf(file, "        printf(\"a is smaller\");\n");
            fprintf(file, "    }\n");
            fprintf(file, "    return 0;\n");
            fprintf(file, "}\n");
            fclose(file);
            
            printf("Sample file 'sample_input.txt' created successfully!\n");
            printf("Please run the program again and enter 'sample_input.txt'\n");
        }
        return 1;
    }
    
    printf("\nLEXICAL ANALYSIS RESULTS:\n");
    printf("=========================\n");
    printf("%-15s %-15s %-15s\n", "TOKEN", "TYPE", "VALUE");
    printf("---------------------------------------------\n");
    
    analyze(file);
    fclose(file);
    
    return 0;
}

int isKeyword(char *word) {
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isOperator(char ch) {
    int numOperators = sizeof(operators) / sizeof(operators[0]);
    for (int i = 0; i < numOperators; i++) {
        if (ch == operators[i]) {
            return 1;
        }
    }
    return 0;
}

int isDelimiter(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || 
           ch == '(' || ch == ')' || ch == '{' || ch == '}' || 
           ch == '[' || ch == ']' || ch == ';' || ch == ',';
}

void analyze(FILE *file) {
    char ch, buffer[100];
    int bufferIndex = 0;
    int lineNumber = 1;
    
    while ((ch = fgetc(file)) != EOF) {
        // Handle new lines
        if (ch == '\n') {
            lineNumber++;
            continue;
        }
        
        // Skip whitespace
        if (ch == ' ' || ch == '\t') {
            continue;
        }
        
        // Check for operators
        if (isOperator(ch)) {
            printf("%-15s %-15s %-15c\n", "OPERATOR", "<op>", ch);
            continue;
        }
        
        // Check for delimiters
        if (isDelimiter(ch)) {
            printf("%-15s %-15s %-15c\n", "DELIMITER", "<del>", ch);
            continue;
        }
        
        // Handle numbers
        if (isdigit(ch)) {
            bufferIndex = 0;
            buffer[bufferIndex++] = ch;
            
            while ((ch = fgetc(file)) != EOF && (isdigit(ch) || ch == '.')) {
                buffer[bufferIndex++] = ch;
            }
            buffer[bufferIndex] = '\0';
            ungetc(ch, file);
            
            printf("%-15s %-15s %-15s\n", "NUMBER", "<num>", buffer);
            continue;
        }
        
        // Handle identifiers and keywords
        if (isalpha(ch) || ch == '_') {
            bufferIndex = 0;
            buffer[bufferIndex++] = ch;
            
            while ((ch = fgetc(file)) != EOF && (isalnum(ch) || ch == '_')) {
                buffer[bufferIndex++] = ch;
            }
            buffer[bufferIndex] = '\0';
            ungetc(ch, file);
            
            if (isKeyword(buffer)) {
                printf("%-15s %-15s %-15s\n", "KEYWORD", "<kw>", buffer);
            } else {
                printf("%-15s %-15s %-15s\n", "IDENTIFIER", "<id>", buffer);
            }
            continue;
        }
        
        // Handle strings (simple implementation)
        if (ch == '"') {
            bufferIndex = 0;
            buffer[bufferIndex++] = ch;
            
            while ((ch = fgetc(file)) != EOF && ch != '"') {
                buffer[bufferIndex++] = ch;
            }
            if (ch == '"') {
                buffer[bufferIndex++] = ch;
            }
            buffer[bufferIndex] = '\0';
            
            printf("%-15s %-15s %-15s\n", "STRING", "<str>", buffer);
            continue;
        }
    }
}