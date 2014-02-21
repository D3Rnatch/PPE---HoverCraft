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
public class grid {
    final static int SIZE_H = 30; // 30 cm largeur
    final static int SIZE_W = 30; // 30 cm hauteur
    final static int MAP_SIZE = 11;
    final static int DIST_MAX = 3; // 3 cases max
    
    public int x,y; // Starting points
    private int prev_x, prev_y; // Previous points
    private int [][] map; // SIZE_H - SIZE_W
    public ArrayList<Integer> previous_pose;
    
    private int xa,ya;
    private int xb,yb;
    private int xc,yc;
    
    public grid()
    {
        for(int i=0;i<MAP_SIZE;i++)
            for(int j= 0; j< MAP_SIZE;j++)
                map[i][j] = 0; // There's Nothing !!!
        this.x = this.y = this.prev_x = this.prev_y = MAP_SIZE/2;
        this.xa = this.ya = this.xb = this.yb = this.xc = this.yc = 0;
    }
    
    public void setObstacle(int cx, int cy, int v) throws Exception
    {
        if(cx < MAP_SIZE || cx >= 0 || cy < MAP_SIZE || cy >= 0)
            this.map[cx][cy] = v;
        else
            throw new Exception("Out of range detection !");
    }
    
    public int getObstacle(int cx, int cy) throws Exception
    {
        if(cx < MAP_SIZE || cx >= 0 || cy < MAP_SIZE || cy >= 0) 
            return this.map[cx][cy];
        else
        {
            throw new Exception("Out of range asking !");
        }
    }
    
    public boolean addObstacle(int dist1, int dist2, int angle)
    {
        boolean ret = false;
        double cx,cy = 0;
        
        // Calcul premier obstacle
        cx = dist1 * java.lang.Math.cos(angle); 
        cy = dist1 * java.lang.Math.sin(angle);
        
        // In real into the MAP
        cx = this.x - cx;
        cy = this.y - cy;
        
        try
        {
            setObstacle((int)cx,(int)cy,1);
        }
        catch(Exception e)
        {
            System.out.println(e);
        }
        
        // Calcul premier obstacle
        cx = dist2 * java.lang.Math.cos(angle + 180); 
        cy = dist2 * java.lang.Math.sin(angle + 180);
        
        // In real into the Map
        cx = this.x - cx;
        cy = this.y - cy;
        
        try
        {
            setObstacle((int)cx,(int)cy,1);
        }
        catch(Exception e)
        {
            System.out.println(e);
        }
        
        return ret;
    }
    
    public boolean localizeHovercraft() throws Exception
    {
        boolean ret = false;
        
        return ret;
    }
    
    /*
    public int moveHovercraft()
    {
        int ret = -1; // There's a bug !
        
        int id = 0;
	for(int i=this.x; i<this.x+1 && this.x+i < MAP_SIZE; i++){
		for(int j=this.y; j<this.y+1 && this.y+j < MAP_SIZE; j++)
                {
			if(this.map[i][j] >= 90 && i != this->prev_x && i != this->x && j != this->prev_y && j != this->y)
				// push_vector(t , i, j);
				
                                if(b < 3)
                                    follow_walls(t, ai, i, j, b+1);
				id = 1;
				break;
		}
		if(id != 0) break;
	}
        
        return ret;
    }
    public void followWalls()
    {

    }
    */
    
    public void localizeHovercraft()
    {
        
        
    }
}

