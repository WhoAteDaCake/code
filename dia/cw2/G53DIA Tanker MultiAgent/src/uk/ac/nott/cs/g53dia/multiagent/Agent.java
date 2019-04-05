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
	
	// These will be set by the manager
	public Path path = null;
	public State state = State.ROAMING;
	public Path toTarget = null;
	public Group2<Integer, Integer> target = null;
	public Group2<Integer, Integer> pTarget = null;
	public Group2<Integer, Integer> coords = new Group2<>(0, 0);
	
	public Agent(Random rand, Manager manager, int myId) {
		r = rand;
		m = manager;
		id = myId;
		
		m.register(this);
	}
	
	// Track future position before agent moves
	private Action registeredMove(int direction) {
		Group2<Integer, Integer> change = Path.moveChange(direction);
		coords = new Group2<>(coords.first + change.first, coords.second + change.second);
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
	 */
	private void analyseView() {
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
	
	
	public Cell myCell() {
		return getCurrentCell(view);
	}
	
	@Override
	public Action senseAndAct(Cell[][] aview, boolean actionFailed, long tstep) {
		timestep = tstep;
		view = aview;
		analyseView();
		
		if (timestep >= 870 && id == 1) {
			int a = 2;
		}
		
		if (path != null && path.hasSteps()) {
			return registeredMove(path.step());
		}
		Group2<Action, Integer> result = m.asignAction(this);
		return result.first != null ? result.first : registeredMove(result.second);
	}

	public String toString() {
		return "Agent: " + id;
	}
}
