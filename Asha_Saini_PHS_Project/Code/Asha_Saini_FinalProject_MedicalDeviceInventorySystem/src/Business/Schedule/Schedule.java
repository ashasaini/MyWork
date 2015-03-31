/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package Business.Schedule;

import java.util.ArrayList;
import java.util.Date;

/**
 *
 * @author Asha
 */
public class Schedule {
    ArrayList<Reservation> reservationList;

    public Schedule() {
        this.reservationList = new ArrayList<Reservation>();
    }
    
    public ArrayList<Reservation> getReservationList() {
        return reservationList;
    }

    public void setReservationList(ArrayList<Reservation> reservationList) {
        this.reservationList = reservationList;
    }
    
    public Reservation addReservation(Date startDate,Date endDate){
        Reservation r = new Reservation(startDate,endDate);
        reservationList.add(r);
        return r;
    }
    
    public void deleteReservation(Reservation r){
        reservationList.remove(r);
    }
}
