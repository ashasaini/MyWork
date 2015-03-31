/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Employee;

/**
 *
 * @author Asha
 */
public class Employee {
    private String employeeName;
    private int age;
    private int employeeId;
    private static int count = 1;

    public Employee(String name, int age) {
        this.employeeName = name;
        this. age = age;
        this.employeeId = count;
        this.count++;
    }

    public int getEmployeeId() {
        return employeeId;
    }

    public void setEmployeeName(String f_EmployeeName) {
        this.employeeName = f_EmployeeName;
    }
    
    public String getEmployeeName() {
        return employeeName;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }
    
    @Override
    public String toString() {
        return employeeName;
    }    
}
