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
	
	// These will be set by the manager
	public Path path = null;
	public State state = State.ROAMING;
	public Path toTarget = null;
	public Group.Group2<Integer, Integer> target = null;
	public Group.Group2<Integer, Integer> pTarget = null;
	public Group.Group2<Integer, Integer> coords = Group.make2(0, 0);
	
	public Agent(Random rand, Manager manager, int myId) {
		r = rand;
		m = manager;
		id = myId;
	}
	
	// Track future position before agent moves
	private Action registeredMove(int direction) {
		Group.Group2<Integer, Integer> change = Path.moveChange(direction);
		coords = Group.make2(coords.first + change.first, coords.second + change.second);
		return new MoveAction(direction);
	}
	
	/*
	 * Find all cells nearby that are special (well/station/pump)
	 */
	private void analyseView(Cell[][] view) {
		// Make sure pump is loaded to allow for distance calculations
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
	
	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		analyseView(view);
		
		if (timestep >= 90) {
			int a = 2;
		}
		
		if (path != null && path.hasSteps()) {
			return registeredMove(path.step());
		}
		Group.Group2<Action, Integer> result = m.asignAction(this, view);
		return result.first != null ? result.first : registeredMove(result.second);
	}

	public String toString() {
		return "Agent: " + id;
	}
}
