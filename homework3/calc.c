#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

//Functions List
void skip_whitespace(char **str);
char peek(char **str);
char peek_num(char **str);
char parse_token(char **str);
double digit(char **str);
double give_digits(char **str);
double number_literal(char **str);
double parenthetical(char **str);
double multiplicative(char **str);
double additive(char **str);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }

    double solution = additive(&argv[1]);
    printf("%lf\n", solution);
}

//All functions
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

double digit(char **str) {
    char token = *str[0];
    double token_value = 0;
    if (token >= '0' && token <= '9' ) {
        //token_value = atoi(token);
        token_value = token - '0';
        // OR sscanf(token, "%lf", token_value);
    } else {
        printf("Invalid chracter: Expected a digit [0-9]"); //for debugging
    }
    return token_value;
}

double give_digits(char **str) {
    double value_left = 0;
    double value_right = 0;
    while (isdigit(peek(str) - '0') == 0) { //peek or peek_num? skip white spaces?
        printf("in give_digits while loop\n");
        if (peek(str) < '0' || peek(str) > '9') {
            printf("in break loop\n");
            break;
        }
        value_left = value_left * 10 + digit(str);
        (*str)++; //moves pointer to next digit
    } //exits loop if no digits- i.e., it should hit a decimal point

    if (peek(str) == '.') {
        char throwaway = parse_token(str);
        double divisor = 0.1;
        while (isdigit(peek(str))) {
            value_right = value_right + digit(str) * divisor;
            divisor /= 10.0;
        }
    }
    return value_left + value_right;
}

double number_literal(char **str) {
    if (peek_num(str) == ' ') {
        skip_whitespace(str);
    }
    int sign = 1; //assume positive value default
    if (peek_num(str) == '+' || peek_num(str) == '-') {
        if (peek_num(str) == '-') {
            sign = -1;
            char throwaway = parse_token(str); //move pointer and detect abrupt end
        }
    }
    double value = 0;
    if(isdigit(peek(str) - '0') == 0) {
        value = give_digits(str);
    }
    return sign * value;
}

double parenthetical(char **str) {
    double value = 0;
    if (peek(str) == '(') {
        char token = parse_token(str); //or can i just do skip_whitespace(str) and (*str)++?
        value = additive(str); //is this = or += because multiple operations???
        if (parse_token(str) != ')') {
            fprintf(stderr, "Parsing Error: Expected ')' but received '%c' instead.\n", parse_token(str));
            exit(1);
        }
    } else {
        value = number_literal(str);
    }
    return value;
}

double multiplicative(char **str) {
    double value = parenthetical(str);
    while (peek(str) == '*' || peek(str) == '/') {
        if (peek(str) == '*') {
            value *= parenthetical(str);
        } else {
            value /= parenthetical(str);
        }
    }
    return value;
}

double additive(char **str) {
    double value = multiplicative(str);
    while (peek(str) == '+' || peek(str) == '-') {
        if (peek(str) == '+') {
            value += multiplicative(str);
        } else {
            value -= multiplicative(str);
        }
    }
    return value;
}
