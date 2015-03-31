/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Network;

import Business.Enterprise.EnterpriseDirectory;

/**
 *
 * @author Asha
 */
public class Network {
    private String networkName;
    private EnterpriseDirectory enterpriseDirectory;

    public Network(String networkName) {
        this.networkName = networkName;
        enterpriseDirectory = new EnterpriseDirectory();
    }

    public EnterpriseDirectory getEnterpriseDirectory() {
        return enterpriseDirectory;
    }

    public String getNetworkName() {
        return networkName;
    }

    public void setNetworkName(String f_networkName) {
        this.networkName = f_networkName;
    }

    @Override
    public String toString() {
        return networkName;
    }
}
