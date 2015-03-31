/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Organization;

import Business.Employee.EmployeeDirectory;
import Business.Role.Role;
import Business.UserAccount.UserAccountDirectory;
import Business.WorkQueue.WorkQueue;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public abstract class Organization {

    private String organizationName;
    private WorkQueue workQueue;
    private EmployeeDirectory employeeDirectory;
    private UserAccountDirectory userAccountDirectory;
    private int organizationID;
    private static int counter;
    Type type;

    public enum Type {

        Admin("Admin Organization"),
        SupplierAdminOrganization("Supplier Admin Organization"),
        SupplierShippingOrganization("Supplier Shipping Organization"),
        ProcurementAdminOrganization("Procurement Admin Organization"),
        InventoryAdminOrganization("Inventory Admin Organization"),
        MaintenanceAdminOrganization("Maintenance Admin Organization"),
        HospitalAdminOrganization("Hospital Admin Organization"),
        HospitalDoctorOrganization("Hospital Doctor Organization"),
        HospitalStaffOrganization("Hospital Staff Organization");

        private String value;

        private Type(String value) {
            this.value = value;
        }

        public String getValue() {
            return value;
        }
    }

    public Organization(String f_organizationName, Organization.Type f_type) {
        this.organizationName = f_organizationName;
        this.type = f_type;
        workQueue = new WorkQueue();
        employeeDirectory = new EmployeeDirectory();
        userAccountDirectory = new UserAccountDirectory();
        organizationID = counter;
        counter++;
    }

    public abstract ArrayList<Role> getSupportedRole();

    public UserAccountDirectory getUserAccountDirectory() {
        return userAccountDirectory;
    }

    public int getOrganizationID() {
        return organizationID;
    }

    public EmployeeDirectory getEmployeeDirectory() {
        return employeeDirectory;
    }

    public String getOrganizationName() {
        return organizationName;
    }

    public WorkQueue getWorkQueue() {
        return workQueue;
    }

    public void setName(String f_organizationName) {
        this.organizationName = f_organizationName;
    }

    public void setWorkQueue(WorkQueue f_workQueue) {
        this.workQueue = f_workQueue;
    }

    public Type getType() {
        return type;
    }

    public void setType(Type type) {
        this.type = type;
    }

    @Override
    public String toString() {
        return organizationName;
    }
}
