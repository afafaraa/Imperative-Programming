#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (*Func1vFp)();

double max_val(double x, double y){
    if(x>y) return x;
    return y;
}

double min_val(double x, double y){
    if(x>y) return y;
    return x;
}
// example functions of one variable
double f_poly(double x) {
    return 2*pow(x,5)-4*pow(x,4)+3.5*pow(x,2)+1.35*x-6.25;
}

double f_rat(double x) {
    return 1/(pow((x-0.5),2)+0.01);
}

double f_exp(double x) {
    return 2*pow(exp(1),-1.5*x)*x-1;
}

double f_trig(double x) {
    return x*tan(x);
}

// Quadratures
// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
    return (b-a)*f1(a);
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
    return (b-a)*f1(b);
}

// rectangle rule, midpoint
// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
    return (b-a)*f1((b+a)/2);
}

// trapezoidal rule
double quad_trap(Func1vFp f1, double a, double b, int n) {
    return (b-a)/2*(f1(a)+f1(b));
}

// Simpson's rule
double quad_simpson(Func1vFp f1, double a, double b, int n) {
    return (b-a)/6*(f1(b)+4*f1((a+b)/2)+f1(a));;
}

// pointer to quadrature function
typedef double (*QuadratureFp)();

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
	quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
    Func1vFp fun=func_tab[fun_no];
    Func1vFp quad=quad_tab[quad_no];
    double Q=0;
    double h=fabs((b-a)/n);
    double l,r;
    l=a;
    r=a+h;
    for(int i=0;i<n;i++){
        Q=Q+quad(fun,l,r,n);
        r+=h;
        l+=h;
    }
    return Q;

}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
    double s1=quad(f,a,(a+b)/2,1), s2=quad(f,(a+b)/2,b,1);
    //printf("%d %lf \n",level,S);
    if(level==RECURS_LEVEL_MAX){
        return NAN;
    }
    if(fabs(s1+s2-S)<=delta){
        return s1+s2;
    }
    s1=recurs(f, a, (a+b)/2, s1, delta/2, quad, level+1);
    s2=recurs(f, (a+b)/2, b, s2, delta/2, quad, level+1);
    return s1+s2;
}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
    double S=quad(f,a,b,1), level=0;
    return recurs(f,a,b,S,delta,quad,level);
}

// double integrals

// pointer to function of two variables
typedef double (*Func2vFp)(double, double);

double func2v_2(double x, double y) {
	return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x){
	return 0.7*exp(-2*x*x);
}

double upper_bound2(double x) {
	return sin(10*x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
    double range_x=fabs(x1-x2)/nx, range_y=fabs(y1-y2)/ny;
    double integral=0,curr=0, left_x=x1,left_y=y1;
    for(int i=0; i<nx; i++){
        for(int j=0; j<ny; j++){
            curr=f(left_x,left_y);
            integral+=curr;
            left_y+=range_y;
        }
        left_y=y1;
        left_x+=range_x;
    }
    return integral*range_x*range_y;
}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy, Func1vFp fg, Func1vFp fh) {
    double left_x=x1, hx=fabs((x1-x2)/nx), left_y=0;
    double integral=0,temp1=0,temp2=0;
    int ny=0;
    for(int i=0; i<nx; i++){
        temp1=left_x+hx/2;
        ny=round((fh(temp1)-fg(temp1))/hy);
        left_y=fg(temp1);
        //printf("%lf %d\n",left_y,ny);
        for(int j=0; j<ny; j++){
            temp2=left_y+hy/2;
            integral+=f(temp1,temp2);
            left_y+=hy;
            //printf("%lf\n",curr_vol);
        }
        left_x+=hx;
    }
    return integral*hy*hx;
}

// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis

double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny, Func1vFp fg, Func1vFp fh)  {
    double hx=fabs(x1-x2)/nx, hy=(y2-y1)/ny, integral=0, left_x=x1, top_y, bottom_y, new_ny, new_hy, y_temp;
    for(int i=0; i <nx; i++) {
        top_y = min_val(y2,fh(left_x));
        bottom_y = max_val(y1,fg(left_x));

        new_ny=ceil((top_y-bottom_y)/hy);
        new_hy=(top_y - bottom_y)/new_ny;

        y_temp=max_val(y1, fg(left_x));
        for(int j=0; j<new_ny; j++) {
            integral+=new_hy * f(left_x, y_temp);
            y_temp+=new_hy;
        }
        left_x+=hx;
    }
    return hx*integral;
}
double dbl_integr_normal_nq(Func2vFp f, double x1, double x2, int nx, double y1, double y2,int ny, Func1vFp fg, Func1vFp fh){
    double left_x=x1, hx=fabs(x1-x2)/(double)(nx), left_y=0, hy=0, integral=0;
    for(int i=0; i<nx; i++){
        if(fh(left_x)>fg(left_x)){
            hy=fabs(min_val(fh(left_x),y2)-max_val(fg(left_x),y1))/(double)(ny);
            left_y=max_val(fg(left_x),y1);
            for(int j=0; j<ny; j++){
                if(left_y>=y2 || left_y>=fh(left_x)) break;
                integral+=f(left_x,left_y)*hy;
                left_y+=hy;
            }
        }
        left_x+=hx;
    }
    return integral*hx;
}
double dbl_integr_normal_n1(Func2vFp f, double x1, double x2, int nx, double y1, double y2,int ny, Func1vFp fg, Func1vFp fh){
    double left_x=x1, hx=fabs(x1-x2)/nx, left_y=0,hy=0;
    double integral=0,curr_area=0,curr_vol=0;
    //printf("%lf \n",hx);
    hy=fabs(y2-y1)/ny;
    for(int i=0; i<nx; i++){
        //printf("%lf %lf\n",fg(left_x),fh(left_x));
        //printf("%lf",left_x);
        if(fh(left_x)>fg(left_x)){
            //printf("%lf %lf\n",fg(left_x),fh(left_x));
            left_y=max_val(fg(left_x),y1);
            for(int j=0; j<ny; j++){
                if(left_y>=y2 || left_y>=fh(left_x)) break;
                integral+=f(left_x,left_y);
                left_y+=hy;
            }
        }
        left_x+=hx;
    }
    return integral*hx*hy;
}

// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
	return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
	return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
	double fv = 1.;
	for (int i = 1; i < n; ++i) {
		fv += sin(i*v[i]);
	}
	return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
	double r = 0.0;
	for (int i = 0; i < n; ++i) r += (v[i]-1)*(v[i]-1);
	return r <= 1.;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)
double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary, int flag) {
    //printf("%d\n",flag);
    double hx=fabs(variable_lim[0][0]-variable_lim[0][1])/(double)(tn[0]);
    double hy=fabs(variable_lim[1][0]-variable_lim[1][1])/(double)(tn[1]);
    double hz=fabs(variable_lim[2][0]-variable_lim[2][1])/(double)(tn[2]);
    double x1=variable_lim[0][0], y1=variable_lim[1][0], z1=variable_lim[2][0];
    double integral=0;
    double vec[]={0,0,0};
    for(int i=0; i<tn[0]; i++){
        y1=variable_lim[1][0];
        for(int j=0; j<tn[1]; j++){
            z1=variable_lim[2][0];
            for(int k=0; k<tn[2]; k++){
                vec[0]=x1+hx;
                vec[1]=y1+hy;
                vec[2]=z1+hz;
                if(flag==1){
                    if(boundary(vec,3)!=NULL)
                        integral+=f(vec,3);
                }
                else
                    integral+=f(vec,3);
                z1+=hz;
            }
            y1+=hy;
        }
        x1+=hx;
    }
    return integral*hx*hy*hz;
}


// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)
void recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[],double variable_lim[][2], const int tn[], int level, BoundNvFp boundary) {
    if(level==variable_no){
        if(boundary(t_variable,variable_no)!=NULL)
            *p_sum+=f(t_variable,variable_no);
        return;
    }
    t_variable[level]=variable_lim[level][0]+(fabs(variable_lim[level][0]-variable_lim[level][1])/tn[level])/2;
    for(int i=0; i<tn[level]; i++){
        recur_quad_rect_mid(p_sum,f,variable_no,t_variable,variable_lim,tn,level+1,boundary);
        t_variable[level]+=fabs(variable_lim[level][0]-variable_lim[level][1])/tn[level];
    }
}

int main(void) {
	int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
	int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
	int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
	double a, b, x1, x2, y1, y2, hy, sum, delta;
	double t_variable[N_MAX], variable_lim[N_MAX][2];
	int tn[N_MAX];

	scanf("%d", &to_do);
	switch (to_do) {
		case 1: // loop over quadratures and integrands
			scanf("%lf %lf %d", &a, &b, &n);
			for(int q = 0; q < no_quads; ++q) {
				for(int f = 0; f < no_funcs; ++f) {
					printf("%.5f ",quad_select(f, q, a, b, n));
				}
				printf("\n");
			}
			break;
		case 2: // adaptive algorithm
			scanf("%d %d",&integrand_fun_no,&method_no);
			scanf("%lf %lf %lf", &a, &b, &delta);
			printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
			break;
		case 3: // double integral over a rectangle
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
			break;
		case 4: // double integral over normal domain
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf", &hy);
			printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
			break;
		case 5: // double integral over multiple normal domains
			scanf("%lf %lf %d", &x1, &x2, &nx);
			scanf("%lf %lf %d", &y1, &y2, &ny);
			printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
			break;
		case 6: // triple integral over a cuboid
			scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], tn);
			scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], tn+1);
			scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], tn+2);
			scanf("%d", &flag);
			printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, bound3v , flag));
			break;
		case 7: // multiple integral over hyper-cuboid
			scanf("%d", &n);
			if(n > N_MAX) break;
			for(int i = 0; i < n; ++i) {
				scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], tn+i);
			}
			scanf("%d", &flag);
			sum = 0.;
			recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0, flag ? boundNv : NULL);
			for(int i=0; i<n; i++)
                sum*=fabs(variable_lim[i][0]-variable_lim[i][1])/tn[i];
			printf("%.5f\n", sum);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
	return 0;
}

