/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.CareTeam;

import java.util.ArrayList;

/**
 *
 * @author Asha
 */
public class CareTeamDirectory {

    ArrayList<CareTeam> careTeamList;

    public CareTeamDirectory() {
        careTeamList = new ArrayList<>();
    }

    public ArrayList<CareTeam> getCareTeamList() {
        return careTeamList;
    }

    public void setCareTeamList(ArrayList<CareTeam> careTeamList) {
        this.careTeamList = careTeamList;
    }

    public CareTeam addCareTeam(String teamName, String teamLeadName, int age) {
        CareTeam p = new CareTeam(teamName, teamLeadName, age);
        careTeamList.add(p);
        return p;
    }

    public CareTeam searchCareTeambyId(int id) {
        for (CareTeam p : careTeamList) {
            if (id == p.getId()) {
                return p;
            }
        }
        return null;
    }

    public ArrayList<CareTeam> searchCareTeambyName(String name) {
        ArrayList<CareTeam> serachResults = new ArrayList<>();
        for (CareTeam p : careTeamList) {
            if (p.getTeamName().contains(name)) {
                serachResults.add(p);
            }
        }
        return serachResults;
    }

    public void deleteCareTeam(CareTeam p) {
        careTeamList.remove(p);
    }

}
