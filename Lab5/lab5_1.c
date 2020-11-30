#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>

double fp(double x)
{
    return (4*x-6*x+5);  /*funkcja podcalkowa*/
}

double metodaTrapezow(double a, double b, int n)
{
    double h, calka = 0;
    h = (b-a)/n;
    for(int i = 1; i < n; i++)
    {
        calka += fp(a+i*h);
    }
    calka += (fp(a)+fp(b))/2;
    calka *= h;
    printf("calka %f\n", calka);
}

double process()
{
    int a = rand()%10+5;
    int b = rand()%10+10;
    int n = rand()%10;
    if(a<b)
    {
        metodaTrapezow(a, b, n);
    }
    else
    {
        printf("a>b nie mozna policzyc calki\n");
    }
}

int main()
{
    printf("Podaj liczbe procesow: ");
    int p;
    scanf("%d", &p);
    srand(time(NULL));
    pid_t pid = fork();
    for(int i = 1; i <= p; i++)
    {
        if(pid == 0)
        {
            wait(NULL);
        }
        else
        {
            process();
        }
    }
return 0;
}
