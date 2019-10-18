//at this point there should only be digits I think?
// //int istheredecimalpoint = 0; //dont really need this right?
// double value = 0;
// double value2 = 0;
// //in below while condition, what is the stopping condition? ' ' or ')' or '!isdigit'
// while (peek_num(str) != '.' || peek_num(str) != ')') { //will it still get stuck in while loop?
//     if (isdigit(peek(str)) { //am i checking for error/invalid digits here or in checkdigits??
//         value = checkdigit(str);
//     }
//     //istheredecimalpoint = 1;
// }
// //will there be white spaces after the decimal point?
// if (isdigit(peek(str)) { //after dp, if there's more digits get that value as well
//     value2 = checkdigit(str);
// }
// return value + value2;


//give digits
//do i check for invalid digits here? or in number_literal?
// if (peek_num(str) == ' ') { //remove any white spaces before the digits start?
//     skip_whitespace(str);
// }
// if (!isdigit(str)) { //if input is not a digit
//     return 0; }
// double token_value = 0;
// while (peek_num(str) == ' ' || isdigit(str)) {
//     char token = *str[0];
//     if (token >= '0' && token <= '9' ) {
//     token_value = (double) token - '0'; //does this work or do i just make it int type?
//     while (token >= '0' && token <= '9') {
//         token_value += token_value * 10;
//         (*str)++;
//     }
//     return token_value;
// }
