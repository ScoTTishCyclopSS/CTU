package cz.cvut.fel.pjv;

import java.util.Scanner;

public class Lab01 {
   
   public void homework(String[] args) {
     Scanner sc = new Scanner(System.in);
     System.out.println("Vyber operaci (1-soucet, 2-rozdil, 3-soucin, 4-podil):\n");
     char op = sc.next().charAt(0);
     System.out.println("Zadej scitanec: ");
     float a = sc.nextFloat();
     System.out.println("Zadej scitanec: ");
     float b = sc.nextFloat();
     System.out.println("Zadej pocet desetinnych mist: ");
     int dec = sc.nextInt();
     float result = operation(op, a, b);
    // String formattedDouble = String.format("%."+ dec +"f", result);
   }
   
   public float operation(char op, float a, float b){
       float result = 0;
       switch (op){
               case '+':
                   result = a + b;
                   break;
               case '-':
                   result = a - b;
                   break;
               case '*':
                   result = a * b;
                   break;
               case '/':
                   result = a / b;
                   break;
               default:
                   break;
       }
       return result;
   }
}