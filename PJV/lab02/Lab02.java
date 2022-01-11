/*
 * File name: Lab02.java
 * Date:      2020/03/17 22:36
 * Author:    @timusfed
 */

package cz.cvut.fel.pjv;

import java.util.Arrays;
import java.util.stream.DoubleStream;

public class Lab02 {

   public void homework(String[] args) {

      TextIO tio = new TextIO();
      //tio.TextIO_file("C:\\Users\\fedti\\Desktop\\SUBJECTS\\PJV\\pjv-lab02\\pjv-lab02\\src\\main\\java\\cz\\cvut\\fel\\pjv\\test.txt");
      String str;
      int numCount = 0;
      int lineCount = 0;
      double[] numbers = new double[10];

      while (true) {
         str = tio.getLine();
         if (str == "") {
            System.err.println("End of input detected!");
            if (numCount > 1) {
               System.out.printf("%2d %.3f %.3f\n", numCount, average(numbers, numCount), stDiviation(numbers, numCount));
            }
            break;
         } else {
            lineCount++;
            if (TextIO.isDouble(str) || TextIO.isFloat(str) || TextIO.isInteger(str)) {
               numbers[numCount++] = Double.parseDouble(str);
               if (numCount == 10) {
                  System.out.printf("%2d %.3f %.3f\n", numCount, average(numbers, numCount), stDiviation(numbers, numCount));
                  numCount = 0;
                  numbers = new double[10];
               }
            } else {
               System.err.println("A number has not been parsed from line " + lineCount);
            }
         }
      }
   }

   private double average(double[] numbers, int amount) {
      double sum = DoubleStream.of(numbers).sum();
      return sum / amount;
   }

   private double stDiviation(double[] numbers, int amount) {
      double aver = average(numbers, amount); //find average
      for (int i = 0; i < amount; i++)
         numbers[i] -= aver; //for all of the numbers -= average
      for (int i = 0; i < amount; i++)
         numbers[i] = Math.pow(numbers[i], 2); //for all of the numbers^2
      double sum = DoubleStream.of(numbers).sum(); //sum all the numbers
      return Math.sqrt(sum / amount); //find the sqrt of (divide to [amount of the numbers - 1])
   }
}

