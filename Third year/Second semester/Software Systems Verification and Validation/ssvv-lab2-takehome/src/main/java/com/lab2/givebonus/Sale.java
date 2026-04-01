package com.lab2.givebonus;

public class Sale {
    private final String department;
    private final int sumSale;

    public Sale(String department, int sumSale) {
        this.department = department;
        this.sumSale = sumSale;
    }

    public String getDepartment() {
        return department;
    }

    public int getSumSale() {
        return sumSale;
    }
}
