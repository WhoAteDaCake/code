package uk.ac.nott.cs.g53dia.multiagent;

import java.util.HashMap;
import java.util.HashSet;

import uk.ac.nott.cs.g53dia.multilibrary.Cell;
import uk.ac.nott.cs.g53dia.multilibrary.Station;
import uk.ac.nott.cs.g53dia.multilibrary.Task;

public class World {
	public HashSet<Pair<Integer, Integer>> pumps = new HashSet<>();
	public HashSet<Pair<Integer, Integer>> wells = new HashSet<>();
	public HashMap<Pair<Integer, Integer>, Cell> stations = new HashMap<>();

	// Stations that have a tanker moving towards
	public HashSet<Pair<Integer, Integer>> reserved = new HashSet<>();
	public HashSet<Pair<Integer, Integer>> unreachable = new HashSet<>();

	private int cx;
	private int cy;
	private int mFuel;

	public World(int size, int maxFuel) {
		cx = cy = size;
		mFuel = maxFuel;
		// Initial pump location
		pumps.add(new Pair<>(0, 0));
	}

	public HashSet<Pair<Integer, Integer>> getTaskStations() {
		HashSet<Pair<Integer, Integer>> keys = new HashSet<>();
		for (Pair<Integer, Integer> entry : stations.keySet()) {
			if (reserved.contains(entry)) {
				continue;
			}
			Station station = (Station) stations.get(entry);
			if (station.getTask() != null) {
				keys.add(entry);
			}
		}
		return keys;
	}

	/*
	 * Will return the coordinates of the closest entity from given coordinates
	 */
	public Pair<Integer, Integer> findClosestCell(CellType type, Pair<Integer, Integer> from) {
		Pair<Integer, Integer> selected = null;
		int distance = Integer.MAX_VALUE;

		HashSet<Pair<Integer, Integer>> items = new HashSet<>();
		
		if (type == CellType.PUMP) {
			items = pumps;
		} else if (type == CellType.STATION) {
//			items = new HashSet<>(stations.keySet());
			items = getTaskStations();
		} else {
			items = wells;
		}
		
		for (Pair<Integer, Integer> coords : items) {
			// We should allow to refuel, while standing on a cell
			int newDist = Path.distance(from, coords);
			if (distance > newDist) {
				selected = coords;
				distance = newDist;
			}
		}
		return selected;
	}

	public Pair<Pair<Integer, Integer>, Boolean> getNearestTaskStation(Agent agent) {
		HashSet<Pair<Integer, Integer>> keys = new HashSet<>(stations.keySet());
		for (Pair<Integer, Integer> entry : reserved) {
			keys.remove(entry);
		}

		int price = Integer.MAX_VALUE;
		Pair<Integer, Integer> coords = null;
		boolean isReachable = false;

		for (Pair<Integer, Integer> entry : keys) {
			Station station = (Station) stations.get(entry);
			// Make sure it has a task and a tanker is able to visit it at some point
			Pair<Boolean, Boolean> reachability = isReachable(agent.coords, entry, agent.getFuelLevel());
			Task task = station.getTask();
			if (task == null || task.getWasteAmount() > agent.getWasteCapacity() || !reachability.second) {
				continue;
			}
			int myPrice = Path.distance(agent.coords, entry);
			if (myPrice < price) {
				coords = entry;
				price = myPrice;
				isReachable = reachability.first;
			}
		}
		return coords == null ? null : new Pair<>(coords, isReachable);
	}

	public Pair<Pair<Integer, Integer>, Boolean> getWell(Agent agent) {
		int price = Integer.MAX_VALUE;
		Pair<Integer, Integer> coords = null;
		boolean isReachable = false;

		for (Pair<Integer, Integer> entry : wells) {
			Pair<Boolean, Boolean> reachability = isReachable(agent.coords, entry, agent.getFuelLevel());
			int myPrice = Path.distance(agent.coords, entry);
			if (myPrice < price) {
				coords = entry;
				price = myPrice;
				isReachable = reachability.first;
			}
		}
		return new Pair<>(coords, isReachable);
	}

	public Pair<Integer, Integer> getClosestPump(Pair<Integer, Integer> from) {
		// Initialise to first pump by default
		Pair<Integer, Integer> coords = new Pair<>(0, 0);
		int price = Path.distance(from, coords);

		for (Pair<Integer, Integer> entry : pumps) {
			int myPrice = Path.distance(from, entry);
			if (myPrice < price) {
				price = myPrice;
				coords = entry;
			}
		}
		return coords;
	}

	// Some point's can't be reached due to limit on max fuel
	// This will return <canReach, possibleToReach>
	public Pair<Boolean, Boolean> isReachable(Pair<Integer, Integer> from, Pair<Integer, Integer> coords,
			int fuelLevel) {
		// Each move costs 2 fuel and car should be able to go there and back
		int pathPrice = Path.distance(from, coords);
		// From path, walk to pump
		Pair<Integer, Integer> pump = getClosestPump(coords);
		// Make sure we can reach a pump after said point
		int toPumpPrice = Path.distance(coords, pump);
		int fullPrice = (pathPrice + toPumpPrice) * 2;
		boolean canReach = fullPrice < fuelLevel;
		boolean reachable = canReach;

		// Check if it's possible to reach a nearby pump first and then reach the other coordinate
		if (!reachable) {
			Pair<Integer, Integer> nearPositionPump = getClosestPump(from);
			int toNearPump = Path.distance(from, nearPositionPump) * 2;
			if (toNearPump < fuelLevel) {
				// At this point we know that we can reach a nearby pump
				int pumpToCoord = Path.distance(nearPositionPump, coords);
				reachable = (pumpToCoord + toPumpPrice) * 2 < mFuel;
			}
		}

		return new Pair<>(canReach, reachable);
	}

	// Will get all the stations that are not reserved
	public HashSet<Pair<Integer, Integer>> getFreeStations() {
		HashSet<Pair<Integer, Integer>> keys = new HashSet<>(stations.keySet());
		for (Pair<Integer, Integer> entry : reserved) {
			keys.remove(entry);
		}
		return keys;
	}

	public void saveGridCell(Pair<Integer, Integer> tCoords, Cell cell, CellType type, int row, int col) {
		// Check for change from centre coordinate
		int xC = cx - row;
		int yC = cy - col;

		Pair<Integer, Integer> coords = new Pair<>(tCoords.first + xC, tCoords.second + yC);
		if (type == CellType.PUMP) {
			pumps.add(coords);
		} else if (type == CellType.STATION) {
			stations.put(coords, cell);
		} else {
			wells.add(coords);
		}
	}

	public void setUnreachable(Pair<Integer, Integer> coords) {
		unreachable.add(coords);
	}

	// Reserve a station, so other wells do not move towards it
	public void reserve(Pair<Integer, Integer> coords, Agent agent) {
		Debug.log(String.format("Reserving station %s by: %s at timestep %d", coords.toString(), agent.toString(),
				agent.timestep));
		reserved.add(coords);
	}

	// Free reservation up
	public void free(Pair<Integer, Integer> coords, Agent agent) {
		Debug.log(String.format("Freeing station %s by: %s at timestep %d", coords.toString(), agent.toString(),
				agent.timestep));
		reserved.remove(coords);
	}
}
