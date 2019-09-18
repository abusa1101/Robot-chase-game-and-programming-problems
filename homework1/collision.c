#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(void) {
    char c[100];
    double d[200]; //all data array

    //PART 1: Read the csv file
    FILE *f = fopen("polygons.csv", "r");

    if (!f) {
        fprintf(stderr, "Error: Missing file.\n");
        return 1;
    }

    int invalid = 0;
    int args_read;
    int m = 1; //m for mask
    char skipif = '0';
    while (skipif != '\n') {
        args_read = fscanf(f, "%c", &c[m]);
        skipif = c[m];
        m++;
        if (args_read != 1) {
            invalid = 1;
        }
    }

    int values = m;

    while (values < 200) {
        int args_read = fscanf(f, "%lf", &d[values]);
        if (values < 14) {
            if (args_read != 1) {
                invalid = 1;
            }
        }
        if (args_read != 1) {
            break;
        }
        values++;
    }

    if (invalid == 1) {
        printf("Error: Invalid file.\n");
        //return 1;
    }
    //PART 2: Store read data. d[i] = first data point, d[values-1] = last data point
    int n = d[m + 3];
    double xpivot = 0; //constant
    double ypivot = 0; //constant

    //Read n points and display error msg if n > 16
    if (n > 16) {
        fprintf(stderr, "Error: Too many points (n < 16)\n");
        return 1;
    }

    //POLYGON 1 DATA
    double p1_xglobal = d[m];
    double p1_yglobal = d[m + 1];
    double p1_degrot = d[m + 2];
    double p1_radrot = p1_degrot * 22 / (7 * 180);
    double p1_x[n];
    double p1_y[n];
    int j = 0;
    for (int k = 4; k < (2 * n) - 1; k++) {
        p1_x[j] = d[m + k];
        j++;
    }
    j = 0;
    int k;
    for (k = (4 + n); k < (2 * n) + (n - 1); k++) {
        p1_y[j] = d[m + k];
        j++;
    }

    //POLYGON 2 DATA
    double p2_xglobal = d[m + k];
    double p2_yglobal = d[m + k + 1];
    double p2_degrot = d[m + k + 2];
    double p2_radrot = p2_degrot * 22 / (7 * 180);
    double p2_x[n];
    double p2_y[n];
    j = 0;
    for (int k2 = k + 4; k2 < (2 * n) - 1 + k; k2++) {
        p2_x[j] = d[m + k2];
        j++;
    }
    j = 0;
    int k2;
    for (k2 = (k + 4 + n); k2 < (2 * n) + (n - 1) + k; k2++) {
        p2_y[j] = d[m + k2];
        j++;
    }
    //Part 3: Apply global transformation to all points
    double p1_transformed_x[n];
    double p1_transformed_y[n];
    double p2_transformed_x[n];
    double p2_transformed_y[n];

    for (int i = 0; i < n; i++) {
        //Polygon 1: Global transformation
        double aa = (p1_x[i] - xpivot) * cos(p1_radrot);
        double p1_xrotated = xpivot + (aa - p1_y[i] - ypivot * sin(p1_radrot));
        double bb = (p1_x[i] - xpivot) * sin(p1_radrot);
        double p1_yrotated = ypivot + (bb + p1_y[i] - ypivot * cos(p1_radrot));
        double p1_xtrans = p1_xrotated + p1_xglobal;
        double p1_ytrans = p1_yrotated + p1_yglobal;
        p1_transformed_x[i] = p1_xtrans;
        p1_transformed_y[i] = p1_ytrans;

        //Polygon 2: Global transformation
        double p2_xrotated = xpivot + ((p2_x[i] - xpivot) * cos(p2_radrot)
                             - (p2_y[i] - ypivot) * sin(p2_radrot));
        double p2_yrotated = ypivot + ((p2_x[i] - xpivot) * sin(p2_radrot)
                             + (p2_y[i] - ypivot) * cos(p2_radrot));
        double p2_xtrans = p2_xrotated + p2_xglobal;
        double p2_ytrans = p2_yrotated + p2_yglobal;
        p2_transformed_x[i] = p2_xtrans;
        p2_transformed_y[i] = p2_ytrans;
    }

    //Part 4: Collision algorithm!
    int collision = 0;  //collision condition
    int l1_opp = 0;     //cp condition: 0 = same side, 1 = opp side, 2 = zero vector
    int l2_opp = 0;

    for (int u = 0; u < n; u++) {
        double line1_x = p1_transformed_x[u + 1] - p1_transformed_x[u];
        double line1_y = p1_transformed_y[u + 1] - p1_transformed_y[u];
        for (int v = 0; v < n; v++) {
            double line2_x = p2_transformed_x[v + 1] - p2_transformed_x[v];
            double line2_y = p2_transformed_y[v + 1] - p2_transformed_y[v];
            //for each point p in the other line-2 points, compute the cross product between l and p
            //cp for line1 and points from line 2
            double a = (line1_x * p2_transformed_y[u]);
            double b = (line1_y * p2_transformed_x[u]);
            double cp1_1 = a - b;
            double c = (line1_x * p2_transformed_y[u + 1]);
            double d = (line1_y * p2_transformed_x[u + 1]);
            double cp1_2 = c - d;
            //cp for line 2 and points from line 1
            double e = (line2_x * p1_transformed_y[u]);
            double f = (line2_y * p1_transformed_x[u]);
            double cp2_1 = e - f;
            double g = (line2_x * p1_transformed_y[u + 1]);
            double h = (line2_y * p1_transformed_x[u + 1]);
            double cp2_2 = g - h;
            //check for likeness or signs of crossproduct
            //line 1 and line 2 points
            if (cp1_1 > 0 & cp1_2 < 0) {
                l1_opp = 1;
            } else if (cp1_1 < 0 & cp1_2 > 0) {
                l1_opp  = 1;
            } else if (cp1_1 == 0 | cp1_2 == 0) {
                l1_opp  = 2;
            } else {
                l1_opp  = 0;
            }
            //line 2 and line 1 points
            if (cp2_1 > 0 & cp2_2 < 0) {
                l2_opp = 1;
            } else if (cp2_1 < 0 & cp2_2 > 0) {
                l2_opp = 1;
            } else if (cp2_1 == 0 | cp2_2 == 0) {
                l2_opp = 2;
            } else {
                l2_opp = 0;
            }

            //check for collision!
            if (l1_opp == 1 && l2_opp == 1) { //both cp checks = opposite signs
                collision = 1;
            } else if (l1_opp == 1 || l2_opp == 1) {
                collision = 1;
            } else if (l1_opp == 2 || l2_opp == 2) {
                collision = 0;
            } else if (l1_opp == 2 && l2_opp == 2) {
                collision = 0;
            } else {
                collision = 0;
            }
        }
    }

    //Part 6: Check Failure Mode! i.e. check if either polygon contains the other
    //POLYGON 1: CONTAINMENT
    int p1_sign[n]; //0 = zero, 1 = positive, 2 = negative
    int p1_previous_sign;
    int contained_collision = 0;
    for (int u = 0; u < n; u++) {
        double line1_x = p1_transformed_x[u + 1] - p1_transformed_x[u];
        double line1_y = p1_transformed_y[u + 1] - p1_transformed_y[u];
        //for one p in the other line, compute the cross product between l and p
        double cp1_1 = (line1_x * p2_transformed_y[u]) - (line1_y * p2_transformed_x[u]);
        //check for likeness or signs of crossproduct
        if (cp1_1 > 0) {
            p1_sign[u] = 1;
        } else if (cp1_1 < 0) {
            p1_sign[u] = 2;
        } else {
            p1_sign[u] = 0;
        }
        if (u == 1) {
            p1_previous_sign = p1_sign[u];
        }
        if (p1_previous_sign == p1_sign[u]) {
            contained_collision = 0;
            //collision = 1;
        } else {
            contained_collision = 1;
        }
        p1_previous_sign = p1_sign[u];
    }

    //POLYGON 2: CONTAINMENT
    int p2_sign[n]; //0 = zero, 1 = positive, 2 = negative
    int p2_previous_sign;
    for (int v = 0; v < n; v++) {
        double line2_x = p2_transformed_x[v + 1] - p2_transformed_x[v];
        double line2_y = p2_transformed_y[v + 1] - p2_transformed_y[v];
        //for one p in the other line, compute the cross product between l and p
        double cp2_1 = (line2_x * p1_transformed_y[v]) - (line2_y * p1_transformed_x[v]);
        //check for likeness or signs of crossproduct
        if (cp2_1 > 0) {
            p2_sign[v] = 1;
        } else if (cp2_1 < 0) {
            p2_sign[v] = 2;
        } else {
            p2_sign[v] = 0;
        }
        if (v == 1) {
            p2_previous_sign = p2_sign[v];
        }
        if (p2_previous_sign == p2_sign[v]) {
            contained_collision = 0;
            //collision = 1;
        } else {
            contained_collision = 1;
        }
        p2_previous_sign = p2_sign[v];
    }

    //Part 7: Declare collision or not!!
    if (collision == 1 && contained_collision == 1) {
        printf("collision!\n");
    } else {
        printf("no collision\n");
    }

    return 0;
}
