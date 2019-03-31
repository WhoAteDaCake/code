package uk.ac.nott.cs.g53dia.multiagent;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Random;

import uk.ac.nott.cs.g53dia.multilibrary.*;

public class Manager {
	private Random r;
	// For performance improvement
	private Group.Group2<Integer, Integer> lastTStation;
	
	public World w;
	
	
	public Manager(Random rand, int gridSize, int maxFuel) {
		r = rand;
		w = new World(gridSize, maxFuel);
	}
	
	private Group.Group2<Integer, Integer> getRoamTarget(Agent agent) {
		// Need a list here because we are accessing random indices
		ArrayList<Group.Group2<Integer, Integer>> stations = new ArrayList<>(w.getFreeStations());
		
		while (stations.size() > 0) {
			int index = r.nextInt(stations.size() > 1 ? stations.size() - 1 : 1);
			Group.Group2<Integer, Integer> coords = stations.get(index);
			boolean notVisited = !coords.equals(agent.pTarget) && !coords.equals(agent.coords);
			if (notVisited && w.isReachable(agent.coords, coords, agent.getFuelLevel()).first) {
				w.reserve(coords);
				agent.pTarget = agent.target;
				agent.target = coords;
				return coords;
			} else {
				stations.remove(coords);
			}
		}

		int modifier = r.nextInt(10) > 5 ? -20 : 20;
		Group.Group2<Integer, Integer> coords = Group.make2(
				agent.coords.first + modifier,
				agent.coords.second + modifier);
		if (w.isReachable(agent.coords, coords, agent.getFuelLevel()).first) {
			return coords;
		}
		return null;
	}
	
	// Returns -1, when the agent can do better than roaming
	// Returns -2, when a refuel is needed
	private int roam(Agent agent) {
		Group.Group2<Group.Group2<Integer, Integer>, Boolean> result = w.getNearestTaskStation(agent);
		if (result != null) {
			lastTStation = result.first;
			// Reset targets
			if (w.reserved.contains(agent.target)) {
				w.free(agent.target);
			}
			agent.toTarget = null;
			agent.target = null;
			agent.pTarget = null;
			
			return result.second ? -1 : -2;
		}
		
		// Target section
		if (agent.toTarget != null && agent.toTarget.hasSteps()) {
			return agent.toTarget.step();
		} else if (w.reserved.contains(agent.target)) {
			w.free(agent.target);
		}
		
		Group.Group2<Integer, Integer> target = getRoamTarget(agent);
		if (target != null) {
			agent.toTarget = Path.movesToPoint(agent.coords, target);
			return agent.toTarget.step();
		}
		return -2;
	}

	private Group.Group2<Action, Integer> refuel(Agent agent) {
		agent.state = State.MOVING_TO_FUEL;
		agent.path = Path.movesToPoint(agent.coords, w.getBestCell(CellType.PUMP, agent.coords));
		return Group.make2(null, agent.path.step());
	}
	
	// Returns action or direction that the tanker should perform
	public Group.Group2<Action, Integer> asignAction(Agent agent, Cell[][] view) {
		if (agent.state == State.ROAMING) {
			int direction = roam(agent);
			if (direction == -1) {
				agent.state = State.MOVING_TO_STATION;
				agent.path = Path.movesToPoint(agent.coords, lastTStation);
				// Make sure no other tanks go for the same place
				w.reserve(lastTStation);
				// Happens when a station generates a task, while we are standing on it
				if (agent.path.stepCount() == 0) {
					return asignAction(agent, view);
				}
				return Group.make2(null, agent.path.step());
			} else if (direction == -2) {
				return refuel(agent);
			} else {
				return Group.make2(null, direction);
			}
		// Can assume, that at this point agent is standing on a station
		} else if (agent.state == State.MOVING_TO_STATION) {
			agent.state = State.CONSUMING;
			Cell cell = agent.getCurrentCell(view);
			Task task = ((Station) cell).getTask();
			return Group.make2(new LoadWasteAction(task) , null);
		// Move to the well
		} else if (agent.state == State.CONSUMING) {
			// No longer reserved
			w.free(agent.coords);
			Group.Group2<Group.Group2<Integer, Integer>, Boolean> meta = w.getWell(agent);
			// Not enough fuel
			if (!meta.second) {
				return refuel(agent);
			}
			agent.state = State.MOVING_TO_WELL;
			agent.path = Path.movesToPoint(agent.coords, meta.first);
			return Group.make2(null, agent.path.step());
		// Assume already at the well
		} else if (agent.state == State.MOVING_TO_WELL) {
			agent.state = State.DISPOSING;
			return Group.make2(new DisposeWasteAction(), null);
		} else if (agent.state == State.DISPOSING) {
			agent.state = State.ROAMING;
			return asignAction(agent, view);
		// Assume already at the pump
		} else if (agent.state == State.MOVING_TO_FUEL) {
			agent.state = State.REFUELING;
			return Group.make2(new RefuelAction(), null);
		} else if (agent.state == State.REFUELING) {
			if (agent.getWasteLevel() != 0) {
				agent.state = State.CONSUMING;
			} else {
				agent.state = State.ROAMING;
			}
			return asignAction(agent, view);
		}
			
		return null;
	}
}
