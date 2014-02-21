/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package cartographie;

import java.util.ArrayList;

/**
 *
 * @author Alex
 */
public class RemarquableObject {
    final static int OBJECT_SIZE_MAX = 10;

    public ArrayList<pose> landmarks; // Landmarks composing the remarquableObject
    public int[][] mask; // Validation mask
    public pose original_pose;
    
    public RemarquableObject()
    {
        landmarks = new ArrayList<pose>();
        for(int i=0;i<OBJECT_SIZE_MAX;i++)
            for(int j=0;j<OBJECT_SIZE_MAX;j++)
                   this.mask[i][j] = 0;
        this.original_pose = new pose();
    }
    
    public RemarquableObject(pose o)
    {
        landmarks = new ArrayList<pose>();
        for(pose e : landmarks)
            for(int i=0;i<OBJECT_SIZE_MAX;i++)
                for(int j=0;j<OBJECT_SIZE_MAX;j++)
                  this.mask[i][j] = 0;
        this.original_pose = o;
    }
    
    public RemarquableObject(ArrayList<pose> ld, pose o)
    {
        landmarks = ld;
        for(pose e : ld)
            for(int i=0;i<OBJECT_SIZE_MAX;i++)
                for(int j=0;j<OBJECT_SIZE_MAX;j++)
                  this.mask[i][j] = 0;
        this.original_pose = o;
    }
    
}
