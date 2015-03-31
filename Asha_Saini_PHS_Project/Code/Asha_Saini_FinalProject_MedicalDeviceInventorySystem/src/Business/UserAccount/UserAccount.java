/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.UserAccount;

import Business.Employee.Employee;
import Business.Role.Role;
import Business.WorkQueue.WorkQueue;

/**
 *
 * @author Asha
 */
public class UserAccount {
    private String userName;
    private String password;
    private Employee employee;
    private Role role;
    private WorkQueue workQueue;
    private String securityQuestion;
    private String securityAnswer;
    private String status;
    private boolean isApproved;

    public UserAccount() {
        workQueue = new WorkQueue();
    }
    
    public String getUserName() {
        return userName;
    }

    public void setUserName(String f_userName) {
        this.userName = f_userName;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String f_password) {
        this.password = f_password;
    }

    public Role getRole() {
        return role;
    }
    
    public void setEmployee(Employee f_employee) {
        this.employee = f_employee;
    }

    public void setRole(Role f_role) {
        this.role = f_role;
    }

    public Employee getEmployee() {
        return employee;
    }

    public WorkQueue getWorkQueue() {
        return workQueue;
    }

    public String getSecurityQuestion() {
        return securityQuestion;
    }

    public void setSecurityQuestion(String securityQuestion) {
        this.securityQuestion = securityQuestion;
    }

    public String getSecurityAnswer() {
        return securityAnswer;
    }

    public void setSecurityAnswer(String securityAnswer) {
        this.securityAnswer = securityAnswer;
    }

    public boolean isIsApproved() {
        return isApproved;
    }

    public void setIsApproved(boolean isApproved) {
        this.isApproved = isApproved;
    }

    public String getStatus() {
        return status;
    }

    public void setStatus(String status) {
        this.status = status;
    }    
    
    @Override
    public String toString() {
        return userName;
    }
}
