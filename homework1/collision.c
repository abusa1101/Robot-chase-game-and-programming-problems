#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/*typedef struct object {
    int trans_x[16];
    int trans_y[16];
} p1_t;

typedef struct object {
    int trans_x[16];
    int trans_y[16];
} p2_t;*/


int main(void) {
    //PART 1: Read the csv file
    FILE *f = fopen("polygons.csv", "r");
    if (!f) {
        fprintf(stderr, "Error: Missing file.\n");
        return 1;
    }

    //while (fgetc(f) != '\n'){;
      for (int i = 0; i < 100; i++) {
          int c = fgetc(f);
          printf("character: %c\n", c);
          if (c == '\n'){
            break;
          }
          if (c == EOF) {
              fprintf(stderr, "unexpected EOF while reading file\n");
              exit(1);
          }
        }

        printf("character last: %d\n", fgetc(f)); // 1
        int fnum[100];

        while((fgetc(f) != EOF)) {
          int fnum = strtol(f, NULL, 10);
          printf("character: %d ", fnum);
        }

        int args_read = fscanf(f, "%d, %lf, %lf", &obj.int_value,
                               &obj.double_array[0], &obj.double_array[1]);
   // we can also check for the EOF (end of file) value from fgetc
    for (int i = 3; i < 6; i++) {
        int c = fgetc(f);
        if (c == EOF) {
            fprintf(stderr, "unexpected EOF while reading file\n");
            exit(1);
        }

        printf("character %d: %c\n", i, c); // ' ', 1, .
    }

    char rest_of_line[128];

    // use sizeof to not have to repeat 128
    if (!fgets(rest_of_line, sizeof(rest_of_line), f)) {
        fprintf(stderr, "failed to read rest of first line\n");
        exit(1);
    }
    printf("Remainder of that line was: %s\n", rest_of_line); // "0, 2.0"

    // fscanf does not care that the rest of the values are on different lines
    object_t obj = {0};
    int args_read = fscanf(f, "%d, %lf, %lf", &obj.int_value,
                           &obj.double_array[0], &obj.double_array[1]);
    if (args_read != 3) {
        fprintf(stderr, "could not read all three values!\n");
        exit(1);
    }*/



    //PART 2: Store read data. d[i] = first data point, d[values-1] = last data point
    int n =
    printf("%d\n", n);
    double xpivot = 0; //constant
    double ypivot = 0; //constant

    //Read n points and display error msg if n > 16
    if (n > 16) {
        fprintf(stderr, "Error: Too many points (n < 16)\n");
        return 1;
    }

    for(int i = 0; i < 200; i++){
      printf("%d ", d[i]);
    }

    //POLYGON 1 DATA
    double p1_xglobal =
    double p1_yglobal =
    double p1_degrot =
    double p1_radrot = p1_degrot * 22 / (7 * 180);
    double p1_x[n];
    double p1_y[n];

    //POLYGON 2 DATA
    double p2_xglobal =
    double p2_yglobal =
    double p2_degrot =
    double p2_radrot = p2_degrot * 22 / (7 * 180);
    double p2_x[n];
    double p2_y[n];

    //Part 3: Apply global transformation to all points
    double p1_transformed_x[n];
    double p1_transformed_y[n];
    double p2_transformed_x[n];
    double p2_transformed_y[n];

    for (int i = 0; i < n; i++) {
        //Polygon 1: Global transformation
        double p1_xrotated = xpivot + ((p1_x[i] - xpivot) * cos(p1_radrot)
                             - p1_y[i] - ypivot * sin(p1_radrot));
        double p1_yrotated = ypivot + ((p1_x[i] - xpivot) * sin(p1_radrot)
                             + p1_y[i] - ypivot * cos(p1_radrot));
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
    if (collision == 1 || contained_collision == 1) {
        printf("collision!\n");
    } else {
        printf("no collision\n");
    }

    return 0;
}
