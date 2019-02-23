package uk.ac.nott.cs.g53dia.agent;

import java.util.Random;

import uk.ac.nott.cs.g53dia.library.Action;
import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.Tanker;
import uk.ac.nott.cs.g53dia.agent.State;

public class IntelligentTanker extends Tanker {
	State state = State.SCOUTING;
	
	public IntelligentTanker() {
		this(new Random());
	}

	/**
	 * The tanker implementation makes random moves. For reproducibility, it
	 * can share the same random number generator as the environment.
	 * 
	 * @param r
	 *            The random number generator.
	 */
	public IntelligentTanker(Random r) {
		this.r = r;
	}

	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		// TODO Auto-generated method stub
		return null;
	}

}
