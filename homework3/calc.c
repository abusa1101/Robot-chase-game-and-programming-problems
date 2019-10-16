#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

// int isalpha(int c): This function checks whether the passed character is alphabetic.
// int isdigit(int c): This function checks whether the passed character is decimal digit.

void skip_whitespace(char **str) {
    while (isspace(*str[0])) {
        (*str)++;
    }
}

char peek(char **str) {
    skip_whitespace(str);
    return *str[0];
}

char peek_num(char **str) {
    //skip_whitespace(str);
    return *str[0];
}

char parse_token(char **str) {
    skip_whitespace(str);
    char token = *str[0];
    if (!token) {
        fprintf(stderr, "Parsing Error: Unexpected end of string\n");
        exit(1);
    }
    (*str)++;
    return token;
}

double additive(char **str) {
    double value = multiplicative_expression(str);
    while (peek(str) == '+' || peek(str) = '-') {
        if (peek(str) == '+') {
            value += multiplicative_expression(str);
        } else {
            value -= multiplicative_expression(str);
        }
    }
    return value;
}

double multiplicative(char **str) {
    double value = parenthetical_expression(str);
    while (peek(str) == '*' || peek(str) = '/') {
        if (peek(str) == '*') {
            value *= parenthetical_expression(str);
        } else {
            value /= parenthetical_expression(str);
        }
    }
    return value;
}

double parenthetical(char **str) {
    double value = 0;
    if (peek(str) == '(') {
        char token = parse_token(str); //or can i just do skip_whitespace(str) and (*str)++?
        value = additive(str);
        if (parse_token(str) != ')') {
            fprintf(stderr, "Parsing Error: Expected ')' but received '%c' instead.\n", parse_token(str));
            exit(1);
        }
    } else {
        value = number_literal(str);
    }
    return value;
}

double number_literal(char **str) {
    // char whatsign = sign(input_char);
    // if (whatsign == '') {
    //     double isdig = digit(input_char);
    //     if !digit {
    //         printf("Error: Number literal");
    //     }
    // }
    if (peek_num(str) == ' ') {
        skip_whitespace(str);
    }
    int sign = 0;
    if (peek_num(str) == '+' || peek_num(str) == '-') {
        if (peek_num(str) == '+') { sign = 1; (*str)++; }
        if (peek_num(str) == '-') { sign = -1; (*str)++; }
    }

    //CASE 1: DECIMAL POINT

    //CASE 2: NO DECIMAL POINT

    if


}

// char checksign(input_char) {
//     if (input_char == '+') {
//         return '+';
//     } else if (input_char == '-') {
//         return '-';
//     }
//     return '@';
// }

int checkdigit(input_char) {
    if (isdigit(input_char) == 0) {
        return atoi(input_char);
    } else {
        fprintf(stderr, "Error: Found alphabet instead of digit in 'Digit'\n");
        exit(1);
    }
    return 0;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }

    double solution = additive(argv[1]);
    printf("%lf\n", solution);
}
