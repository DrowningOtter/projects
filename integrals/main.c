#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef double(*func)(double x);

int iterations;//переменная для подсчета количества итераций, потребовавшихся для поиска корня

double rounding(double x, double eps) {
	int sign = x >= 0 ? 1 : -1;
	int exp = 1;
	while (eps < 1) {
		eps *= 10;
		exp *= 10;
	}
	return round(x * sign * exp) / exp;
}

extern double f1(double x);
extern double f2(double x);
extern double f3(double x);
extern double df1(double x);
extern double df2(double x);
extern double df3(double x);


double root_case_1(func f1, func f2, func df1, func df2, double a, double b, double eps) {
	double c_left = a, c_right = b;
	double save_a = a;
	double save_b = b;
	iterations = 0;

	double F_root(double x) {
		return f1(x) - f2(x);
	}
	double DF_root(double x) {
		return df1(x) - df2(x);
	}

	do {
		c_left = (c_left * F_root(c_right) - c_right * F_root(c_left)) / (F_root(c_right) - F_root(c_left));
		c_right = c_right - F_root(c_right) / DF_root(c_right);
		iterations++;
	} while(c_right - c_left >= eps);

	return (c_left + c_right) / 2;
}

double root_case_2(func f1, func f2, func df1, func df2, double a, double b, double eps) {
	double c_left = a, c_right = b;
	double save_a = a;
	double save_b = b;
	iterations = 0;

	double F_root(double x) {
		return f1(x) - f2(x);
	}
	double DF_root(double x) {
		return df1(x) - df2(x);
	}

	do {
		c_right = (c_left * F_root(c_right) - c_right * F_root(c_left)) / (F_root(c_right) - F_root(c_left));
		c_left = c_left - F_root(c_left) / DF_root(c_left);
		iterations++;
	} while(c_right - c_left >= eps);

	return (c_left + c_right) / 2;
}

double root(func f1, func f2, func df1, func df2, double a, double b, double eps) {
	double F_root(double x) {
		return f1(x) - f2(x);
	}

	if (a > b){
		double temp = a;
		a = b;
		b = a;
	}

	double res;

	double cond1 = F_root(a) - 0;
	double cond2 = F_root((a + b) / 2) - (F_root(a) + F_root(b)) / 2;
	if (cond1 > 0) {//F убывает
		if (cond2 > 0) {//график расположен выше хорды
			res = root_case_1(f1, f2, df1, df2, a, b, eps);
		} else if (cond2 < 0) {
			res = root_case_2(f1, f2, df1, df2, a, b, eps);
		} else {
			printf("DEBUG: cond1 > 0, cond2 = 0\n");
		}
	} else if (cond1 < 0){//F возрастает
		if (cond2 > 0) {
			res = root_case_2(f1, f2, df1, df2, a, b, eps);
		} else if (cond2 < 0) {
			res = root_case_1(f1, f2, df1, df2, a, b, eps);
		} else {
		}
	} else {
	}

	if (res > b || res < a) {
		printf("ROOT: there is no root on this interval!!!\n");
		return -1;
	}

	return rounding(res, eps);
}

double integral_n(func F, int n, double a, double b) {
	double h = (b - a) / n;

	double sum_f = 0;

	for (int i = 0; i < n; ++i)
	{
		sum_f += F(a + (i + 0.5) * h);
	}

	sum_f *= h;

	return sum_f;
}

double integral(func f, double a, double b, double eps) {
	int n0 = 20;

	double In = integral_n(f, 2 * n0, a, b), I2n = integral_n(f, 4 * n0, a, b);
	n0 *= 4;
	while (fabs(In - I2n) / 3 >= eps) {
		In = I2n;
		n0 *= 2;
		I2n = integral_n(f, n0, a, b);
	}

	return I2n;
}

void test_root(func f1, func f2, func f3, double eps) {
	double root_f1_f3 = 2.191743;
	double root_f1_f2 = 6.096169;
	double root_f2_f3 = 4.224745;

	printf("testing root func:\n\n");

	printf("Testing intersection between f1 and f2...\n");
	if (fabs(root(f1, f2, df1, df2, 5, 7, eps) - root_f1_f2) <= eps) {
		printf("intersection between f1 and f2 calculated correctly!\n");
	} else {
		printf("calculated intersection between f1 and f2 has bad accuracy...\n");;
	}
	printf("\n");

	printf("Testing intersection between f1 and f3...\n");
	if (fabs(root(f1, f3, df1, df3, 2.0001, 3, eps) - root_f1_f3) <= eps) {
		printf("intersection between f1 and f3 calculated correctly!\n");
	} else {
		printf("calculated intersection between f1 and f3 has bad accuracy...\n");;
	}
	printf("\n");

	printf("Testing intersection between f2 and f3...\n");
	if (fabs(root(f2, f3, df2, df3, 4, 5, eps) - root_f2_f3) <= eps) {
		printf("intersection between f2 and f3 calculated correctly!\n");
	} else {
		printf("calculated intersection between f2 and f3 has bad accuracy...\n");;
	}
	printf("\n");

}

