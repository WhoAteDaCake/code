package uk.ac.nott.cs.g53dia.multiagent;

import java.util.Random;

import uk.ac.nott.cs.g53dia.multilibrary.Action;

public class Manager {
	private Random r;
	public World w;
	
	public Manager(Random rand, int gridSize) {
		r = rand;
		w = new World(gridSize);
	}
	
	// Returns action or direction that the tanker should perform
	public Group.Group2<Action, Integer> asignAction(Agent agent) {
		
		return null;
	}
}
