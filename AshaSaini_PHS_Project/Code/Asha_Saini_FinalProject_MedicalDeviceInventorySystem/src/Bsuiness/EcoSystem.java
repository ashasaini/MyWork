/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Bsuiness;   

import Business.Enterprise.InventoryEnterprise;
import Business.Enterprise.MaintenanceEnterprise;
import Business.Enterprise.ProcurementEnterprise;
import Business.Enterprise.SupplierEnterprise;
import Business.Network.Network;
import Business.Organization.Organization;
import Business.Role.Role;
import Business.Role.SystemAdminRole;
import Business.UserAccount.UserAccount;
import Business.UserAccount.UserAccountDirectory;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class EcoSystem extends Organization{
    private static EcoSystem business;
    private ArrayList<Network> networkList;
    private ArrayList<SupplierEnterprise> supplierEnterpriseList;
    private ProcurementEnterprise procurementEnterprise;
    private InventoryEnterprise inventoryEnterprise;
    private MaintenanceEnterprise maintenanceEnterprise;
    
//    private EnterpriseDirectory enterpriseList;
    //to hold supplier enterprise, inventory enterprise, 
    //procurement enterprise, maintenance enterprise
    
    //private OrganizationDirectory organizationDir;
    //to hold supplier organization, inventory organization,
    //procurement organization, maintanence organization
    private UserAccountDirectory registeredSuppliersList;
        
    public static EcoSystem getEcoSystemInstance() {
        if (business == null) {
            business = new EcoSystem();
        }
        return business;
    }

    private EcoSystem() {
        super("Ecosystem", null);
        networkList = new ArrayList<>();       
    //    enterpriseList = new EnterpriseDirectory();       
        registeredSuppliersList = new UserAccountDirectory();       
        supplierEnterpriseList = new ArrayList<>();       
        procurementEnterprise = new ProcurementEnterprise("Procurement Enterprise");
//      procurementEnterprise.getOrganizationDirectory().getOrganizationList().get(0).setType(Type.ProcurementAdminOrganization);
        inventoryEnterprise = new InventoryEnterprise("Inventory Enterprise");
        maintenanceEnterprise = new MaintenanceEnterprise("Maintenance Enterprise");
    }   

    public ArrayList<Network> getNetworkList() {
        return networkList;
    }

    public Network createAndAddNetwork(String networkName) {
        Network l_network = new Network(networkName);
        networkList.add(l_network);
        return l_network;
    }
    
    public void deleteNetwork(Network f_network){
        networkList.remove(f_network);
    }

//    public EnterpriseDirectory getEnterpriseList() {
//        return enterpriseList;
//    }
//
//    public void setEnterpriseList(EnterpriseDirectory enterpriseList) {
//        this.enterpriseList = enterpriseList;
//    }

    @Override
    public ArrayList<Role> getSupportedRole() {
        ArrayList<Role> l_roleList = new ArrayList<>();
        l_roleList.add(new SystemAdminRole());
        return l_roleList;
    }

    public boolean checkIfUsernameIsUnique(String f_userName) {
        if (!this.getUserAccountDirectory().checkIfUsernameIsUnique(f_userName)) {
            return false;
        }

        for (Network l_network : networkList) {
        }

        return true;
    }    

    public ArrayList<SupplierEnterprise> getSupplierEnterpriseList() {
        return supplierEnterpriseList;
    }

    public void setSupplierEnterpriseList(ArrayList<SupplierEnterprise> supplierEnterpriseList) {
        this.supplierEnterpriseList = supplierEnterpriseList;
    }

    public UserAccountDirectory getRegisteredSuppliersList() {
        return registeredSuppliersList;
    }
    
    public SupplierEnterprise CreateAndAddSupplierEnterprise(String f_name){
        SupplierEnterprise se = new SupplierEnterprise(f_name);
        supplierEnterpriseList.add(se);
        return se;
    }    
    
    public UserAccount serachUserAccountInRegisteredSupplier(String userName, String password){
        UserAccount ua = registeredSuppliersList.authenticateUser(userName, password);
        return ua;
    }

    public ProcurementEnterprise getProcurementEnterprise() {
        return procurementEnterprise;
    }

    public void setProcurementEnterprise(ProcurementEnterprise procurementEnterprise) {
        this.procurementEnterprise = procurementEnterprise;
    }

    public InventoryEnterprise getInventoryEnterprise() {
        return inventoryEnterprise;
    }

    public void setInventoryEnterprise(InventoryEnterprise inventoryEnterprise) {
        this.inventoryEnterprise = inventoryEnterprise;
    }

    public MaintenanceEnterprise getMaintenanceEnterprise() {
        return maintenanceEnterprise;
    }

    public void setMaintenanceEnterprise(MaintenanceEnterprise maintenanceEnterprise) {
        this.maintenanceEnterprise = maintenanceEnterprise;
    }
    
    SupplierEnterprise getSupplierEnterprise(SupplierEnterprise supplierEnterprise){
        //serach in the supplier enterprise list
        for(SupplierEnterprise se: supplierEnterpriseList){
            if(se.equals(supplierEnterprise)){
                return se;
            }
        }
        return null;
    }
}
