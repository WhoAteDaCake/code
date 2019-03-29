package uk.ac.nott.cs.g53dia.multiagent;

import java.util.Random;

public class Manager {
	private Random r;
	public World w;
	
	public Manager(Random rand) {
		r = rand;
		w = new World();
	}
}
