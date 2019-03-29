package uk.ac.nott.cs.g53dia.multiagent;

import java.util.Random;

import uk.ac.nott.cs.g53dia.multilibrary.Action;
import uk.ac.nott.cs.g53dia.multilibrary.Cell;
import uk.ac.nott.cs.g53dia.multilibrary.Tanker;

public class Agent extends Tanker {
	Manager m;
	State state = State.ROAMING;
	Group.Group2<Integer, Integer> coords = Group.make2(0, 0);	
	
	public Agent(Random rand, Manager manager) {
		r = rand;
		m = manager;
	}
	
	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		// TODO Auto-generated method stub
		return null;
	}

}
