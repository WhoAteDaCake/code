package uk.ac.nott.cs.g53dia.multiagent;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Random;
import uk.ac.nott.cs.g53dia.multiagent.Path;

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
	
	public Group2<Group2<Integer, Integer>, Boolean> getBestWell(Agent agent) {
		Group2<Integer, Integer> selected = null;

		double cost = Double.MAX_VALUE;
		double wasteMultiplier = agent.getWasteCapacity() > (Tanker.MAX_WASTE / 2) ? 4 : 2;
		double fuelMultiplier = agent.getFuelLevel() > (Tanker.MAX_FUEL / 2) ? 4 : 2;
		boolean canAfford = false;

		for (Group2<Integer, Integer> coords : w.wells) {
			// Check whether is reachable, otherwise we might get false paths
			if (!w.isReachable(agent.coords, coords, agent.getFuelLevel()).first) {
				continue;
			}
			Group2<Integer, Integer> pump = w.findClosestCell(CellType.PUMP, coords);
			Group2<Integer, Integer> station = w.findClosestCell(CellType.STATION, coords);
			double pathCost = Path.distance(agent.coords, coords);
			double pumpCost = Path.distance(coords, pump) / fuelMultiplier;
			double stationCost = pumpCost * 2;
			if (station != null) {
				stationCost = Path.distance(coords, station) / wasteMultiplier;
			}
			double myCost = pathCost + pumpCost + stationCost;
			if (cost > myCost) {
				selected = coords;
				cost = myCost;
			}
		}
		return selected == null ? w.getWell(agent) : new Group2(selected, true);
	}
	
	/**
	 * Goes trough all of the stations at chooses a random one to roam towards
	 * Will not choose the same station twice
	 * If no station is chosen, it will use initiation direction to move to a point further away
	 * @param agent
	 * @return Group2<Integer, Integer>
	 */
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
		/*
		 * Here we use initiation direction to move to a random point
		 * Using a specific direction helps to spread tankers around
		 * Which helps to avoid competition over tasks
		 */
		int moves = 10;
		Group2<Integer, Integer> direction = Path.moveChange(agent.initiationDirection);
		Group2<Integer, Integer> coords = new Group2<>(
				agent.coords.first + (direction.first * moves),
				agent.coords.second + (direction.second * moves));
		if (w.isReachable(agent.coords, coords, agent.getFuelLevel()).first) {
			return coords;
		}
		return null;
	}
	
	/**
	 * Finds a best agent for a specific station with a task
	 * Criteria:
	 * 	Needs to have enough space
	 * 	Can be in the following states: ROAMING, DISPOSING, REFUELING, CONSUMING
	 * Gets sorted by which one is the closest
	 * @param entry
	 * @return id of agent
	 */
	private int assingAgent(Group2<Integer, Integer> entry) {
		int price = Integer.MAX_VALUE;
		
		int id = -1;
		Task task = ((Station) w.stations.get(entry)).getTask();
		int waste = task.getWasteAmount();
		
		for (int i = 0; i < agents.size(); i += 1) {
			Agent agent = agents.get(i);
			State state = agent.state;
			if (!(
					state == State.ROAMING ||
					state == State.DISPOSING ||
					state == State.REFUELING ||
					state == State.CONSUMING)) {
				continue;
			}
			if (agent.getWasteCapacity() < waste && state != State.DISPOSING) {
				continue;
			}
			if (!w.isReachable(agent.coords, entry, agent.getFuelLevel()).second) {
				continue;
			}
			int myPrice = Path.distance(entry, agent.coords);
			if (myPrice < price) {
				price = myPrice;
				id = i;
			}
		}
		
		return id;
	}
	
	/**
	 * Tries to find the best station for an agent
	 * Works by:
	 * 		Assigns all stations to agents (using assingAgent)
	 * 		Checks the list if given agent was assigned anything
	 * 		If not, checks whether some agents have been assigned multiple tasks
	 * 		And takes one of those tasks
	 * @param agent
	 * @return <Coordinates, whether it's reachable with current fuel>
	 */
	private Group2<Group2<Integer, Integer>, Boolean> checkForTasks(Agent agent) {
//		return w.getNearestTaskStation(agent);
		HashSet<Group2<Integer, Integer>> stations = w.getTaskStations();
		HashMap<Integer, HashSet<Group2<Integer, Integer>>> assignments = new HashMap<>();
		if (stations.size() == 0) {
			return null;
		}
		
		for (Group2<Integer, Integer> entry: stations) {
			int id = assingAgent(entry);
			if (id == -1) {
				Debug.warn(String.format("Got -1 for entry: %s agent : %s", entry.toString(), agent.toString()));
				continue;
			}
			if (assignments.containsKey(id)) {
				assignments.get(id).add(entry);
			} else {
				HashSet<Group2<Integer, Integer>> coords = new HashSet<>();
				coords.add(entry);
				assignments.put(id, coords);
			}
		}
		
		HashSet<Group2<Integer, Integer>> myAssignments = assignments.get(agent.id);
		// NOTE: after doing some runs, it doesn't seem to increase perf
		// Check other station assignments
		// and take tasks from agents that have more than one
//		if (myAssignments == null) {
//			myAssignments = new HashSet<>();
//			for(int i = 0; i < agents.size(); i += 1) {
//				HashSet<Group2<Integer, Integer>> items = assignments.get(i);
//				if (i == agent.id || items == null) {
//					continue;
//				}
//				if (items.size() > 1) {
//					myAssignments.addAll(items);
//				}
//			}
//		}
//		
		// We have been assigned tasks, Find the closest station
		if (myAssignments != null && myAssignments.size() != 0) {
			int price = Integer.MAX_VALUE;
			boolean isReachable = false;
			Group2<Integer, Integer> coords = null;
			
			for(Group2<Integer, Integer> entry: myAssignments) {
				// Skip stations that can't be reached
				Group2<Boolean, Boolean> result = w.isReachable(agent.coords, entry, agent.getFuelLevel());
				Task task = ((Station)w.stations.get(entry)).getTask();
				// When tasks were assigned, waste was checked for single agent only
				if (!result.second || task.getWasteAmount() > agent.getWasteCapacity()) {
					continue;
				}
				
				int myPrice = Path.distance(agent.coords, entry);
				if (myPrice < price) {
					price = myPrice;
					coords = entry;
					isReachable = result.first;
				}
			}
			if (coords != null) {
				return new Group2<>(coords, isReachable);
			}
		}
		
		return null;
	}
	
	/**
	 * Returns -1, when the agent can do better than roaming
	 * Returns -2, when a refuel is needed
	 * Otherwise will return integer 1-8 for the direction agent should move to
	 * @param agent
	 * @return int
	 */
	private int roamAction(Agent agent) {
		// Get the coordinates and whether we can afford it
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
		
		if (agent.getFuelLevel() == agent.MAX_FUEL) {
			Debug.error("Tank is full for " + agent.toString() + " at timestep " + agent.timestep);
		}
		
		return new Group2<>(new RefuelAction(), null);
	}
	
	private Group2<Action, Integer> consumeTask(Agent agent) {
		agent.state = State.CONSUMING;
		Cell cell = agent.myCell();
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
	
	private Group2<Action, Integer> dispose(Agent agent) {
		agent.state = State.DISPOSING;
		return new Group2<>(new DisposeWasteAction(), null);
	}
	
	private Group2<Action, Integer> moveToWell(Agent agent) {
		if (w.reserved.contains(agent.coords)) {
			w.free(agent.coords, agent);
		}
		Group2<Group2<Integer, Integer>, Boolean> meta = getBestWell(agent);
//		Group2<Group2<Integer, Integer>, Boolean> meta = w.getWell(agent);
		// Not enough fuel
		if (!meta.second) {
			return moveToPump(agent);
		}
		if (agent.coords.equals(meta.first)) {
			return dispose(agent);
		}
		
		agent.state = State.MOVING_TO_WELL;
		agent.path = Path.movesToPoint(agent.coords, meta.first);
		
		return new Group2<>(null, agent.path.step());
	}
	
	private Group2<Action, Integer> moveToStation(Agent agent, Group2<Integer, Integer> station) {
		// make sure no other tanks go for the same place
		w.reserve(station, agent);
		// Happens when a station generates a task, while we are standing on it
		if (agent.coords.equals(station)) {
			return consumeTask(agent);
		}
		
		agent.state = State.MOVING_TO_STATION;
		agent.path = Path.movesToPoint(agent.coords, station);
		return new Group2<>(null, agent.path.step());
	}
	
	private Group2<Action, Integer> roam(Agent agent) {
		// Setting state now makes it convenient for future iterations
		agent.state = State.ROAMING;
		int direction = roamAction(agent);
		if (direction == -1) {
			return moveToStation(agent, lastTStation);
		} else if (direction == -2) {
			// Should recalculate targets after refuelling
			agent.freeTargets();
			return moveToPump(agent);
		} else {
			return new Group2<>(null, direction);
		}
	}
	
	/**
	 * The function should be called after consuming a task
	 * It will try to find any other tasks nearby
	 * And evaluate whether it's worth it to pick it up
	 * If not, it will go to nearest well and drop off the waste
	 * @param agent
	 * @return Group2<Action, Integer>
	 */
	private Group2<Action, Integer> tryToPickupTask(Agent agent) {
		// Release just consumed task
		if (w.reserved.contains(agent.coords)) {
			w.free(agent.coords, agent);
		}
		
		// TODO: improve by passing the found well to moveToWell function
//		Group2<Group2<Integer, Integer>, Boolean> well = w.getWell(agent);
		Group2<Group2<Integer, Integer>, Boolean> well = getBestWell(agent);
		
		if (well.first == null) {
			// TODO: remove before release
			Debug.error("Failed to find a well for agent: " + agent.toString());
			return null;
		}
		Group2<Group2<Integer, Integer>, Boolean> station = checkForTasks(agent);
		
		// If no task was found or we can't reach it
		if (station == null || !station.second) {
			return moveToWell(agent);
		}
		
		int wellRatio = 2;
		int toWell = Path.distance(agent.coords, well.first);
		int toStation = Path.distance(agent.coords, station.first);
		
		if (toWell * 2 < toStation) {
			return moveToWell(agent);
		}
		
		return moveToStation(agent, station.first);
	}
	
	/**
	 * Returns an action or direction that an agent should perform
	 * If action is null, agent should perform a move action using supplied direction
	 * All MOVINT_TO_* stations assume that at this point the agent is at the target location
	 * @param agent
	 * @return Group2<Action, direction>
	 */
	public Group2<Action, Integer> asignAction(Agent agent) {
		
		if (agent.state == State.ROAMING) {
			return roam(agent);
		// Can assume, that at this point agent is standing on a station
		} else if (agent.state == State.MOVING_TO_STATION) {
			return consumeTask(agent);
		// Move to the well
		} else if (agent.state == State.CONSUMING) {
//			return moveToWell(agent);
			return tryToPickupTask(agent);
		} else if (agent.state == State.MOVING_TO_WELL) {
			agent.state = State.DISPOSING;
			return new Group2<>(new DisposeWasteAction(), null);
		} else if (agent.state == State.DISPOSING) {
			return roam(agent);
		} else if (agent.state == State.MOVING_TO_FUEL) {
			return refuel(agent);
		} else if (agent.state == State.REFUELING) {
			if (agent.getWasteLevel() != 0) {
				return tryToPickupTask(agent);
			} else {
				return roam(agent);
			}
		}
			
		return null;
	}
}
