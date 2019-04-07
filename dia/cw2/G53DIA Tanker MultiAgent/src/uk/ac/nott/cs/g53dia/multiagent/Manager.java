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
	private Pair<Integer, Integer> lastTStation;
	private HashMap<Integer, Agent> agents = new HashMap<>();
	
	public World w;
	
	
	public Manager(Random rand, int gridSize, int maxFuel) {
		r = rand;
		w = new World(gridSize, maxFuel);
	}
	
	public void register(Agent agent) {
		agents.put(agent.id, agent);
	}
	/**
	 * Tries to evaluate the best well to use for disposing of waste
	 * Takes the waste level and fuel level in to account when scoring wells
	 * @param agent
	 * @return <coords, canReach>
	 */
	public Pair<Pair<Integer, Integer>, Boolean> getBestWell(Agent agent) {
		Pair<Integer, Integer> selected = null;

		double cost = Double.MAX_VALUE;
		/*
		 * If we have more than half of the waste tank occupied,
		 * we are are less likely to be able to consume any more tasks
		 * 
		 * If fuel level is less the half the tank, we are likely to need a refuel soon
		 * so it's better to choose a well with a pump nearby
		 */
		double wasteMultiplier = agent.getWasteCapacity() > (Tanker.MAX_WASTE / 2) ? 4 : 2;
		double fuelMultiplier = agent.getFuelLevel() > (Tanker.MAX_FUEL / 2) ? 4 : 2;

		for (Pair<Integer, Integer> coords : w.wells) {
			// Check whether is reachable, otherwise we might get false paths
			if (!w.isReachable(agent.coords, coords, agent.getFuelLevel()).first) {
				continue;
			}
			Pair<Integer, Integer> pump = w.findClosestCell(CellType.PUMP, coords);
			Pair<Integer, Integer> station = w.findClosestCell(CellType.STATION, coords);
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
		/*
		 * If we didn't select a good well, we fallback to finding a closest one
		 * Even if it's not reachable, the tanker can continue by refuelling first
		 */
		return selected == null ? w.getWell(agent) : new Pair<>(selected, true);
	}
	
	/**
	 * Goes trough all of the stations at chooses one to roam towards
	 * Will not choose the same station twice
	 * Will try to move in the same direction as initiation direction
	 * If no station is chosen, it will use initiation direction to move to a point further away
	 * @param agent
	 * @return Group2<Integer, Integer>
	 */
	private Pair<Integer, Integer> getRoamTarget(Agent agent) {
		// Need a list here because we are accessing random indices
		ArrayList<Pair<Integer, Integer>> stations = new ArrayList<>();
		/*
		 * Remove unreachable and visited stations
		 */
		for (Pair<Integer, Integer> entry: w.getFreeStations()) {
			boolean notVisited = !entry.equals(agent.pTarget) && !entry.equals(agent.coords);
			if (notVisited && w.isReachable(agent.coords, entry, agent.getFuelLevel()).first) {
				// We prefer to follow initiationDirection to spread tankers around
				if (Path.bestMove(agent.coords, entry) == agent.initiationDirection) {
					w.reserve(entry, agent);
					agent.pTarget = agent.target;
					agent.target = entry;
					return entry;
				}
				// To save iteration, save the station for random access loop bellow
				stations.add(entry);
			}
		}
		
		// Fallback to choosing a random station
		while (stations.size() > 0) {
			int index = r.nextInt(stations.size() > 1 ? stations.size() - 1 : 1);
			Pair<Integer, Integer> coords = stations.get(index);
			w.reserve(coords, agent);
			agent.pTarget = agent.target;
			agent.target = coords;
			return coords;
		}
		/*
		 * Here we use initiation direction to spread tankers further,
		 * which helps to avoid competition over tasks
		 */
		int moves = 10;
		Pair<Integer, Integer> direction = Path.moveChange(agent.initiationDirection);
		Pair<Integer, Integer> coords = new Pair<>(
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
	 * 	Needs to have enough space for waste
	 * 	Should be able to reach specific point
	 * 	Can be in the following states: ROAMING, DISPOSING, REFUELING, CONSUMING
	 * Gets sorted by which one is the closest
	 * @param entry
	 * @return id of agent
	 */
	private int assingAgent(Pair<Integer, Integer> entry) {
		int price = Integer.MAX_VALUE;
		
		int id = -1;
		Task task = ((Station) w.stations.get(entry)).getTask();
		int waste = task.getWasteAmount();
		
		for (int i = 0; i < agents.size(); i += 1) {
			Agent agent = agents.get(i);
			State state = agent.state;

			// Agent should have enough capacity
			if (agent.getWasteCapacity() < waste ||
				!w.isReachable(agent.coords, entry, agent.getFuelLevel()).second) {
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
	 * 		Saves all stations assigned to passed agent and chooses the closest one
	 * @param agent
	 * @return <Coordinates, if has enough fuel>
	 */
	private Pair<Pair<Integer, Integer>, Boolean> checkForTasks(Agent agent) {
		HashSet<Pair<Integer, Integer>> stations = w.getTaskStations();
		
		if (stations.size() == 0) {
			return null;
		}
		
		HashSet<Pair<Integer, Integer>> myAssignments = new HashSet<>();
		
		
		for (Pair<Integer, Integer> entry: stations) {
			if (assingAgent(entry) == agent.id) {
				myAssignments.add(entry);
			}
		}

		// We have been assigned tasks, Find the closest station
		if (myAssignments.size() != 0) {
			int price = Integer.MAX_VALUE;
			Pair<Integer, Integer> coords = null;
			
			for(Pair<Integer, Integer> entry: myAssignments) {
				int myPrice = Path.distance(agent.coords, entry);
				if (myPrice < price) {
					price = myPrice;
					coords = entry;
				}
			}
			if (coords != null) {
				return new Pair<>(coords, w.isReachable(agent.coords, coords, agent.getFuelLevel()).first);
			}
		}
		
		return null;
	}
	
	/**
	 * Returns -1, when the agent can get a task
	 * Returns -2, when a refuel is needed
	 * Otherwise will return integer 1-8 for the direction agent should move to
	 * @param agent
	 * @return int
	 */
	private int roamAction(Agent agent) {
		// Get the coordinates and whether we can afford it
		Pair<Pair<Integer, Integer>, Boolean> result = checkForTasks(agent);
		if (result != null) {
			lastTStation = result.first;
			if (result.second) {
				// Reset targets
				agent.freeTargets();
				return -1;
			}
			return -2;
		}
		
		// Continue moving towards the target
		if (agent.toTarget != null && agent.toTarget.hasSteps()) {
			return agent.toTarget.step();
		// Once target is reached free it.
		} else if (w.reserved.contains(agent.target)) {
			w.free(agent.target, agent);
		}
		// Find a new roaming target
		Pair<Integer, Integer> target = getRoamTarget(agent);
		if (target != null) {
			agent.toTarget = Path.movesToPoint(agent.coords, target);
			return agent.toTarget.step();
		}
		return -2;
	}

	private Pair<Action, Integer> refuel(Agent agent) {
		agent.state = State.REFUELING;
		
		if (agent.getFuelLevel() == agent.MAX_FUEL) {
			Debug.error("Tank is full for " + agent.toString() + " at timestep " + agent.timestep);
		}
		
		return new Pair<>(new RefuelAction(), null);
	}
	
	private Pair<Action, Integer> consumeTask(Agent agent) {
		agent.state = State.CONSUMING;
		Cell cell = agent.myCell();
		Task task = ((Station) cell).getTask();
		return new Pair<>(new LoadWasteAction(task) , null);
	}
	
	private Pair<Action, Integer> moveToPump(Agent agent) {
		// Check whether we aren't just standing on re-fuelling spot already
		if (w.pumps.contains(agent.coords)) {
			return refuel(agent);
		}
		agent.state = State.MOVING_TO_FUEL;
		agent.path = Path.movesToPoint(agent.coords, w.getClosestPump(agent.coords));
		return new Pair<>(null, agent.path.step());
	}
	
	private Pair<Action, Integer> dispose(Agent agent) {
		agent.state = State.DISPOSING;
		return new Pair<>(new DisposeWasteAction(), null);
	}
	
	private Pair<Action, Integer> moveToWell(Agent agent) {
		if (w.reserved.contains(agent.coords)) {
			w.free(agent.coords, agent);
		}
		Pair<Pair<Integer, Integer>, Boolean> meta = getBestWell(agent);
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
		
		return new Pair<>(null, agent.path.step());
	}
	
	private Pair<Action, Integer> moveToStation(Agent agent, Pair<Integer, Integer> station) {
		// make sure no other tanks go for the same place
		w.reserve(station, agent);
		// Happens when a station generates a task, while we are standing on it
		if (agent.coords.equals(station)) {
			return consumeTask(agent);
		}
		
		agent.state = State.MOVING_TO_STATION;
		agent.path = Path.movesToPoint(agent.coords, station);
		return new Pair<>(null, agent.path.step());
	}
	
	private Pair<Action, Integer> roam(Agent agent) {
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
			return new Pair<>(null, direction);
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
	private Pair<Action, Integer> tryToPickupTask(Agent agent) {
		// Release just consumed task
		if (w.reserved.contains(agent.coords)) {
			w.free(agent.coords, agent);
		}
		
		// TODO: improve by passing the found well to moveToWell function
//		Group2<Group2<Integer, Integer>, Boolean> well = w.getWell(agent);
		Pair<Pair<Integer, Integer>, Boolean> well = getBestWell(agent);
		
		if (well.first == null) {
			// TODO: remove before release
			Debug.error("Failed to find a well for agent: " + agent.toString());
			return null;
		}
		Pair<Pair<Integer, Integer>, Boolean> station = checkForTasks(agent);
		
		// If no task was found or we can't reach it
		if (station == null || !station.second) {
			return moveToWell(agent);
		}
		
		int wellRatio = 2;
		int toWell = Path.distance(agent.coords, well.first);
		int toStation = Path.distance(agent.coords, station.first);
		
		if (toWell * wellRatio < toStation) {
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
	public Pair<Action, Integer> asignAction(Agent agent) {
		
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
			return dispose(agent);
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
