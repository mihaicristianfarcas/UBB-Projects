package ssvv.lab2.inclass;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.InputMismatchException;

public class App {
    public static void main(String[] args) {
        File inputFile = new File("in.txt");
        
        try (Scanner scanner = new Scanner(inputFile)) {
            
            if (!scanner.hasNextInt()) {
                System.out.println("Error: The first value in in.txt must be an integer (n).");
                return;
            }
            int noElementsArray = scanner.nextInt();

            if (noElementsArray < 0) {
                System.out.println("Error: Number of elements cannot be negative.");
                return;
            }

            int[] valuesArray = new int[noElementsArray];
            int noOfEvenElements = 0;

            for (int i = 0; i < noElementsArray; i++) {
                if (!scanner.hasNextInt()) {
                    System.out.println("Error: Not enough elements or invalid data type in in.txt.");
                    return;
                }
                
                int currentVal = scanner.nextInt();
                valuesArray[i] = currentVal;

                // Natural numbers are >= 0. Count even ones.
                if (valuesArray[i] >= 0 && valuesArray[i] % 2 == 0) {
                    noOfEvenElements++;
                }
            }

            System.out.println(noOfEvenElements);

        } catch (FileNotFoundException e) {
            System.out.println("Error: in.txt not found.");
        } catch (InputMismatchException e) {
            System.out.println("Error: Invalid data format in in.txt.");
        } catch (Exception e) {
            System.out.println("An unexpected error occurred: " + e.getMessage());
        }
    }
}
