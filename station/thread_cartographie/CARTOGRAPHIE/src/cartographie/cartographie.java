/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cartographie;

import java.util.Deque;

/**
 *
 * @author Alex
 */
public class cartographie {
    
    // GRID DATA
    // -- GRID CALCULUS
    private grid the_grid;
    
    // HOVERCRAFT DATA
    // -- Coordinates
    private pose current_pose;
    private pose previous_pose;
    
    // HISTORIC POSES 
    // -- Historic of the followed Path
    private Deque<pose> historic_pose;
    
    public cartographie()
    {
        
    }
}
