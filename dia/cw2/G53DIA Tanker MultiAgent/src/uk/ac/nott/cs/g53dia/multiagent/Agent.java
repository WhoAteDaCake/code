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
	Manager m;
	State state = State.ROAMING;
	Group.Group2<Integer, Integer> coords = Group.make2(0, 0);	
	
	public Agent(Random rand, Manager manager) {
		r = rand;
		m = manager;
	}
	
	// Track future position before agent moves
	private Action registeredMove(int direction) {
		Group.Group2<Integer, Integer> change = Path.moveChange(direction);
		coords = Group.make2(coords.first + change.first, coords.second + change.second);
		return new MoveAction(direction);
	}
	
	/*
	 * Find all cells nearby that are special (well/station/pump) TODO: in the
	 * future probably don't need to run it every single move
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
		// TODO Auto-generated method stub
		return registeredMove(r.nextInt(8));
	}

}
