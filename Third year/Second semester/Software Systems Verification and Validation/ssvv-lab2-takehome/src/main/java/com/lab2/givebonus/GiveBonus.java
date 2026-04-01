package com.lab2.givebonus;

import java.util.List;

public class GiveBonus {

    public static int giveBonus(int numberEmployees, List<Employee> listEmployees,
                                int numberSales, List<Sale> listSales) {
        if (numberEmployees < 0 || numberSales < 0) {
            throw new IllegalArgumentException("numberEmployees and numberSales must be >= 0");
        }
        if (listEmployees == null || listSales == null) {
            throw new IllegalArgumentException("Input lists must not be null");
        }
        if (numberEmployees == 0 || numberSales == 0) {
            return 1;
        }
        if (listEmployees.size() != numberEmployees || listSales.size() != numberSales) {
            throw new IllegalArgumentException("Count does not match list size");
        }

        for (Employee employee : listEmployees) {
            if (employee.getSalary() < 0) {
                throw new IllegalArgumentException("Employee salary must be >= 0");
            }
        }

        String topDepartment = null;
        int maxSale = Integer.MIN_VALUE;
        for (Sale sale : listSales) {
            if (sale.getSumSale() > maxSale) {
                maxSale = sale.getSumSale();
                topDepartment = sale.getDepartment();
            }
        }

        int eligibleCount = 0;
        for (Employee employee : listEmployees) {
            if (employee.getDepartment().equals(topDepartment)) {
                eligibleCount++;
            }
        }
        if (eligibleCount == 0) {
            return 2;
        }

        for (Employee employee : listEmployees) {
            if (!employee.getDepartment().equals(topDepartment)) {
                continue;
            }
            boolean isManager = employee.getFunction() != null
                && employee.getFunction().equalsIgnoreCase("manager");
            if (employee.getSalary() > 5000 || isManager) {
                employee.setSalary(employee.getSalary() + 500);
            } else {
                employee.setSalary(employee.getSalary() + 1000);
            }
        }

        return 0;
    }
}
