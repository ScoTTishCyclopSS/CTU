#include <stdio.h>

int main(int argc, char *argv[]){
    int r, n, counter = 0, klad = 0, zap = 0, sud = 0, lich = 0, max = -10000, min = 10000;//max = (-1)*__INT_MAX__, min = __INT_MAX__
    float prumer, sum = 0;
    while ((r = scanf("%d", &n)) == 1){
        counter++;
        if (n > 0){
            klad++;
        }
        if (n < 0){
            zap++;
        }
        if(n % 2 == 0){
            sud++;
        }
        else{
            lich++;
        }
        sum += n;
        prumer = sum/counter;
        
        if(n > max)
        max = n;

        if(n < min)
        min = n;

        if (n <= 10000 && n >= -10000){
            if (counter != 1)
            printf(", ");
            printf("%d", n);
        }
        else{
            printf("\nError: Vstup je mimo interval!\n");
            return 100;
        }
 
    }
    if (r == 0){
        printf("\nError: Vstup neni cislo!\n");
        return 100;
    }
    else{
        printf("\nPocet cisel: %d\n", counter);
        printf("Pocet kladnych: %d\n", klad);
        printf("Pocet zapornych: %d\n", zap);
        printf("Procento kladnych: %.2f\n", (float)(klad/(counter * 0.01)));
        printf("Procento zapornych: %.2f\n", (float)(zap/(counter * 0.01)));
        printf("Pocet sudych: %d\n", sud);
        printf("Pocet lichych: %d\n", lich);
        printf("Procento sudych: %.2f\n", (float)(sud/(counter * 0.01)));
        printf("Procento lichych: %.2f\n", (float)(lich/(counter * 0.01)));
        printf("Prumer: %.2f\n", prumer);
        printf("Maximum: %d\n", max);
        printf("Minimum: %d\n", min);
        return 0;
    }
}
