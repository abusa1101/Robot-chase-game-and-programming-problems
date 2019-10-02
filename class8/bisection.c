#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double polynomial(double x) {
    double func = (2 * x - 20 * x * x + 20 * x * x * x - 4 * x * x * x * x);
    return func;
}

void search(double x0, double x1) {
    if (polynomial(x0) * polynomial(x1) >= 0) {
        printf("Wrong input bounds\n");
    }
    double xmid = x0;
    double threshold = 1e-6;
    while ((x1 - x0) > threshold) {
        xmid = (x0 + x1) / 2;
        if (polynomial(xmid) == 0.0) {
            break;
        }
        if (polynomial(xmid) * polynomial(x0) < 0) {
            x1 = xmid;
        } else {
            x0 = xmid;
        }
        double ymid = polynomial(xmid);
        printf("x_mid: %11.4e y_mid: %11.4e\n", xmid, ymid);
    }
}

int main(int argc, char **argv) {
    if (argc != 3) { // Print error message if too many arguments
        printf("Wrong num of arguments\n");
        return 1;
    }
    double lower = atof(argv[1]);
    double higher = atof(argv[2]);
    search(lower, higher);
    return 0;
}
