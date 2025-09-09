#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef enum {
    token_plus,
    token_minus,
    token_asterisk,
    token_slash,
    token_modulo,
    token_caret,
    token_octothorpe,
    token_equal_equal,
    token_not_equal,
    token_less_equal,
    token_greater_equal,
    token_less,
    token_greater,
    token_equal,
    token_open_paren,
    token_close_paren,
    token_open_brace,
    token_close_brace,
    token_open_bracket,
    token_close_bracket,
    token_semicolon,
    token_colon,
    token_comma,
    token_dot,
    token_dot_dot,
    token_dot_dot_dot, 

    // keywords
    token_and,
    token_break,
    token_do,
    token_else,
    token_elseif,
    token_end,
    token_false,
    token_for,
    token_function,
    token_if,
    token_in,
    token_local,
    token_nil,
    token_not,
    token_or,
    token_repeat,
    token_return,
    token_then,
    token_true,
    token_until,
    token_while,

    // literals
    token_number,
    token_string,
    token_identifier,

    // special
    token_eof,
} TokenType;

typedef struct {
    char *str;
    size_t len;
} String;

// string_create copies str
String string_create(const char *str) {
    String string;
    string.len = strlen(str);
    string.str = (char *)malloc(string.len + 1);
    strcpy(string.str, str);
    return string;
}

// warning, if src has length zero AND the char * is unitialized, dest will be fed garbage and potential
// buffer overruns
void string_copy(String *dest, String *src) {
    dest->len = src->len;
    dest->str = (char *)malloc(src->len + 1);
    strcpy(dest->str, src->str);
}

typedef struct {
    TokenType type;
    union {
        double number;
        String string;
        String identifier;
    };
} Token;

void print_token(Token *token) {
    switch (token->type) {
        // operators
        case token_plus:            printf("+\n"); break;
        case token_minus:           printf("-\n"); break;
        case token_asterisk:        printf("*\n"); break;
        case token_slash:           printf("/\n"); break;
        case token_modulo:          printf("%%\n"); break;
        case token_caret:           printf("^\n"); break;
        case token_octothorpe:      printf("#\n"); break;
        case token_equal_equal:     printf("==\n"); break;
        case token_not_equal:       printf("~=\n"); break;
        case token_less_equal:      printf("<=\n"); break;
        case token_greater_equal:   printf(">=\n"); break;
        case token_less:            printf("<\n"); break;
        case token_greater:         printf(">\n"); break;
        case token_equal:           printf("=\n"); break;
        case token_open_paren:      printf("(\n"); break;
        case token_close_paren:     printf(")\n"); break;
        case token_open_brace:      printf("{\n"); break;
        case token_close_brace:     printf("}\n"); break;
        case token_open_bracket:    printf("[\n"); break;
        case token_close_bracket:   printf("]\n"); break;
        case token_semicolon:      printf(";\n"); break;
        case token_colon:           printf(":\n"); break;
        case token_comma:           printf(",\n"); break;
        case token_dot:             printf(".\n"); break;
        case token_dot_dot:         printf("..\n"); break;   // concat
        case token_dot_dot_dot:     printf("...\n"); break;  // varargs

        // keywords
        case token_and:             printf("keyword: and\n"); break;
        case token_break:           printf("keyword: break\n"); break;
        case token_do:              printf("keyword: do\n"); break;
        case token_else:            printf("keyword: else\n"); break;
        case token_elseif:          printf("keyword: elseif\n"); break;
        case token_end:             printf("keyword: end\n"); break;
        case token_false:           printf("keyword: false\n"); break;
        case token_for:             printf("keyword: for\n"); break;
        case token_function:        printf("keyword: function\n"); break;
        case token_if:              printf("keyword: if\n"); break;
        case token_in:              printf("keyword: in\n"); break;
        case token_local:           printf("keyword: local\n"); break;
        case token_nil:             printf("keyword: nil\n"); break;
        case token_not:             printf("keyword: not\n"); break;
        case token_or:              printf("keyword: or\n"); break;
        case token_repeat:          printf("keyword: repeat\n"); break;
        case token_return:          printf("keyword: return\n"); break;
        case token_then:            printf("keyword: then\n"); break;
        case token_true:            printf("keyword: true\n"); break;
        case token_until:           printf("keyword: until\n"); break;
        case token_while:           printf("keyword: while\n"); break;

        // literals
        case token_number: 
            printf("number: %g\n", token->number);
            break;
        case token_string: 
            printf("string: \"%.*s\"\n", (int)token->string.len, token->string.str); 
            break;
        case token_identifier: 
            printf("identifier: %.*s\n", (int)token->identifier.len, token->identifier.str); 
            break;

        // special
        case token_eof: 
            printf("end of file\n"); 
            break;

        default: 
            printf("unknown token\n"); 
            break;
    }
}

