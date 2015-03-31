/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.MaintenanceSchedule;

import java.util.Date;

/**
 *
 * @author Asha
 */
public class MaintenanceSchedule {
    private Date lastMaintenanceDate;
    private Date nextMaintenanceDate;

    public MaintenanceSchedule() {
        lastMaintenanceDate = new Date();
        nextMaintenanceDate =new Date();
    }

    public Date getLastMaintenanceDate() {
        return lastMaintenanceDate;
    }

    public void setLastMaintenanceDate(Date lastMaintenanceDate) {
        this.lastMaintenanceDate = lastMaintenanceDate;
    }

    public Date getNextMaintenanceDate() {
        return nextMaintenanceDate;
    }

    public void setNextMaintenanceDate(Date nextMaintenanceDate) {
        this.nextMaintenanceDate = nextMaintenanceDate;
    }
}
