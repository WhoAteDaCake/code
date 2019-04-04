package uk.ac.nott.cs.g53dia.multiagent;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Random;

import uk.ac.nott.cs.g53dia.multilibrary.*;

public class Manager {
	private Random r;
	// For performance improvement
	private Group2<Integer, Integer> lastTStation;
	private HashMap<Integer, Agent> agents = new HashMap<>();
	
	public World w;
	
	
	public Manager(Random rand, int gridSize, int maxFuel) {
		r = rand;
		w = new World(gridSize, maxFuel);
	}
	
	public void register(Agent agent) {
		agents.put(agent.id, agent);
	}
	
	private Group2<Integer, Integer> getRoamTarget(Agent agent) {
		// Need a list here because we are accessing random indices
		ArrayList<Group2<Integer, Integer>> stations = new ArrayList<>(w.getFreeStations());
		
		while (stations.size() > 0) {
			int index = r.nextInt(stations.size() > 1 ? stations.size() - 1 : 1);
			Group2<Integer, Integer> coords = stations.get(index);
			boolean notVisited = !coords.equals(agent.pTarget) && !coords.equals(agent.coords);
			if (notVisited && w.isReachable(agent.coords, coords, agent.getFuelLevel()).first) {
				w.reserve(coords, agent);
				agent.pTarget = agent.target;
				agent.target = coords;
				return coords;
			} else {
				stations.remove(coords);
			}
		}

		int modifier = r.nextInt(10) > 5 ? -20 : 20;
		Group2<Integer, Integer> coords = new Group2<>(
				agent.coords.first + modifier,
				agent.coords.second + modifier);
		if (w.isReachable(agent.coords, coords, agent.getFuelLevel()).first) {
			return coords;
		}
		return null;
	}
	
	// Find the best agent to consume a specific task
	// returns <id of agent, price to get there>
	private Group2<Integer, Integer> assingAgent(Group2<Integer, Integer> entry) {
		int price = Integer.MAX_VALUE;
		int id = -1;
		Task task = ((Station) w.stations.get(entry)).getTask();
		int waste = task.getWasteAmount();
		
		/*
		 * Criteria:
		 * 	Needs to have enough space
		 * 	Can be in the following states: ROAMING, DISPOSING, REFUELING
		 * Gets sorted by which one is the closest
		 */
		for (int i = 0; i < agents.size(); i += 1) {
			Agent agent = agents.get(i);
			State state = agent.state;
			if (!(state == State.ROAMING || state == State.DISPOSING || state == State.REFUELING)) {
				continue;
			}
			if (agent.getWasteCapacity() < waste && state != State.DISPOSING) {
				continue;
			}
			if (!(w.isReachable(entry, agent.coords, agent.getFuelLevel())).first) {
				continue;
			}
			int myPrice = Path.distance(entry, agent.coords);
			if (myPrice < price) {
				price = myPrice;
				id = i;
			}
		}
		
		return new Group2<>(id, price);
	}
	
	// Checks whether there are any tasks that the agent is best for
	private Group2<Group2<Integer, Integer>, Boolean> checkForTasks(Agent agent) {
		int price = Integer.MAX_VALUE;
		Group2<Integer, Integer> coords = null;
		
		for (Group2<Integer, Integer> entry: w.getTaskStations()) {
			Group2<Integer, Integer> result = assingAgent(entry);
			if (result.first != agent.id) {
				continue;
			}
			if (result.second < price) {
				price = result.second;
				coords = entry;
			}
			
		}
		return coords == null ? null : new Group2<>(coords, price * 2 < agent.getFuelLevel());
	}
	
	// Returns -1, when the agent can do better than roaming
	// Returns -2, when a refuel is needed
	private int roam(Agent agent) {
		// Get the coordinates and whether we can afford it
//		Group2<Group2<Integer, Integer>, Boolean> result = w.getNearestTaskStation(agent);
		Group2<Group2<Integer, Integer>, Boolean> result = checkForTasks(agent);
		if (result != null) {
			lastTStation = result.first;
			if (result.second) {
				// Reset targets
				agent.freeTargets();
				return -1;
			}
			return -2;
		}
		
		// Target section
		if (agent.toTarget != null && agent.toTarget.hasSteps()) {
			return agent.toTarget.step();
		} else if (w.reserved.contains(agent.target)) {
			w.free(agent.target, agent);
		}
		
		Group2<Integer, Integer> target = getRoamTarget(agent);
		if (target != null) {
			agent.toTarget = Path.movesToPoint(agent.coords, target);
			return agent.toTarget.step();
		}
		return -2;
	}

	private Group2<Action, Integer> refuel(Agent agent) {
		agent.state = State.REFUELING;
		return new Group2<>(new RefuelAction(), null);
	}
	
	private Group2<Action, Integer> consumeTask(Agent agent, Cell[][] view) {
		agent.state = State.CONSUMING;
		Cell cell = agent.getCurrentCell(view);
		Task task = ((Station) cell).getTask();
		return new Group2<>(new LoadWasteAction(task) , null);
	}
	
	private Group2<Action, Integer> moveToPump(Agent agent) {
		// Check whether we aren't just standing on re-fuelling spot already
		if (w.pumps.contains(agent.coords)) {
			return refuel(agent);
		}
		agent.state = State.MOVING_TO_FUEL;
		agent.path = Path.movesToPoint(agent.coords, w.getBestCell(CellType.PUMP, agent.coords));
		return new Group2<>(null, agent.path.step());
	}
	
	// Returns action or direction that the tanker should perform
	public Group2<Action, Integer> asignAction(Agent agent, Cell[][] view) {
		
		if (agent.state == State.ROAMING) {
			int direction = roam(agent);
			if (direction == -1) {
				agent.state = State.MOVING_TO_STATION;
				agent.path = Path.movesToPoint(agent.coords, lastTStation);
				// new Group2<> sure no other tanks go for the same place
				w.reserve(lastTStation, agent);
				// Happens when a station generates a task, while we are standing on it
				// To re-use code we just re-run action assignment again
				if (agent.path.stepCount() == 0) {
					return consumeTask(agent, view);
				}
				return new Group2<>(null, agent.path.step());
			} else if (direction == -2) {
				// Should recalculate targets after refuelling
				agent.freeTargets();
				return moveToPump(agent);
			} else {
				return new Group2<>(null, direction);
			}
		// Can assume, that at this point agent is standing on a station
		} else if (agent.state == State.MOVING_TO_STATION) {
			return consumeTask(agent, view);
		// Move to the well
		} else if (agent.state == State.CONSUMING) {
			// No longer reserved
			w.free(agent.coords, agent);
			Group2<Group2<Integer, Integer>, Boolean> meta = w.getWell(agent);
			// Not enough fuel
			if (!meta.second) {
				return moveToPump(agent);
			}
			agent.state = State.MOVING_TO_WELL;
			agent.path = Path.movesToPoint(agent.coords, meta.first);
			return new Group2<>(null, agent.path.step());
		// Assume already at the well
		} else if (agent.state == State.MOVING_TO_WELL) {
			agent.state = State.DISPOSING;
			return new Group2<>(new DisposeWasteAction(), null);
		} else if (agent.state == State.DISPOSING) {
			agent.state = State.ROAMING;
			return asignAction(agent, view);
		// Assume already at the pump
		} else if (agent.state == State.MOVING_TO_FUEL) {
			return refuel(agent);
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
