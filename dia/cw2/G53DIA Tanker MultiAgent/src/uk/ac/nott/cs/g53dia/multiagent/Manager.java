package uk.ac.nott.cs.g53dia.multiagent;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.Random;

import uk.ac.nott.cs.g53dia.multilibrary.Action;

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
			int index = r.nextInt(stations.size() > 0 ? stations.size() - 1 : 0);
			Group.Group2<Integer, Integer> coords = stations.get(index);
			if (w.isReachable(agent.coords, coords, agent.getFuelLevel()).first) {
				w.reserve(coords);
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
			return result.second ? -1 : -2;
		}
		
		// Target section
		if (agent.toTarget != null && agent.toTarget.hasSteps()) {
			return agent.toTarget.step();
		}
		Group.Group2<Integer, Integer> target = getRoamTarget(agent);
		if (target != null) {
			agent.toTarget = Path.movesToPoint(agent.coords, target);
			return agent.toTarget.step();
		}
		return -2;
	}

	// Returns action or direction that the tanker should perform
	public Group.Group2<Action, Integer> asignAction(Agent agent) {
		if (agent.state == State.ROAMING) {
			int direction = roam(agent);
			if (direction == -1) {
				agent.state = State.MOVING_TO_STATION;
				agent.path = Path.movesToPoint(agent.coords, lastTStation);
				return Group.make2(null, agent.path.step());
			} else if (direction == -2) {
				agent.state = State.MOVING_TO_FUEL;
				agent.path = Path.movesToPoint(agent.coords, w.getBestCell(CellType.PUMP, agent.coords));
				return Group.make2(null, agent.path.step());
			} else {
				return Group.make2(null, direction);
			}
			
		}
		return null;
	}
}
