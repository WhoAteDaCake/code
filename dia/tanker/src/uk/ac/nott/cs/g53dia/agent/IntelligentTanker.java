package uk.ac.nott.cs.g53dia.agent;

import java.util.Random;

import uk.ac.nott.cs.g53dia.library.Action;
import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.MoveAction;
import uk.ac.nott.cs.g53dia.library.Station;
import uk.ac.nott.cs.g53dia.library.Tanker;

public class IntelligentTanker extends Tanker {
	State state = State.SCOUTING;
	World world = new World(Tanker.VIEW_RANGE);

	public IntelligentTanker() {
		this(new Random(50));
	}

	/**
	 * The tanker implementation makes random moves. For reproducibility, it can
	 * share the same random number generator as the environment.
	 * 
	 * @param r The random number generator.
	 */
	public IntelligentTanker(Random r) {
		this.r = r;
	}

	/*
	 * Looks for the most "relevant" cell. Depending on current state it might be a
	 * well, refuelling station or a waste station. Will return the cell as well as
	 * it's coordinates in the view array
	 */
	private Pair<Cell, Pair<Integer, Integer>> findRelevantCell(Cell[][] view) {
		Cell found = null;
		int lastDist = Integer.MAX_VALUE;
		int row = -1;
		int col = -1;

		for (int x = 0; x < view.length; x += 1) {
			for (int y = 0; y < view[x].length; y += 1) {
				Cell current = view[x][y];
				if (!(current instanceof Station) && state == State.SCOUTING) {
					continue;
				}
				// TODO Account for other states.
				// Manhattan distance
				int newDist = Math.abs(Tanker.VIEW_RANGE - y) + Math.abs(Tanker.VIEW_RANGE - x);
				if (newDist < lastDist) {
					found = current;
					lastDist = newDist;
					row = y;
					col = x;
				}
			}
		}
		return Pair.make(found, Pair.make(row, col));
	}

	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		// TODO Auto-generated method stub
		Pair<Cell, Pair<Integer, Integer>> result = findRelevantCell(view);

		int nextMove;
		// Means we didn't find a relevant cell
		if (result.first == null) {
			nextMove = this.r.nextInt(8);
		} else {
			nextMove = world.registerCell(result.first, result.second.first, result.second.second);
		}
		return new MoveAction(nextMove);
	}

}
