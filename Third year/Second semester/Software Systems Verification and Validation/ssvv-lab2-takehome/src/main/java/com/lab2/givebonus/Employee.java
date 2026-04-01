package com.lab2.givebonus;

public class Employee {
    private final String name;
    private final String department;
    private final String function;
    private int salary;

    public Employee(String name, String department, String function, int salary) {
        this.name = name;
        this.department = department;
        this.function = function;
        this.salary = salary;
    }

    public String getName() {
        return name;
    }

    public String getDepartment() {
        return department;
    }

    public String getFunction() {
        return function;
    }

    public int getSalary() {
        return salary;
    }

    public void setSalary(int salary) {
        this.salary = salary;
    }
}
