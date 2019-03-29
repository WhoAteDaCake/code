package uk.ac.nott.cs.g53dia.multiagent;
import uk.ac.nott.cs.g53dia.multilibrary.*;

import java.util.*;

public class DemoFleet extends Fleet {
	Manager m;
    /** 
     * Number of tankers in the fleet (this is just an example, not a requirement).
     */
    private static int FLEET_SIZE = 5;
    
    public DemoFleet() {
    	this(new Random());
    }

	/**
	 * The DemoTanker implementation makes random moves. For reproducibility, it
	 * can share the same random number generator as the environment.
	 * 
	 * @param r
	 *            The random number generator.
	 */
    public DemoFleet(Random r) {
    	// Create the tankers
    	m = new Manager(r);
		for (int i=0; i<FLEET_SIZE; i++) {
		    this.add(new Agent(r, m));
		}
    }
}
