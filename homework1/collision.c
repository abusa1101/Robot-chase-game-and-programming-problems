#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main(void) {
    char c[100];
    double d[200]; //all data array

    //PART 1(a): Read the csv file 
    FILE *f = fopen("polygons.csv", "r");

    if (!f){
        fprintf(stderr, "Error: Missing file.\n");
        return 1;
    }

    int args_read;
    int m = 1; //m for mask
    char skipif;
    while (skipif != '\n'){
        int args_read = fscanf(f, "%c", &c[m]);
        //printf("%c\n", c[i]);
        skipif = c[m];
        m++;
    }
    //printf("%d\n", i);
    int values = m;
    while ( values < 200){
        int args_read = fscanf(f, "%lf", &d[values]);
        if(args_read != 1){
            break;
        }
        //printf("%lf\n", d[values]);
        values++;
    }

    //for (int k = 0; k<200; k++){
       // printf("%lf\n", d[k]);
        //printf("%d\n", k);
    //}
    //printf("%d\n", m);
    //printf("%lf\n", d[m+3]);
    //printf("%lf\n", d[values-1]);

    //PART 1(b): Store read data into discernable variables/arrays. d[i] = first data point, d[values-1] = last data point
    int n = d[m+3];

    if( typeof(n) != 'double'){
        fprintf(stderr, "Error: Invalid file.\n");
        return 1;
    }

    double xpivot = 0; //constant
    double ypivot = 0; //constant
    //POLYGON 1 DATA
    double p1_xglobal = d[m]; 
    double p1_yglobal = d[m + 1]; 
    double p1_degrot = d[m + 2]; 
    double p1_radrot = p1_degrot*22/(7*180);
    double p1_x[n];
    double p1_y[n];
    int j = 1;
    for (int k = 4; k < (2*n)-1; k++){
        p1_x[j] = d[m + k];
        //printf("%lf\n", p1_x[j]);
        j++;
    }
    j = 1;
    int k;
    for (k = (4 + n); k < (2*n)+(n-1); k++){
        p1_y[j] = d[m + k];
        //printf("%lf\n", p1_y[j]);
        j++;
    }
    //POLYGON 2 DATA
    double p2_xglobal = d[m + k]; 
    double p2_yglobal = d[m + k + 1]; 
    double p2_degrot = d[m + k + 2]; 
    double p2_radrot = p2_degrot*22/(7*180);
    double p2_x[n];
    double p2_y[n];
    j = 1;
    for (int k2 = k + 4; k2 < (2*n)-1 + k; k2++){
        p2_x[j] = d[m + k2];
        //printf("%lf\n", p2_x[j]);
        j++;
    }
    j = 1;
    int k2;
    for (k2 = (k + 4 + n); k2 < (2*n)+(n-1) + k; k2++){
        p2_y[j] = d[m + k2];
        //printf("%lf\n", p2_y[j]);
        j++;
    }

    //Part 2: Read n points and display error msg if n > 16
    if (n > 16){
        fprintf(stderr, "Error: Too many points (n < 16)\n");
        return 1;
    }

    //Part 3: Apply global transformation to all points
    double p1_rotated_x[n];
    double p1_rotated_y[n];
    double p2_rotated_x[n];
    double p2_rotated_y[n];
    double p1_transformed_x[n];
    double p1_transformed_y[n];
    double p2_transformed_x[n];
    double p2_transformed_y[n];

    for (int i = 1; i <= n; i++){ 
        //Polygon 1: Global rotation
        double p1_xshifted = p1_x[i] - xpivot; 
        double p1_yshifted = p1_y[i] - ypivot; 
        double p1_xrotated = xpivot + (p1_xshifted*cos(p1_radrot) - p1_yshifted*sin(p1_radrot)); 
        double p1_yrotated = ypivot + (p1_xshifted*sin(p1_radrot) + p1_yshifted*cos(p1_radrot));
        p1_rotated_x[i] = p1_xrotated; //1.can also replace original poly_x with updated transformation values
        p1_rotated_y[i] = p1_yrotated; //2.Not doing that rn for debugging purposes

        //Polygon 2: Global rotation
        double p2_xshifted = p2_x[i] - xpivot; 
        double p2_yshifted = p2_y[i] - ypivot; 
        double p2_xrotated = xpivot + (p2_xshifted*cos(p2_radrot) - p2_yshifted*sin(p2_radrot)); 
        double p2_yrotated = ypivot + (p2_xshifted*sin(p2_radrot) + p2_yshifted*cos(p2_radrot));
        p2_rotated_x[i] = p2_xrotated; //1.can also replace original poly_x with updated transformation values
        p2_rotated_y[i] = p2_yrotated; //2.Not doing that rn for debugging purposes

        //Polygon 1: Global translation
        double p1_xtrans = p1_rotated_x[i] + p1_xglobal; 
        double p1_ytrans = p1_rotated_y[i] + p1_yglobal;
        p1_transformed_x[i] = p1_xtrans;
        p1_transformed_y[i] = p1_ytrans;

        //Polygon 2: Global translation
        double p2_xtrans = p2_rotated_x[i] + p2_xglobal; 
        double p2_ytrans = p2_rotated_y[i] + p2_yglobal;
        p2_transformed_x[i] = p2_xtrans;
        p2_transformed_y[i] = p2_ytrans;

        //printf("(%lf, %lf) \n", rotated_x[i], rotated_y[i]); 
        //printf("(%lf, %lf) \n", p1_transformed_x[i], p1_transformed_y[i]); 
        //printf("\n(%lf, %lf) \n", p2_transformed_x[i], p2_transformed_y[i]); 
    }

    //Part 4: Make an array of lines, slopes, y-intercepts for both polygons
    // double lx_p1[n] = 0;
    // double ly_p1[n] = 0;
    // double lx_p2[n];
    // double ly_p2[n];
    double m_p1[n]; //slopes
    double m_p2[n];
    double c_p1[n]; //y intercepts
    double c_p2[n];

    for (int j = 1; j <= n; j++ ){ 
        if (j == n){
            // lx_p1[j] = p1_transformed_x[1] - p1_transformed_x[j]; //set last line endpoint to fist line start point
            // ly_p1[j] = p1_transformed_y[1] - p1_transformed_y[j];

            // Polygon 1: make arrays of slopes, y-intercepts
            m_p1[j] = (p1_transformed_y[j+1] - p1_transformed_y[j])/(p1_transformed_x[j + 1] - p1_transformed_x[j]); //slope of last line
            c_p1[j] = p1_transformed_y[j] - (m_p1[j] * p1_transformed_x[j]); // y intercept of last line
            // Polygon 2: make arrays of slopes, y-intercepts
            m_p2[j] = (p2_transformed_y[j+1] - p2_transformed_y[j])/(p2_transformed_x[j + 1] - p2_transformed_x[j]); 
            c_p2[j] = p2_transformed_y[j] - (m_p2[j] * p2_transformed_x[j]); 
        }

        //lx_p1[j] = p1_transformed_x[j+1] - p1_transformed_x[j];
        //ly_p1[j] = p1_transformed_y[j+1] - p1_transformed_y[j];

        // Polygon 1: make arrays of slopes, y-intercepts
        m_p1[j] = (p1_transformed_y[j+1] - p1_transformed_y[j])/(p1_transformed_x[j+1] - p1_transformed_x[j]);
        c_p1[j] = p1_transformed_y[j] - (m_p1[j] * p1_transformed_x[j]);
        // Polygon 2: make arrays of slopes, y-intercepts
        m_p2[j] = (p2_transformed_y[j+1] - p2_transformed_y[j])/(p2_transformed_x[j+1] - p2_transformed_x[j]);
        c_p2[j] = p2_transformed_y[j] - (m_p2[j] * p2_transformed_x[j]);

        //printf("(%lf, %lf) \n", m_p1[j], m_p2[j]); 
        //printf("(%lf, %lf) \n", c_p1[j], c_p2[j]); 
    }


    //Part 5: Collision algorithm!
    int collision = 0; //collision condition
    int l1_opp = 0; //cp condition: 0 = same side, 1 = opp side, 2 = zero vector
    int l2_opp = 0;
    double slope_p1[n];
    double slope_p2[n];

    for (int u = 1; u <= n; u++ ) {

        if (u == n){
        double line1_x = p1_transformed_x[1] - p1_transformed_x[u]; 
        double line1_y = p1_transformed_y[1] - p1_transformed_y[u];
        slope_p1[u] = (line1_y)/(line1_x);
        }
        double line1_x = p1_transformed_x[u+1] - p1_transformed_x[u];
        double line1_y = p1_transformed_y[u+1] - p1_transformed_y[u];
        slope_p1[u] = (line1_y)/(line1_x);

        for (int v = 1; v <= n; v++ ) {

            if (v == n){
                double line2_x = p2_transformed_x[1] - p2_transformed_x[v]; 
                double line2_y = p2_transformed_y[1] - p2_transformed_y[v];
                slope_p2[u] = (line2_y)/(line2_x);
            }
            double line2_x = p2_transformed_x[v+1] - p2_transformed_x[v];
            double line2_y = p2_transformed_y[v+1] - p2_transformed_y[v];
            slope_p2[u] = (line2_y)/(line2_x);

            if (slope_p1[u] == slope_p2[v]) { //Do l1 and l2 intersect?
                //printf("intersect\n");
                //collision = 1;
            } else {
                //printf("DO NOT intersect\n");
            }
            //for (int w = 1; w < 3; w++ ) 
                //for (int p = 0; p < n; p++ ) { //for each point p in the other line-2 points
                    //compute the cross product between l and p
                    //cp for line1 and points from line 2
                    double cp1_1 = (line1_x * p2_transformed_y[u]) - (line1_y * p2_transformed_x[u]);
                    double cp1_2 = (line1_x * p2_transformed_y[u+1]) - (line1_y * p2_transformed_x[u+1]);
                    //cp for line 2 and points from line 1
                    double cp2_1 = (line2_x * p1_transformed_y[u]) - (line2_y * p1_transformed_x[u]);
                    double cp2_2 = (line2_x * p1_transformed_y[u+1]) - (line2_y * p1_transformed_x[u+1]);
                //}

                //check for likeness or signs of crossproduct
                //line 1 and line 2 points
                if (cp1_1 > 0 & cp1_2 < 0) { //if the two cross products have opposite sign, or one of them is zero
                    //then they were on opposite sides of/touching the line l that extends to infinity
                    l1_opp = 1;
                } 
                else if (cp1_1 < 0 & cp1_2 > 0) {
                    l1_opp  = 1;
                }
                else if (cp1_1 == 0 | cp1_2 == 0){
                    l1_opp  = 2;
                } else {
                    l1_opp  = 0;
                }
                //line 2 and line 1 points
                if (cp2_1 > 0 & cp2_2 < 0) { //if the two cross products have opposite sign, or one of them is zero
                    l2_opp = 1;
                } 
                else if (cp2_1 < 0 & cp2_2 > 0) {
                    l2_opp = 1;
                }
                else if (cp2_1 == 0 | cp2_2 == 0){
                    l2_opp = 2;
                } else {
                    l2_opp = 0;
                }
            //}
            //check for collision!
            if (l1_opp == 1 && l2_opp == 1) { //if both cross product checks above indicated opposite signs,
                    //printf("Actual lines intersect!!\n"); //then the actual lines (not extending to infinity) intersect,
                    collision = 1;
            } else if (l1_opp == 2 || l2_opp == 2) { //EXCEPT when both checks (for lines l1 and l2) have at least one zero cross product
                    //printf("There is atleast one zero cross product");
                    collision = 1;
            } else if (l1_opp == 2 && l2_opp == 2){
                    //printf("There is atleast one zero cross product");
                    collision = 0;
            } else {
                //printf("Actual lines DO NOT intersect\n");
                collision = 0;
            }
        }
    }


    /*//Part 6: Check Failure Mode! i.e. check if either polygon contains the other
        for each polygon pg in [polygon 1, polygon 2] {
            // compute if pg contains a point p of the other polygon
            for each line l in pg {
                //compute the cross product between l and p
            }
            // the sign of each cross product indicates the side the point lies on.
            // the lines in the polygon must have a consistent winding order (clockwise or counter)
            // for the signs to consistently indicate inside or outside of the polygon.
            if all the cross products have the same sign{
                //then the polygon contains point p
            }
        }*/
    
    //Part 7: Declare collision or not!! 
    if (collision == 1){
        printf("collision!\n");
    } else{
        printf("no collision\n");
    }
    return 0;
}

