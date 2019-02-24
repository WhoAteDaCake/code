package uk.ac.nott.cs.g53dia.agent;

import java.util.Random;

import uk.ac.nott.cs.g53dia.library.Action;
import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.EmptyCell;
import uk.ac.nott.cs.g53dia.library.MoveAction;
import uk.ac.nott.cs.g53dia.library.Station;
import uk.ac.nott.cs.g53dia.library.Tanker;

public class IntelligentTanker extends Tanker {
	State state = State.SCOUTING;
	World world = new World(Tanker.VIEW_RANGE);
	Path activePath;

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
	 * Find all cells nearby that are special (well/station/pump) TODO: in the
	 * future probably don't need to run it every single move
	 */
	private void analyseView(Cell[][] view) {
		for (int x = 0; x < view.length; x += 1) {
			for (int y = 0; y < view[x].length; y += 1) {
				Cell current = view[x][y];

				if (current instanceof EmptyCell) {
					continue;
				}
				Group.Group2<Integer, Integer> coords = world.getGlobalCoords(x, y);
				if (world.hasSeenCell(coords)) {
					// If it's a station, it should be updated in order to make sure any new tasks
					// are retrieved
					if (current instanceof Station) {
						world.updateCell(current, coords);
					}

				} else {
					world.registerCell(current, coords);
				}

				// This means that it's a special cell
//				world.registerCell(current, x, y);
			}
		}
	}

	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		// Because we have no resource restrictions, it's best to just analyse every
		// time we move
		analyseView(view);

		// If we have a path, should always complete it.
		if (this.activePath != null && this.activePath.hasSteps()) {
			return new MoveAction(activePath.step());
		}

		int nextMove = this.r.nextInt(8);

		if (this.state == State.SCOUTING) {
			Group.Group2<Integer, Path> result = world.findClosestCell(CellType.STATION);
			if (result != null) {
				activePath = result.second;
				nextMove = activePath.step();
				this.state = State.MOVING_TO_STATION;
			}
			return new MoveAction(nextMove);
		} else if (this.state == State.MOVING_TO_STATION) {
			return new MoveAction(0);
		}

		return null;
	}

}
