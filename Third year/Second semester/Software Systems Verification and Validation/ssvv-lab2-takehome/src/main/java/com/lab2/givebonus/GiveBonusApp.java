package com.lab2.givebonus;

import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class GiveBonusApp {

    public static void main(String[] args) {
        Path inputPath = Path.of("IN.TXT");
        Path outputPath = Path.of("OUT.TXT");

        try {
            InputData inputData = readInput(inputPath);
            int code = GiveBonus.giveBonus(inputData.numberEmployees(), inputData.employees(),
                inputData.numberSales(), inputData.sales());
            writeOutput(outputPath, code, inputData.employees());
        } catch (Exception ex) {
            try {
                Files.writeString(outputPath, "Error", StandardCharsets.UTF_8);
            } catch (IOException ignored) {
            }
        }
    }

    private static InputData readInput(Path inputPath) throws IOException {
        List<Employee> employees = new ArrayList<>();
        List<Sale> sales = new ArrayList<>();

        try (Scanner scanner = new Scanner(Files.newBufferedReader(inputPath, StandardCharsets.UTF_8))) {
            int numberEmployees = scanner.nextInt();
            for (int i = 0; i < numberEmployees; i++) {
                String name = scanner.next();
                String department = scanner.next();
                String function = scanner.next();
                int salary = scanner.nextInt();
                employees.add(new Employee(name, department, function, salary));
            }

            int numberSales = scanner.nextInt();
            for (int i = 0; i < numberSales; i++) {
                String department = scanner.next();
                int sumSale = scanner.nextInt();
                sales.add(new Sale(department, sumSale));
            }

            return new InputData(numberEmployees, employees, numberSales, sales);
        }
    }

    private static void writeOutput(Path outputPath, int code, List<Employee> employees) throws IOException {
        try (BufferedWriter writer = Files.newBufferedWriter(outputPath, StandardCharsets.UTF_8)) {
            writer.write(String.valueOf(code));
            writer.newLine();
            for (Employee employee : employees) {
                writer.write(employee.getName() + " " + employee.getDepartment() + " "
                    + employee.getFunction() + " " + employee.getSalary());
                writer.newLine();
            }
        }
    }

    private record InputData(int numberEmployees, List<Employee> employees,
                             int numberSales, List<Sale> sales) {
    }
}
