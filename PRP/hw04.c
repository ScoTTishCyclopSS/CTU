#include <stdio.h>
#include <string.h>
#include <stdlib.h>
  
/* Function declaration */
void erat_sieve(long long int);
  
/* The main structure.*/
int main(int argc, char *argv[])
{
    long long int num;
    int read;
  
    while ((read = scanf("%lld", &num)) == 1) {
        if (num > 0) {
            printf("Prvociselny rozklad cisla %lld je:\n", num);
            if (num == 1) {
                printf("1\n");
            } else {
                erat_sieve(num);  
            } 
        } else if (num == 0) {
            return 0;
        } else {
            fprintf(stderr, "Error: Chybny vstup!\n");
            return 100;
        }
    }
    if (read == 0) {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }
}
  
  
/* Sieve of Eratosthenes. */
void erat_sieve(long long int num)
{
    /* base_arr - array of chars with size of 10^6 */
    int k = 1000000;
    char base_arr[k];
    /* Filling the array with 1 for each element. */
    for (int i = 1; i <= k; i++)
        base_arr[i] = 1;
    /* 
     * Working sieve of Eratosthenes. 
     * Logic: https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
     * (basically - we re-write numbers, with complex (non-prime) indexes, with 0)
     */
    for (int i = 2; i * i <= k; i++) {
        if (base_arr[i]) {
            for (int j = i * i; j <= k; j += i){
                base_arr[j] = 0;
            }      
        }
    }
    /* 
     * Function for the primordial decomposition of a number.
     * Logic: https://bit.ly/2GgqFrF
     */
    for (int i = 2; i <= k; i++){
        if (base_arr[i] == 1) {
            int pwr = 0;
            while (num % i == 0) {
                num /= i;
                pwr++;
            }
            if (pwr > 0) {
                printf("%d", i);
                if (pwr > 1)
                    printf("^%d", pwr);
                if (num > 1) 
                    printf(" x ");
            }
        } else {
            continue;
        }
         
    }
    printf("\n");
    memset(base_arr, 0, sizeof base_arr);
}
