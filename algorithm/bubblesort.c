/* bubblesorter.c -- 버블 정렬을 사용하여 수를 소팅한다 */
#include <stdio.h>
#include <stdlib.h>

#define NUM 40

void fillarray(double ar[], int n);
void showarray(const double ar[], int n);
void bubblesort(double ar[], int n);

int main(void)
{
    double vals[NUM];
    fillarray(vals, NUM);
    puts("random list : ");
    showarray(vals, NUM);
    bubblesort(vals, NUM);
    puts("\nsorted list : ");
    showarray(vals, NUM);
    return 0;
}

void fillarray(double ar[], int n)
{
    for (int index = 0; index < n; index++)
    {
        ar[index] = (double)rand() / ((double)rand() + 0.1);
    }
}

void showarray(const double ar[], int n)
{
    for (int index = 0; index < n; index++)
    {
        printf("%9.4f ", ar[index]);
        if (index % 6 == 5)
            putchar('\n');
    }
    if (n % 6 != 0)
        putchar('\n');
}

void bubblesort(double ar[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (ar[j] > ar[j + 1])
            {
                double temp = ar[j];
                ar[j] = ar[j + 1];
                ar[j + 1] = temp;
            }
        }
    }
}
