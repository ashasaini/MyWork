/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.UserAccount;

import Business.Employee.Employee;
import Business.Role.Role;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class UserAccountDirectory {
     private ArrayList<UserAccount> userAccountList;

    public UserAccountDirectory() {
        userAccountList = new ArrayList<UserAccount>();
    }

    public ArrayList<UserAccount> getUserAccountList() {
        return userAccountList;
    }
    
    public UserAccount authenticateUser(String f_userName, String f_password){
        for (UserAccount l_ua : userAccountList)
            if ((l_ua.getUserName().equals(f_userName)) && (l_ua.getPassword().equals(f_password))){
                return l_ua;
            }
        return null;
    }
    
    public UserAccount createUserAccount(String f_userName,
                                        String f_password,
                                        Employee f_employee, 
                                        Role f_role,
                                        String status,
                                        String securityQuestion,
                                        String securityAnswer){
        UserAccount l_userAccount = new UserAccount();
        l_userAccount.setUserName(f_userName);
        l_userAccount.setPassword(f_password);
        l_userAccount.setEmployee(f_employee);
        l_userAccount.setRole(f_role);
        l_userAccount.setStatus(status);
        l_userAccount.setSecurityQuestion(securityQuestion);
        l_userAccount.setSecurityQuestion(securityAnswer);
        l_userAccount.setIsApproved(false);
        userAccountList.add(l_userAccount);
        
        return l_userAccount;
    }
    
    public boolean checkIfUsernameIsUnique(String f_userName){
        for (UserAccount ua : userAccountList){
            if (ua.getUserName().equals(f_userName))
                return false;
        }
        return true;
    }
}
