/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.Order;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class MasterOrderCatalog {

    private ArrayList<Order> orderList;

    public MasterOrderCatalog() {
        orderList = new ArrayList<Order>();
    }

    public ArrayList<Order> getOrderList() {
        return orderList;
    }

    public Order addOrder(Order order) {
        orderList.add(order);
        return order;
    }

    public void removeOrder(Order order) {
        orderList.remove(order);
    }
}
