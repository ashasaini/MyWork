/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.CareTeam;

import Business.Employee.Employee;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class TeamMember {

    ArrayList<Employee> teamMemberList;

    public TeamMember() {
        teamMemberList = new ArrayList<>();
    }

    public Employee addEmployee(String name, int age) {
        Employee p = new Employee(name,age);
        p.setEmployeeName(name);
        teamMemberList.add(p);
        return p;
    }

    public Employee searchEmployeebyId(int id) {
        for (Employee p : teamMemberList) {
            if (id == p.getEmployeeId()) {
                return p;
            }
        }
        return null;
    }

    public ArrayList<Employee> searchEmployeebyName(String name) {
        ArrayList<Employee> serachResults = new ArrayList<>();
        for (Employee p : teamMemberList) {
            if (p.getEmployeeName().contains(name)) {
                serachResults.add(p);
            }
        }
        return null;
    }

    public void deleteEmployee(Employee p) {
        teamMemberList.remove(p);
    }
}