void test_integral(func f1, func f2, func f3, double eps) {
	printf("testing integral func:\n\n");
	double temp;

	double bounds1[] = {0.015, 2, 5.7, 14.2, 3.6, 9.6};
	double ans1[] = { -0.5357100627119105, 19.25537870, 11.10156390};
	for (int i = 0; i < 3; i++)
	{
		double cur = integral(f1, bounds1[2*i], bounds1[2*i + 1], eps / 2);
		printf("testing f1 integral on interval [%lf, %lf], res= %lf", bounds1[2*i], bounds1[2*i + 1], cur);
		fabs(cur - ans1[i]) <= eps ? printf("(OK)\n"): printf("(Bad Accuracy)\n");
	}
	printf("\n");

	double bounds2[] = {-14.6542, -5.43574, -3.5654, 2.44645, 17.4634, 25.46453};
	double ans2[] = {314.2567482924, 90.8928595575, -231.4561285609};
	for (int i = 0; i < 3; i++)
	{
		double cur = integral(f2, bounds2[2*i], bounds2[2*i + 1], eps / 2);
		printf("testing f2 integral on interval [%lf, %lf], res= %lf", bounds2[2*i], bounds2[2*i + 1], cur);
		fabs(cur - ans2[i]) <= eps ? printf("(OK)\n"): printf("(Bad Accuracy)\n");
	}
	printf("\n");

	double bounds3[] = {-14.543634, -1.43532, -2.34265, 1.45326, 5.325426, 14.432643};
	double ans3[] = {80.22177529526116, 24.84772667033824, 53.32457427254986};
	for (int i = 0; i < 3; i++)
	{
		double cur = integral(f3, bounds3[2*i], bounds3[2*i + 1], eps / 2);
		printf("testing f3 integral on interval [%lf, %lf], res= %lf", bounds3[2*i], bounds3[2*i + 1], cur);
		fabs(cur - ans3[i]) <= eps ? printf("(OK)\n"): printf("(Bad Accuracy)\n");
	}
	printf("\n");
}

int main(int argc, char *argv[]) {
	double eps1 = 0.0001;
	double eps2 = 0.0001;
	double eps = 0.001;
	double temp;

	double a = root(f1, f3, df1, df3, 2.0000001, 3, eps1);
	int iters1 = iterations;

	double b = root(f2, f3, df2, df3, 4, 5, eps1);
	int iters2 = iterations;

	double c = root(f1, f2, df1, df2, 5.5, 6.5, eps1);
	int iters3 = iterations;

	double ans1 = integral(f2, b, c, eps2);
	double ans2 = integral(f1, b, c, eps2);
	double ans3 = integral(f3, a, b, eps2);
	double ans4 = integral(f1, a, b, eps2);

	int long_opt_res = 0;
	const char* short_options = "pnhir";

	int options_len = 6;//dont forget to increase this when adding some new options!!!
	const char* options_names[] = {
		"print-intersections",
		"number-of-iterations",
		"test-root",
		"test-integral",
		"test-root-manual",
		"test-integral-manual",
		""
		"help",
	};

	const char* options_description[] = {
		"Prints the abscissas of the intersection points of curves",
		"Prints the number of iterations required to approximate the solution of equations when searching intersection points",
		"test finding root function",
		"test calculating integral function",
		"test finding root function with specified parameters",
		"test calculating integral function with specified parameters"
	};

	const struct option long_options[] = {
		{ options_names[0], no_argument, NULL, 'p' },//"print-intersections"
		{ options_names[1], no_argument, NULL, 'n' },//"number-of-iterations"
		{ options_names[2], no_argument, NULL, 'r' },//"test-root"
		{ options_names[3], no_argument, NULL, 'i' },//"test-integral"
		{ options_names[4], no_argument, NULL, 292 },//"test-root-manual"
		{ options_names[5], no_argument, NULL, 293 },//"test-integral-manual"
		{ options_names[7], no_argument, NULL, 'h' },//"help"
		{ NULL, 0, NULL, 0 }
	};

	int option_index = -1;	
	int rez;
	while ((rez = getopt_long(argc, argv, short_options, long_options, &option_index)) != -1) {
		switch(rez) {
			case 'h': {
				int i = 0;
				printf("HELP:\n");
				for (int i = 0; i < options_len; ++i)
				{
					if(long_options[i].val >= 0 && long_options[i].val <= 255) {
						printf("--%s, -%c: %s\n", long_options[i].name, long_options[i].val, options_description[i]);
					} else {
						printf("--%s: %s\n", long_options[i].name, options_description[i]);
					}
				}
				break;
			};
			case 'n': {
				printf("Number of iterations for calculating roots: %d, %d, %d\n", iters1, iters2, iters3);
				break;
			};
			case 'p': {
				printf("Abscissas of intersection points: %lf; %lf; %lf\n", a, b, c);
				printf("Ordinates of intersection points: %lf, %lf, %lf\n", f1(a), f2(b), f3(c));
				break;
			};
			case 'r': {
				test_root(f1, f2, f3, eps1);
				break;
			};
			case 'i': {
				test_integral(f1, f2, f3, eps);
				break;
			};
			case 292: {
				// printf("test-root-manual called!\n");
				int func1, func2;
				double left, right;
				func func_arr[] = {f1, f2, f3};
				func dfunc_arr[] = {df1, df2, df3};
				printf("enter number of func1, func2, left and right bounds to find the root\n");
				scanf("%d %d %lf %lf", &func1, &func2, &left, &right);
				func1--;
				func2--;
				double res = root(func_arr[func1], func_arr[func2], dfunc_arr[func1], dfunc_arr[func2], left, right, eps1);
				printf("%lf\n", res);

				break;
			};
			case 293: {
				printf("enter number of func, left and right bounds\n");
				int func_number;
				double left, right;
				scanf("%d %lf %lf", &func_number, &left, &right);
				func_number--;
				func funcs[] = {f1, f2 ,f3};

				double res = integral(funcs[func_number], left, right, eps);
				printf("integral on your parameters: %lf\n", res);

				break;
			};

			case '?': default: {
				printf("unknown option...\n");
				exit(1);
				break;
			};
		};
		
		option_index = -1;
	}

	printf("%lf\n", ans1 - ans2 + ans3 - ans4);

	return 0;
}