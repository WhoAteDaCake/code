package uk.ac.nott.cs.g53dia.multiagent;

import java.util.Random;

import uk.ac.nott.cs.g53dia.multilibrary.Action;
import uk.ac.nott.cs.g53dia.multilibrary.Cell;
import uk.ac.nott.cs.g53dia.multilibrary.EmptyCell;
import uk.ac.nott.cs.g53dia.multilibrary.FuelPump;
import uk.ac.nott.cs.g53dia.multilibrary.MoveAction;
import uk.ac.nott.cs.g53dia.multilibrary.Station;
import uk.ac.nott.cs.g53dia.multilibrary.Tanker;

public class Agent extends Tanker {
	private Manager m;
	public int id;
	public Cell[][] view;
	// For debugging
	public long timestep;
	
	public int initiationSteps = 0;
	public int initiationDirection;
	/*
	 * Each move takes 2 fuel
	 * So we need to limit to less than (Tanker.MAX_FUEL / 2) / 2
	 * Otherwise we would not be able to get back
	 */
	public int initiationLimit = (Tanker.MAX_FUEL / 4) - 1;
	
	// These will be set by the manager
	public Path path = null;
	public State state = State.ROAMING;
	public Path toTarget = null;
	public Pair<Integer, Integer> target = null;
	public Pair<Integer, Integer> pTarget = null;
	public Pair<Integer, Integer> coords = new Pair<>(0, 0);
	
	/**
	 * Chose diagonal based on id
	 */
	public static int getInitiationDir(int id) {
		return 4 + (id % 4);
	}
	
	public Agent(Random rand, Manager manager, int myId) {
		r = rand;
		m = manager;
		id = myId;
		
		m.register(this);
		initiationDirection = getInitiationDir(myId);
	}
	
	// Track future position before agent moves
	private Action registeredMove(int direction) {
		Pair<Integer, Integer> change = Path.moveChange(direction);
		coords = new Pair<>(coords.first + change.first, coords.second + change.second);
		return new MoveAction(direction);
	}
	
	public void freeTargets() {
		if (m.w.reserved.contains(target)) {
			m.w.free(target, this);
		}
		toTarget = null;
		target = null;
		pTarget = null;	
	}
	
	/*
	 * Find all cells nearby that are special (well/station/pump)
	 * and save them to the grid
	 */
	private void analyseView() {
		for (int x = 0; x < view.length; x += 1) {
			for (int y = 0; y < view[x].length; y += 1) {
				Cell current = view[x][y];
				CellType type;

				if (current instanceof EmptyCell) {
					continue;
				} else if (current instanceof Station) {
					type = CellType.STATION;
				} else if (current instanceof FuelPump) {
					type = CellType.PUMP;
				} else {
					type = CellType.WELL;
				}
				m.w.saveGridCell(coords, current, type, x, y);
			}
		}
	}
	
	
	public Cell myCell() {
		return getCurrentCell(view);
	}
	
	@Override
	public Action senseAndAct(Cell[][] aview, boolean actionFailed, long tstep) {
		timestep = tstep;
		view = aview;
		analyseView();
		
		// Make sure initiation is ran first
		// This way the logic inside manager is simpler
		if (initiationSteps < initiationLimit) {
			initiationSteps += 1;
			return registeredMove(initiationDirection);
		}
		// Following a path is always a strong goal
		// This allows the logic in the Manager class to be more simple
		// By not considering movement
		if (path != null && path.hasSteps()) {
			return registeredMove(path.step());
		}
		Pair<Action, Integer> result = m.asignAction(this);
		return result.first != null ? result.first : registeredMove(result.second);
	}

	public String toString() {
		return String.format("Agent: %d | Coords: %s | Fuel: %d | State: %s", id, coords.toString(), getFuelLevel(), state.toString());
	}
}
