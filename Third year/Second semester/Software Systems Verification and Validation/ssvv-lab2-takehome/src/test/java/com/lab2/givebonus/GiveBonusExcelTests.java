package com.lab2.givebonus;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.ArrayList;
import java.util.List;
import org.junit.jupiter.api.Test;

class GiveBonusExcelTests {

    // Req_EC_TC
    @Test
    void ecTc1_ne0_returns1() {
        int code = GiveBonus.giveBonus(0, List.of(), 1, List.of(new Sale("IT", 1000)));
        assertEquals(1, code);
    }

    @Test
    void ecTc2_ns0_returns1() {
        int code = GiveBonus.giveBonus(1,
            List.of(new Employee("E1", "IT", "Dev", 3000)),
            0,
            List.of());
        assertEquals(1, code);
    }

    @Test
    void ecTc3_etd0_returns2() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 3000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("HR", 5000)));
        assertEquals(2, code);
    }

    @Test
    void ecTc4_salaryLe5000_notManager_plus1000() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 4000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(5000, employees.get(0).getSalary());
    }

    @Test
    void ecTc5_salaryGt5000_notManager_plus500() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 6000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(6500, employees.get(0).getSalary());
    }

    @Test
    void ecTc6_salaryLe5000_manager_plus500() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Manager", 4000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(4500, employees.get(0).getSalary());
    }

    @Test
    void ecTc7_neNegative_errorException() {
        assertThrows(IllegalArgumentException.class, () ->
            GiveBonus.giveBonus(-1,
                mutableEmployees(new Employee("E1", "IT", "Dev", 3000)),
                1,
                List.of(new Sale("IT", 5000))));
    }

    // Req_BVA_TC
    @Test
    void bvaTc1_neMinus1_error() {
        assertThrows(IllegalArgumentException.class, () ->
            GiveBonus.giveBonus(-1,
                mutableEmployees(new Employee("E1", "IT", "Dev", 3000)),
                1,
                List.of(new Sale("IT", 5000))));
    }

    @Test
    void bvaTc2_ne0_code1() {
        int code = GiveBonus.giveBonus(0, List.of(), 1, List.of(new Sale("IT", 5000)));
        assertEquals(1, code);
    }

    @Test
    void bvaTc3_ne1_code0_salary4000() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 3000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(4000, employees.get(0).getSalary());
    }

    @Test
    void bvaTc4_nsMinus1_error() {
        assertThrows(IllegalArgumentException.class, () ->
            GiveBonus.giveBonus(1,
                mutableEmployees(new Employee("E1", "IT", "Dev", 3000)),
                -1,
                List.of(new Sale("IT", 5000))));
    }

    @Test
    void bvaTc5_ns0_code1() {
        int code = GiveBonus.giveBonus(1,
            mutableEmployees(new Employee("E1", "IT", "Dev", 3000)),
            0,
            List.of());
        assertEquals(1, code);
    }

    @Test
    void bvaTc6_ns1_code0_salary4000() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 3000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(4000, employees.get(0).getSalary());
    }

    @Test
    void bvaTc7_salaryMinus1_error() {
        assertThrows(IllegalArgumentException.class, () ->
            GiveBonus.giveBonus(1,
                mutableEmployees(new Employee("E1", "IT", "Dev", -1)),
                1,
                List.of(new Sale("IT", 5000))));
    }

    @Test
    void bvaTc8_salary0_plus1000() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 0));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(1000, employees.get(0).getSalary());
    }

    @Test
    void bvaTc9_salary4999_plus1000() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 4999));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(5999, employees.get(0).getSalary());
    }

    @Test
    void bvaTc10_salary5000_plus1000() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 5000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(6000, employees.get(0).getSalary());
    }

    @Test
    void bvaTc11_salary5001_plus500() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 5001));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(5501, employees.get(0).getSalary());
    }

    // Req_EC_BVA_all_TC
    @Test
    void finalTc1_ec1_bva2() {
        int code = GiveBonus.giveBonus(0, List.of(), 1, List.of(new Sale("IT", 5000)));
        assertEquals(1, code);
    }

    @Test
    void finalTc2_ec2_bva5() {
        int code = GiveBonus.giveBonus(1,
            mutableEmployees(new Employee("E1", "IT", "Dev", 3000)),
            0,
            List.of());
        assertEquals(1, code);
    }

    @Test
    void finalTc3_ec3_code2() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 3000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("HR", 5000)));
        assertEquals(2, code);
    }

    @Test
    void finalTc4_ec4_bva9() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 4999));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(5999, employees.get(0).getSalary());
    }

    @Test
    void finalTc5_ec4_bva10() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 5000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(6000, employees.get(0).getSalary());
    }

    @Test
    void finalTc6_ec5_bva11() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Dev", 5001));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(5501, employees.get(0).getSalary());
    }

    @Test
    void finalTc7_ec6_managerPlus500() {
        List<Employee> employees = mutableEmployees(new Employee("E1", "IT", "Manager", 4000));
        int code = GiveBonus.giveBonus(1, employees, 1, List.of(new Sale("IT", 5000)));
        assertEquals(0, code);
        assertEquals(4500, employees.get(0).getSalary());
    }

    @Test
    void finalTc8_ec1inv_bva1_error() {
        assertThrows(IllegalArgumentException.class, () ->
            GiveBonus.giveBonus(-1,
                mutableEmployees(new Employee("E1", "IT", "Dev", 3000)),
                1,
                List.of(new Sale("IT", 5000))));
    }

    @Test
    void finalTc9_bva7_error() {
        assertThrows(IllegalArgumentException.class, () ->
            GiveBonus.giveBonus(1,
                mutableEmployees(new Employee("E1", "IT", "Dev", -1)),
                1,
                List.of(new Sale("IT", 5000))));
    }

    private static List<Employee> mutableEmployees(Employee employee) {
        List<Employee> employees = new ArrayList<>();
        employees.add(employee);
        return employees;
    }
}
