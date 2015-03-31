/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Bsuiness;

import Business.Employee.Employee;
import Business.Enterprise.Enterprise;
import Business.Enterprise.EnterpriseDirectory;
import Business.Enterprise.SupplierEnterprise;
import Business.Network.Network;
import Business.Organization.InventoryAdminOrganization;
import Business.Organization.MaintenanceAdminOrganization;
import Business.Organization.Organization;
import Business.Organization.Organization.Type;
import Business.Organization.OrganizationDirectory;
import Business.Organization.ProcurementAdminOrganization;
import Business.Organization.SupplierAdminOrganization;
import Business.Organization.SupplierShippingOrganization;
import Business.Role.HospitalAdminRole;
import Business.Role.HospitalDoctorRole;
import Business.Role.HospitalStaffRole;
import Business.Role.InventoryAdminRole;
import Business.Role.MaintenanceAdminRole;
import Business.Role.ProcurementAdminRole;
import Business.Role.Role;
import Business.Role.SupplierAdminRole;
import Business.Role.SupplierEnterpriseAdminRole;
import Business.Role.SupplierShippingRole;
import Business.Role.SystemAdminRole;
import Business.UserAccount.UserAccount;
import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class ConfigureASystem {

    public static EcoSystem configure() {
        EcoSystem l_system = EcoSystem.getEcoSystemInstance();
        if (null == l_system) {
            //error
        }

        //create user account for system admin
        Employee l_employee = l_system.getEmployeeDirectory().createEmployee("Asha Saini", 24);

        UserAccount l_ua = l_system.getUserAccountDirectory().
                createUserAccount("sysadmin",
                        "sysadmin",
                        l_employee,
                        new SystemAdminRole(),
                        "Active",
                        "sysadmin",
                        "sysadmin");

        InitializeSystem();
        return l_system;
    }

    private static void InitializeSystem() {
        //initialize networklist
        InitializeNetworkList();
        InitializeEnterprizeList();
        InitializeRegisteredSupplierList();
        InitializeSupplierEnterpriseList();

        //initialize procurement enterprise
        InitializeProcurementEnterprise();

        //initialize inventory enterprise
        InitializeInventoryEnterprise();

        //initialize maintenance enterprise
        InitializeMaintenanceEnterprise();
    }

    private static void InitializeNetworkList() {
        EcoSystem system = EcoSystem.getEcoSystemInstance();
        if (null == system) {
            //error pending
            return;
        }

        //adding netwroks
        String netwrokName = "";
        for (int i = 0; i < 4; i++) {
            switch (i + 1) {
                case 1:
                    netwrokName = "Masachusetts";
                    break;

                case 2:
                    netwrokName = "California";
                    break;

                case 3:
                    netwrokName = "New York";
                    break;

                case 4:
                    netwrokName = "New Jersey";
                    break;
            }
            system.createAndAddNetwork(netwrokName);
        }

        //adding enterprise (hospitals) to each netwrok
        for (Network netwrok : system.getNetworkList()) {
            EnterpriseDirectory enterpriseDirectory = netwrok.getEnterpriseDirectory();

            for (int i = 0; i < 14; i++) {
                String enterpriseName = "";

                String userNameHosAdminOrg = "";
                String passwordHosAdminOrg = "";
                Employee empHosAdminOrg = null;
                Role roleHosAdminOrg = null;
                String securityHosQuestionAdminOrg = "";
                String securityHosAnswerAdminOrg = "";

                String userNameDocOrg = "";
                String passwordDocOrg = "";
                Employee empDocOrg = null;
                Role roleDocOrg = null;
                String securityQuestionDcoOrg = "";
                String securityAnswerDocOrg = "";

                String userNameStaffOrg = "";
                String passwordStaffOrg = "";
                Employee empStaffOrg = null;
                Role roleStaffOrg = null;
                String securityQuestionStaffOrg = "";
                String securityAnswerStaffOrg = "";

                switch (i + 1) {
                    case 1:
                        enterpriseName = "Brigham and Women's Hospital";

                        userNameHosAdminOrg = "BWHAdmin";
                        passwordHosAdminOrg = "BWHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "BWHAdmin";
                        securityHosAnswerAdminOrg = "BWHAdmin";

                        userNameDocOrg = "BWHDoc";
                        passwordDocOrg = "BWHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "BWHDoc";
                        securityAnswerDocOrg = "BWHDoc";

                        userNameStaffOrg = "BWHStaff";
                        passwordStaffOrg = "BWHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "BWHStaff";
                        securityAnswerStaffOrg = "BWHStaff";
                        break;

                    case 2:
                        enterpriseName = "Massachusetts General Hospital";
                        
                        userNameHosAdminOrg = "MGHAdmin";
                        passwordHosAdminOrg = "MGHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "MGHAdmin";
                        securityHosAnswerAdminOrg = "MGHAdmin";

                        userNameDocOrg = "MGHDoc";
                        passwordDocOrg = "MGHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "MGHDoc";
                        securityAnswerDocOrg = "MGHDoc";

                        userNameStaffOrg = "MGHStaff";
                        passwordStaffOrg = "MGHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "MGHStaff";
                        securityAnswerStaffOrg = "MGHStaff";
                        break;

                    case 3:
                        enterpriseName = "Brigham and Women's Faulkner Hospital";
                        
                        userNameHosAdminOrg = "BWFHAdmin";
                        passwordHosAdminOrg = "BWFHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "BWFHAdmin";
                        securityHosAnswerAdminOrg = "BWFHAdmin";

                        userNameDocOrg = "BWFHDoc";
                        passwordDocOrg = "BWFHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "BWFHDoc";
                        securityAnswerDocOrg = "BWFHDoc";

                        userNameStaffOrg = "BWFHStaff";
                        passwordStaffOrg = "BWFHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "BWFHStaff";
                        securityAnswerStaffOrg = "BWFHStaff";
                        break;

                    case 4:
                        enterpriseName = "Cooley Dickinson Hospital";
                        userNameHosAdminOrg = "CDHAdmin";
                        passwordHosAdminOrg = "CDHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "CDHAdmin";
                        securityHosAnswerAdminOrg = "CDHAdmin";

                        userNameDocOrg = "CDHDoc";
                        passwordDocOrg = "CDHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "CDHDoc";
                        securityAnswerDocOrg = "CDHDoc";

                        userNameStaffOrg = "CDHStaff";
                        passwordStaffOrg = "CDHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "CDHStaff";
                        securityAnswerStaffOrg = "CDHStaff";
                        
                        break;

                    case 5:
                        enterpriseName = "Martha's Vineyard Hospital";
                        
                        userNameHosAdminOrg = "MVHAdmin";
                        passwordHosAdminOrg = "MVHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "MVHAdmin";
                        securityHosAnswerAdminOrg = "MVHAdmin";

                        userNameDocOrg = "MVHDoc";
                        passwordDocOrg = "MVHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "MVHDoc";
                        securityAnswerDocOrg = "MVHDoc";

                        userNameStaffOrg = "MVHStaff";
                        passwordStaffOrg = "MVHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "MVHStaff";
                        securityAnswerStaffOrg = "MVHStaff";
                        break;

                    case 6:
                        enterpriseName = "MGH Institute of Health Professions";
                        userNameHosAdminOrg = "MGHIHPAdmin";
                        passwordHosAdminOrg = "MGHIHPAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "MGHIHPAdmin";
                        securityHosAnswerAdminOrg = "MGHIHPAdmin";

                        userNameDocOrg = "MGHIHPDoc";
                        passwordDocOrg = "MGHIHPDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "MGHIHPDoc";
                        securityAnswerDocOrg = "MGHIHPDoc";

                        userNameStaffOrg = "MGHIHPStaff";
                        passwordStaffOrg = "MGHIHPStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "MGHIHPStaff";
                        securityAnswerStaffOrg = "MGHIHPStaff";
                        break;

                    case 7:
                        enterpriseName = "McLean Hospital";
                        userNameHosAdminOrg = "MHAdmin";
                        passwordHosAdminOrg = "MHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "MHAdmin";
                        securityHosAnswerAdminOrg = "MHAdmin";

                        userNameDocOrg = "MHDoc";
                        passwordDocOrg = "MHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "MHDoc";
                        securityAnswerDocOrg = "MHDoc";

                        userNameStaffOrg = "MHStaff";
                        passwordStaffOrg = "MHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "MHStaff";
                        securityAnswerStaffOrg = "MHStaff";
                        break;

                    case 8:
                        enterpriseName = "Nantucket Cottage Hospital";
                        userNameHosAdminOrg = "NCHAdmin";
                        passwordHosAdminOrg = "NCHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "NCHAdmin";
                        securityHosAnswerAdminOrg = "NCHAdmin";

                        userNameDocOrg = "NCHDoc";
                        passwordDocOrg = "NCHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "NCHDoc";
                        securityAnswerDocOrg = "NCHDoc";

                        userNameStaffOrg = "NCHStaff";
                        passwordStaffOrg = "NCHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "NCHStaff";
                        securityAnswerStaffOrg = "NCHStaff";
                        break;

                    case 9:
                        enterpriseName = "Neighborhood Health Plan";
                        userNameHosAdminOrg = "NHPAdmin";
                        passwordHosAdminOrg = "NHPAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "NHPAdmin";
                        securityHosAnswerAdminOrg = "NHPAdmin";

                        userNameDocOrg = "NHPDoc";
                        passwordDocOrg = "NHPDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "NHPDoc";
                        securityAnswerDocOrg = "NHPDoc";

                        userNameStaffOrg = "NHPStaff";
                        passwordStaffOrg = "NHPStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "NHPStaff";
                        securityAnswerStaffOrg = "NHPStaff";
                        break;

                    case 10:
                        enterpriseName = "Newton-Wellesley Hospital";
                        userNameHosAdminOrg = "BWHAdmin";
                        passwordHosAdminOrg = "BWHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "BWHAdmin";
                        securityHosAnswerAdminOrg = "BWHAdmin";

                        userNameDocOrg = "BWHDoc";
                        passwordDocOrg = "BWHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "BWHDoc";
                        securityAnswerDocOrg = "BWHDoc";

                        userNameStaffOrg = "BWHStaff";
                        passwordStaffOrg = "BWHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "BWHStaff";
                        securityAnswerStaffOrg = "BWHStaff";
                        break;

                    case 11:
                        enterpriseName = "North Shore Medical Center";
                        userNameHosAdminOrg = "BWHAdmin";
                        passwordHosAdminOrg = "BWHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "BWHAdmin";
                        securityHosAnswerAdminOrg = "BWHAdmin";

                        userNameDocOrg = "BWHDoc";
                        passwordDocOrg = "BWHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "BWHDoc";
                        securityAnswerDocOrg = "BWHDoc";

                        userNameStaffOrg = "BWHStaff";
                        passwordStaffOrg = "BWHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "BWHStaff";
                        securityAnswerStaffOrg = "BWHStaff";
                        break;

                    case 12:
                        enterpriseName = "Partners Community Healthcare Inc";
                        userNameHosAdminOrg = "BWHAdmin";
                        passwordHosAdminOrg = "BWHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "BWHAdmin";
                        securityHosAnswerAdminOrg = "BWHAdmin";

                        userNameDocOrg = "BWHDoc";
                        passwordDocOrg = "BWHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "BWHDoc";
                        securityAnswerDocOrg = "BWHDoc";

                        userNameStaffOrg = "BWHStaff";
                        passwordStaffOrg = "BWHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "BWHStaff";
                        securityAnswerStaffOrg = "BWHStaff";
                        break;

                    case 13:
                        enterpriseName = "Partners HealthCare at Home";
                        userNameHosAdminOrg = "BWHAdmin";
                        passwordHosAdminOrg = "BWHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "BWHAdmin";
                        securityHosAnswerAdminOrg = "BWHAdmin";

                        userNameDocOrg = "BWHDoc";
                        passwordDocOrg = "BWHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "BWHDoc";
                        securityAnswerDocOrg = "BWHDoc";

                        userNameStaffOrg = "BWHStaff";
                        passwordStaffOrg = "BWHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "BWHStaff";
                        securityAnswerStaffOrg = "BWHStaff";
                        
                        break;

                    case 14:
                        enterpriseName = "Spaulding Rehabilitation Network";
                        userNameHosAdminOrg = "BWHAdmin";
                        passwordHosAdminOrg = "BWHAdmin";
                        empHosAdminOrg = null;
                        roleHosAdminOrg = new HospitalAdminRole();
                        securityHosQuestionAdminOrg = "BWHAdmin";
                        securityHosAnswerAdminOrg = "BWHAdmin";

                        userNameDocOrg = "BWHDoc";
                        passwordDocOrg = "BWHDoc";
                        empDocOrg = null;
                        roleDocOrg = new HospitalDoctorRole();
                        securityQuestionDcoOrg = "BWHDoc";
                        securityAnswerDocOrg = "BWHDoc";

                        userNameStaffOrg = "BWHStaff";
                        passwordStaffOrg = "BWHStaff";
                        empStaffOrg = null;
                        roleStaffOrg = new HospitalStaffRole();
                        securityQuestionStaffOrg = "BWHStaff";
                        securityAnswerStaffOrg = "BWHStaff";
                        break;
                }
                Enterprise enterprise = enterpriseDirectory.createAndAddEnterprise(enterpriseName, Enterprise.EnterpriseType.Hospital);

                //adding 
                OrganizationDirectory orgDir = enterprise.getOrganizationDirectory();

                for (int j = 0; j < 3; j++) {
                    String organizationName = "";
                    Type orgType = null;
                    switch (j + 1) {
                        case 1:
                            organizationName = "Hospital Admin Organization";
                            orgType = Organization.Type.HospitalAdminOrganization;
                            Organization org = orgDir.createOrganization(orgType, netwrokName);
                            org.getUserAccountDirectory().createUserAccount(userNameHosAdminOrg,
                                    passwordHosAdminOrg,
                                    empHosAdminOrg,
                                    roleHosAdminOrg,
                                    "Active",
                                    securityHosQuestionAdminOrg,
                                    securityHosAnswerAdminOrg);
                            break;
                        case 2:
                            organizationName = "Hospital Doctor Organization";
                            orgType = Organization.Type.HospitalDoctorOrganization;
                            Organization org2 = orgDir.createOrganization(orgType, netwrokName);
                             org2.getUserAccountDirectory().createUserAccount(userNameDocOrg,
                                    passwordDocOrg,
                                    empDocOrg,
                                    roleDocOrg,
                                    "Active",
                                    securityQuestionDcoOrg,
                                    securityAnswerDocOrg);
                        case 3:
                            organizationName = "Hospital Staff Organization";
                            orgType = Organization.Type.HospitalStaffOrganization;
                            Organization org3 = orgDir.createOrganization(orgType, netwrokName);
                            org3.getUserAccountDirectory().createUserAccount(userNameStaffOrg,
                                    passwordStaffOrg,
                                    empStaffOrg,
                                    roleStaffOrg,
                                    "Active",
                                    securityQuestionDcoOrg,
                                    securityAnswerStaffOrg);
                            break;
                    }
                }
            }
        }
    }

    private static void InitializeEnterprizeList() {
        EcoSystem system = EcoSystem.getEcoSystemInstance();
        if (null == system) {
            //error pending
            return;
        }
    }

    private static void InitializeRegisteredSupplierList() {
        EcoSystem system = EcoSystem.getEcoSystemInstance();
        if (null == system) {
            //error pending
            return;
        }
    }

    private static void InitializeSupplierEnterpriseList() {
        EcoSystem system = EcoSystem.getEcoSystemInstance();
        if (null == system) {
            //error pending
            return;
        }

        //add suppliers to supplier enterprise list
        ArrayList<SupplierEnterprise> supplierEnterpriseList = system.getSupplierEnterpriseList();
        for (int i = 0; i < 6; i++) {
            String enterpriseName = "";
            String userName = "";
            String password = "";
            Employee emp = null;
            Role role = null;
            String securityQuestion = "";
            String securityAnswer = "";

            String userNameAdminOrg = "";
            String passwordAdminOrg = "";
            Employee empAdminOrg = null;
            Role roleAdminOrg = null;
            String securityQuestionAdminOrg = "";
            String securityAnswerAdminOrg = "";

            String userNameShipOrg = "";
            String passwordShipOrg = "";
            Employee empShipOrg = null;
            Role roleShipOrg = null;
            String securityQuestionShipOrg = "";
            String securityAnswerShipOrg = "";

            switch (i + 1) {
                case 1:
                    enterpriseName = "General Electrics";
                    userName = "General Electrics";
                    password = "General Electrics";
                    //emp = new Employee();
                    role = new SupplierEnterpriseAdminRole();
                    securityQuestion = "General Electrics";
                    securityAnswer = "General Electrics";

                    userNameAdminOrg = "GeAdmin";
                    passwordAdminOrg = "GeAdmin";
                    empAdminOrg = null;
                    roleAdminOrg = new SupplierAdminRole();
                    securityQuestionAdminOrg = "GeAdmin";
                    securityAnswerAdminOrg = "GeAdmin";

                    userNameShipOrg = "GeShip";
                    passwordShipOrg = "GeShip";
                    empShipOrg = null;
                    roleShipOrg = new SupplierShippingRole();
                    securityQuestionShipOrg = "GeShip";
                    securityAnswerShipOrg = "GeShip";

                    break;

                case 2:
                    enterpriseName = "Medtronic Inc.";
                    userName = "Medtronic";
                    password = "Medtronic";
                    //emp = new Employee();
                    role = new SupplierEnterpriseAdminRole();
                    securityQuestion = "Medtronic";
                    securityAnswer = "Medtronic";

                    userNameAdminOrg = "MeAdmin";
                    passwordAdminOrg = "MeAdmin";
                    empAdminOrg = null;
                    roleAdminOrg = new SupplierAdminRole();
                    securityQuestionAdminOrg = "MeAdmin";
                    securityAnswerAdminOrg = "MeAdmin";

                    userNameShipOrg = "MeShip";
                    passwordShipOrg = "MeShip";
                    empShipOrg = null;
                    roleShipOrg = new SupplierShippingRole();
                    securityQuestionShipOrg = "MeShip";
                    securityAnswerShipOrg = "MeShip";
                    break;

                case 3:
                    enterpriseName = "Seimens";
                    userName = "Seimens";
                    password = "Seimens";
                    //emp = new Employee();
                    role = new SupplierEnterpriseAdminRole();
                    securityQuestion = "Seimens";
                    securityAnswer = "Seimens";

                    userNameAdminOrg = "SeAdmin";
                    passwordAdminOrg = "SeAdmin";
                    empAdminOrg = null;
                    roleAdminOrg = new SupplierAdminRole();
                    securityQuestionAdminOrg = "SeAdmin";
                    securityAnswerAdminOrg = "SeAdmin";

                    userNameShipOrg = "SeShip";
                    passwordShipOrg = "SeShip";
                    empShipOrg = null;
                    roleShipOrg = new SupplierShippingRole();
                    securityQuestionShipOrg = "SeShip";
                    securityAnswerShipOrg = "SeShip";
                    break;

                case 4:
                    enterpriseName = "Baxter International Inc.";
                    userName = "Baxter";
                    password = "Baxter";
                    //emp = new Employee();
                    role = new SupplierEnterpriseAdminRole();
                    securityQuestion = "Baxter";
                    securityAnswer = "Baxter";

                    userNameAdminOrg = "BaAdmin";
                    passwordAdminOrg = "BaAdmin";
                    empAdminOrg = null;
                    roleAdminOrg = new SupplierAdminRole();
                    securityQuestionAdminOrg = "BaAdmin";
                    securityAnswerAdminOrg = "BaAdmin";

                    userNameShipOrg = "BaShip";
                    passwordShipOrg = "BaShip";
                    empShipOrg = null;
                    roleShipOrg = new SupplierShippingRole();
                    securityQuestionShipOrg = "BaShip";
                    securityAnswerShipOrg = "BaShip";
                    break;

                case 5:
                    enterpriseName = "Stryker Corp";
                    userName = "Stryker";
                    password = "Stryker";
                    //emp = new Employee();
                    role = new SupplierEnterpriseAdminRole();
                    securityQuestion = "Stryker";
                    securityAnswer = "Stryker";

                    userNameAdminOrg = "StAdmin";
                    passwordAdminOrg = "StAdmin";
                    empAdminOrg = null;
                    roleAdminOrg = new SupplierAdminRole();
                    securityQuestionAdminOrg = "StAdmin";
                    securityAnswerAdminOrg = "StAdmin";

                    userNameShipOrg = "StShip";
                    passwordShipOrg = "StShip";
                    empShipOrg = null;
                    roleShipOrg = new SupplierShippingRole();
                    securityQuestionShipOrg = "StShip";
                    securityAnswerShipOrg = "StShip";
                    break;

                case 6:
                    enterpriseName = "Toshiba";
                    enterpriseName = "Toshiba";
                    userName = "Toshiba";
                    password = "Toshiba";
                    //emp = new Employee();
                    role = new SupplierEnterpriseAdminRole();
                    securityQuestion = "Toshiba";
                    securityAnswer = "Toshiba";

                    userNameAdminOrg = "ToAdmin";
                    passwordAdminOrg = "ToAdmin";
                    empAdminOrg = null;
                    roleAdminOrg = new SupplierAdminRole();
                    securityQuestionAdminOrg = "ToAdmin";
                    securityAnswerAdminOrg = "ToAdmin";

                    userNameShipOrg = "ToShip";
                    passwordShipOrg = "ToShip";
                    empShipOrg = null;
                    roleShipOrg = new SupplierShippingRole();
                    securityQuestionShipOrg = "ToShip";
                    securityAnswerShipOrg = "ToShip";
                    break;
            }
            SupplierEnterprise se = system.CreateAndAddSupplierEnterprise(enterpriseName);
            se.getUserAccountDirectory().createUserAccount(userName, password, emp,
                    role, "Active", securityQuestion, securityAnswer);

            //add supplier admin org to supplier enterprise
            SupplierAdminOrganization sao = (SupplierAdminOrganization) se.
                    getOrganizationDirectory().
                    createOrganization(
                            Organization.Type.SupplierAdminOrganization,
                            "Supplier Admin Organization");
            sao.getUserAccountDirectory().createUserAccount(userNameAdminOrg,
                    passwordAdminOrg,
                    empAdminOrg,
                    roleAdminOrg,
                    passwordAdminOrg,
                    securityQuestionAdminOrg,
                    securityAnswerAdminOrg);

            //add supplier ship org to supplier enterprise
            SupplierShippingOrganization sso = (SupplierShippingOrganization) se.
                    getOrganizationDirectory().
                    createOrganization(Organization.Type.SupplierShippingOrganization,
                            "Supplier Shipping Organization");
            sso.getUserAccountDirectory().createUserAccount(userNameShipOrg,
                    passwordShipOrg,
                    empShipOrg,
                    roleShipOrg,
                    passwordShipOrg,
                    securityQuestionShipOrg,
                    securityAnswerShipOrg);
        }
    }

    private static void InitializeProcurementEnterprise() {
        EcoSystem system = EcoSystem.getEcoSystemInstance();
        if (null == system) {
            //error pending
            return;
        }

        //create and add procurement admin organization
        ProcurementAdminOrganization pao = (ProcurementAdminOrganization) system.
                getProcurementEnterprise().
                getOrganizationDirectory().
                createOrganization(
                        Organization.Type.ProcurementAdminOrganization,
                        "Procurement Admin Organization");

        //add employee to procurement admin organization
        Employee emp = pao.getEmployeeDirectory().createEmployee("Procurement Admin", 26);

        //add user account for procurement admin        
        Role role = new ProcurementAdminRole();
        UserAccount ua = pao.getUserAccountDirectory().createUserAccount(
                "Procurement Admin",
                "Procurement Admin",
                emp,
                role,
                "Procurement Admin",
                "Procurement Admin",
                "Procurement Admin");
    }

    private static void InitializeInventoryEnterprise() {
        EcoSystem system = EcoSystem.getEcoSystemInstance();
        if (null == system) {
            //error pending
            return;
        }
        //create and add inventory admin organization
        InventoryAdminOrganization iao = (InventoryAdminOrganization) system.getInventoryEnterprise().getOrganizationDirectory().createOrganization(
                Organization.Type.InventoryAdminOrganization,
                "Inventory Admin Organization");

        //add employee to inventory admin organization
        Employee emp = iao.getEmployeeDirectory().createEmployee("Inventory Admin", 26);

        //add user account for inventory admin
        Role role = new InventoryAdminRole();
        UserAccount ua = iao.getUserAccountDirectory().createUserAccount(
                "Inventory Admin",
                "Inventory Admin",
                emp,
                role,
                "Inventory Admin",
                "Inventory Admin",
                "Inventory Admin");
    }

    private static void InitializeMaintenanceEnterprise() {
        EcoSystem system = EcoSystem.getEcoSystemInstance();
        if (null == system) {
            //error pending 
            return;
        }

        //create and add maintenance admin organization
        MaintenanceAdminOrganization mao = (MaintenanceAdminOrganization) system.getMaintenanceEnterprise().getOrganizationDirectory().createOrganization(
                Organization.Type.MaintenanceAdminOrganization,
                "Maintenance Admin Organization");

        //create employee for maintenance admin organization
        Employee emp = mao.getEmployeeDirectory().createEmployee("Inventory Admin", 26);

        //create user account for maintenance admin organization
        Role role = new MaintenanceAdminRole();
        UserAccount ua = mao.getUserAccountDirectory().createUserAccount(
                "Maintenance Admin",
                "Maintenance Admin",
                emp,
                role,
                "Maintenance Admin",
                "Maintenance Admin",
                "Maintenance Admin");
    }
}
