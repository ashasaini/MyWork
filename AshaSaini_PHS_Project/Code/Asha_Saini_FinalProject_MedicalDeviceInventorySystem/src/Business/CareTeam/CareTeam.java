/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package Business.CareTeam;

import Business.Schedule.Schedule;

/**
 *
 * @author Asha
 */
public class CareTeam {

    private String teamName;
    private TeamLeader teamLeader;
    private TeamMember teamMember;
    private Schedule schedule;
    private int id;
    private static int count = 0;

    public CareTeam() {
    }
    
    public CareTeam(String teamName, String teamLeadName, int age) {
        this.teamName = teamName;
        this.schedule = new Schedule();
        this.teamLeader = new TeamLeader(teamLeadName, age);
        this.teamMember = new TeamMember();
        this.count++;
        this.id = count;
    }

    public String getTeamName() {
        return teamName;
    }

    public void setTeamName(String teamName) {
        this.teamName = teamName;
    }

    public Schedule getSchedule() {
        return schedule;
    }

    public void setSchedule(Schedule schedule) {
        this.schedule = schedule;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public TeamLeader getTeamLeader() {
        return teamLeader;
    }

    public void setTeamLeader(TeamLeader teamLeader) {
        this.teamLeader = teamLeader;
    }

    public TeamMember getTeamMember() {
        return teamMember;
    }

    public void setTeamMember(TeamMember teamMember) {
        this.teamMember = teamMember;
    }

    @Override
    public String toString() {
        return teamName;
    }
}