// TODO: make these error report functions accept multiple arguments
void report_error(const char *error) {
    printf("error: %s\n", error);
}

void report_error_and_exit(const char *error) {
    printf("error: %s\n", error);
    exit(0);
}

static size_t start_char = 0;
static size_t next_char = 0;

char advance_char(char *code, long length) {
    if (next_char >= length) return '\0';
    return code[next_char++];
}

char peek_char(char *code, long length) {
    if (next_char >= length) return '\0';
    return code[next_char];
}

char peek_next_char(char *code, long length) {
    if (next_char + 1 >= length) return '\0';
    return code[next_char + 1];
}

int is_alpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

int is_numeric(char c) {
    return '0' <= c && c <= '9';
}

int is_alphanumeric(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_';
}

int main(int argc, char **argv) {
    if (argc != 2) report_error_and_exit("expected 1 file");

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) report_error_and_exit("couldn't open file");

    fseek(file, 0, SEEK_END);
    long file_size_in_bytes = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *code = (char *)malloc(file_size_in_bytes + 1);
    fread(code, 1, file_size_in_bytes, file);
    code[file_size_in_bytes] = '\0';

    // TODO: dynamic token amount
    Token tokens[1024];
    int token_count = 0;

    while (start_char < file_size_in_bytes) {
        start_char = next_char;
        char current_char = advance_char(code, file_size_in_bytes);

        switch (current_char) {
            case '+':
                tokens[token_count].type = token_plus;
                token_count++;
                break;
            case '-':
                if (peek_char(code, file_size_in_bytes)) {
                    while (advance_char(code, file_size_in_bytes) != '\n');
                } else {
                    tokens[token_count].type = token_minus;
                    token_count++;
                }
                break;
            case '*':
                tokens[token_count].type = token_asterisk;
                token_count++;
                break;
            case '/':
                tokens[token_count].type = token_slash;
                token_count++;
                break;
            case '%':
                tokens[token_count].type = token_modulo;
                token_count++;
                break;
            case '^':
                tokens[token_count].type = token_caret;
                token_count++;
                break;
            case '#':
                tokens[token_count].type = token_octothorpe;
                token_count++;
                break;
            case '=':
                if (peek_char(code, file_size_in_bytes) == '=') {
                    advance_char(code, file_size_in_bytes);
                    tokens[token_count].type = token_equal_equal;
                    token_count++;
                } else {
                    tokens[token_count].type = token_equal;
                    token_count++;
                }
                break;
            case '~':
                if (peek_char(code, file_size_in_bytes) == '=') {
                    advance_char(code, file_size_in_bytes);
                    tokens[token_count].type = token_not_equal;
                    token_count++;
                } else {
                    report_error("unexpected token '~'");
                }
                break;
            case '<':
                if (peek_char(code, file_size_in_bytes) == '=') {
                    advance_char(code, file_size_in_bytes);
                    tokens[token_count].type = token_less_equal;
                    token_count++;
                } else {
                    tokens[token_count].type = token_less;
                    token_count++;
                }
                break;
            case '>':
                if (peek_char(code, file_size_in_bytes) == '=') {
                    advance_char(code, file_size_in_bytes);
                    tokens[token_count].type = token_greater_equal;
                    token_count++;
                } else {
                    tokens[token_count].type = token_equal;
                    token_count++;
                }
                break;
            case '(':
                tokens[token_count].type = token_open_paren;
                token_count++;
                break;
            case ')':
                tokens[token_count].type = token_close_paren;
                token_count++;
                break;
            case '{':
                tokens[token_count].type = token_open_brace;
                token_count++;
                break;
            case '}':
                tokens[token_count].type = token_close_brace;
                token_count++;
                break;
            case '[':
                if (peek_char(code, file_size_in_bytes) == '[') {
                    advance_char(code, file_size_in_bytes);
                    if (peek_char(code, file_size_in_bytes) == '\n') advance_char(code, file_size_in_bytes);

                } else {
                    tokens[token_count].type = token_open_bracket;
                    token_count++;
                }
                break;
            case ']':
                tokens[token_count].type = token_close_bracket;
                token_count++;
                break;
            case ';':
                tokens[token_count].type = token_semicolon;
                token_count++;
                break;
            case ':':
                tokens[token_count].type = token_colon;
                token_count++;
                break;
            case ',':
                tokens[token_count].type = token_comma;
                token_count++;
                break;
            case '.':
                if (peek_char(code, file_size_in_bytes) == '.') {
                    advance_char(code, file_size_in_bytes);
                    if (peek_char(code, file_size_in_bytes) == '.') {
                        advance_char(code, file_size_in_bytes);
                        tokens[token_count].type = token_dot_dot_dot;
                        token_count++;
                    } else {
                        tokens[token_count].type = token_dot_dot;
                        token_count++;
                    }
                } else {
                    tokens[token_count].type = token_dot;
                    token_count++;
                }
                break;

            case '\0':
                tokens[token_count].type = token_eof;
                token_count++;
                break;

            case '\n':
            case '\r':
            case ' ':
            case '\t':
                continue;

            case '\'':
            case '"': {
                char quote = current_char;
                while (peek_char(code, file_size_in_bytes) != quote && next_char < file_size_in_bytes && peek_char(code, file_size_in_bytes) != '\n') {
                    if (advance_char(code, file_size_in_bytes) == '\\') next_char++;
                }
                if (peek_char(code, file_size_in_bytes) == quote) {
                    advance_char(code, file_size_in_bytes);

                    size_t len = next_char - start_char - 2;
                    char *str = (char *)malloc(len + 1);
                    str[len] = '\0';
                    strncpy(str, code + start_char + 1, len);
                    String string = string_create(str);
                    free(str);

                    tokens[token_count].type = token_string;
                    tokens[token_count].string = string;
                    token_count++;
                } else {
                    report_error("unterminated string");
                }
                break;
            }

            default:
                if (is_alpha(current_char)) {
                    while (is_alphanumeric(peek_char(code, file_size_in_bytes))) advance_char(code, file_size_in_bytes);
                    size_t len = next_char - start_char;

                    char *str = (char *)malloc(len + 1);
                    str[len] = '\0';
                    strncpy(str, code + start_char, len);
                    String identifier = string_create(str);

                    tokens[token_count].identifier = identifier;

                    if (!strcmp(str, "and")) {
                        tokens[token_count].type = token_and;
                    } else if (!strcmp(str, "break")) {
                        tokens[token_count].type = token_break;
                    } else if (!strcmp(str, "do")) {
                        tokens[token_count].type = token_do;
                    } else if (!strcmp(str, "else")) {
                        tokens[token_count].type = token_else;
                    } else if (!strcmp(str, "elseif")) {
                        tokens[token_count].type = token_elseif;
                    } else if (!strcmp(str, "end")) {
                        tokens[token_count].type = token_end;
                    } else if (!strcmp(str, "false")) {
                        tokens[token_count].type = token_false;
                    } else if (!strcmp(str, "for")) {
                        tokens[token_count].type = token_for;
                    } else if (!strcmp(str, "function")) {
                        tokens[token_count].type = token_function;
                    } else if (!strcmp(str, "if")) {
                        tokens[token_count].type = token_if;
                    } else if (!strcmp(str, "in")) {
                        tokens[token_count].type = token_in;
                    } else if (!strcmp(str, "local")) {
                        tokens[token_count].type = token_local;
                    } else if (!strcmp(str, "nil")) {
                        tokens[token_count].type = token_nil;
                    } else if (!strcmp(str, "not")) {
                        tokens[token_count].type = token_not;
                    } else if (!strcmp(str, "or")) {
                        tokens[token_count].type = token_or;
                    } else if (!strcmp(str, "repeat")) {
                        tokens[token_count].type = token_repeat;
                    } else if (!strcmp(str, "return")) {
                        tokens[token_count].type = token_return;
                    } else if (!strcmp(str, "then")) {
                        tokens[token_count].type = token_then;
                    } else if (!strcmp(str, "true")) {
                        tokens[token_count].type = token_true;
                    } else if (!strcmp(str, "until")) {
                        tokens[token_count].type = token_until;
                    } else if (!strcmp(str, "while")) {
                        tokens[token_count].type = token_while;
                    } else {
                        tokens[token_count].type = token_identifier;
                    }
                    free(str);

                    token_count++;
                } else if (is_numeric(current_char)) {
                    while (is_numeric(peek_char(code, file_size_in_bytes))) advance_char(code, file_size_in_bytes);

                    if (peek_char(code, file_size_in_bytes) == '.' && is_numeric(peek_next_char(code, file_size_in_bytes))) {
                        advance_char(code, file_size_in_bytes);
                        while (is_numeric(peek_char(code, file_size_in_bytes))) advance_char(code, file_size_in_bytes);
                    }

                    size_t len = next_char - start_char;
                    char *number_string = (char *)malloc(next_char - start_char);
                    strncpy(number_string, code + start_char, len);
                    number_string[len] = '\0';
                    double number = strtod(number_string, NULL);
                    free(number_string);

                    tokens[token_count].type = token_number;
                    tokens[token_count].number = number;
                    token_count++;
                } else {
                    // report_error_and_exit("unrecognized character 'TODO'");
                    printf("unrecognized character %c\n", current_char);
                }
                break;
        }
    }

    for (int i = 0; i < token_count; i++) {
        print_token(&tokens[i]);
    }

    fclose(file);
}
