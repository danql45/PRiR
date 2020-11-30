#include<stdio.h>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>

double pow(double x, double y)
{
    double res = 1;
    for(int i = 1; i <= y; i++)
    {
        res *= x;
    }
    return res;
}

float leibniz()
{
    float pi = 0, sum = 0;
    int n = rand()%5000+100;
	for(int j = 1; j <= n; j++)
	{
		sum += pow(-1, j-1) / (2*j - 1);
	}
	pi = 4 * sum;
	printf("%f\n", pi);
}

int main()
{
    int p;
    printf("Podaj liczbe procesow: ");
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
            leibniz();
        }
    }

return 0;
}
