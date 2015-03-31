/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Employee;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class EmployeeDirectory {
     private ArrayList<Employee> employeeList;

    public EmployeeDirectory() {
        employeeList = new ArrayList<Employee>();
    }

    public ArrayList<Employee> getEmployeeList() {
        return employeeList;
    }
    
    public Employee createEmployee(String f_employeeName, int age){
        Employee l_employee = new Employee(f_employeeName, age);
        l_employee.setEmployeeName(f_employeeName);
        employeeList.add(l_employee);
        return l_employee;
    }
}
