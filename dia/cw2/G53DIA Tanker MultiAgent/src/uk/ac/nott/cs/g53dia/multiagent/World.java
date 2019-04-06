package uk.ac.nott.cs.g53dia.multiagent;


import java.util.HashMap;
import java.util.HashSet;

import uk.ac.nott.cs.g53dia.multilibrary.Cell;
import uk.ac.nott.cs.g53dia.multilibrary.Station;
import uk.ac.nott.cs.g53dia.multilibrary.Task;

public class World {
	public HashSet<Group2<Integer, Integer>> pumps = new HashSet<>();
	public HashSet<Group2<Integer, Integer>> wells = new HashSet<>();
	public HashMap<Group2<Integer, Integer>, Cell> stations = new HashMap<>();
	
	// Stations that have a tanker moving towards
	public HashSet<Group2<Integer, Integer>> reserved = new HashSet<>();
	public HashSet<Group2<Integer, Integer>> unreachable = new HashSet<>();
	
	private int cx;
	private int cy;
	private int mFuel;
	
	public World(int size, int maxFuel) {
		cx = cy = size;
		mFuel = maxFuel;
		// Initial pump location
		pumps.add(new Group2<>(0, 0));
	}
	
	
	public HashSet<Group2<Integer, Integer>> getTaskStations() {
		HashSet<Group2<Integer, Integer>> keys = new HashSet<>();
		for (Group2<Integer, Integer> entry: stations.keySet()) {
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
	
	public Group2<Group2<Integer, Integer>, Boolean> getNearestTaskStation(Agent agent) {
		HashSet<Group2<Integer, Integer>> keys = new HashSet<>(stations.keySet());
		for (Group2<Integer, Integer> entry: reserved) {
			keys.remove(entry);
		}
		
		
		int price = Integer.MAX_VALUE;
		Group2<Integer, Integer> coords = null;
		boolean isReachable = false;
		
		for (Group2<Integer, Integer> entry: keys) {
			Station station = (Station) stations.get(entry);
			// Make sure it has a task and a tanker is able to visit it at some point
			Group2<Boolean, Boolean> reachability = isReachable(agent.coords, entry, agent.getFuelLevel());
			Task task = station.getTask();
			if (task == null || task.getWasteAmount() > agent.getWasteCapacity() ||!reachability.second) {
				continue;
			}
			int myPrice = Path.distance(agent.coords, entry);
			if (myPrice < price) {
				coords = entry;
				price = myPrice;
				isReachable = reachability.first;
			}
		}
		return coords == null ? null : new Group2<>(coords, isReachable);
	}
	
	public Group2<Group2<Integer, Integer>, Boolean> getWell(Agent agent) {
		int price = Integer.MAX_VALUE;
		Group2<Integer, Integer> coords = null;
		boolean isReachable = false;
		
		for (Group2<Integer, Integer> entry: wells) {
			Group2<Boolean, Boolean> reachability = isReachable(agent.coords, entry, agent.getFuelLevel());
			int myPrice = Path.distance(agent.coords, entry);
			if (myPrice < price) {
				coords = entry;
				price = myPrice;
				isReachable = reachability.first;
			}
		}
		return new Group2<>(coords, isReachable);
	}
	
	public Group2<Integer, Integer> getBestCell(CellType type, Group2<Integer, Integer> from) {
		if (type == CellType.PUMP) {
			// Initialise to first pump by default
			Group2<Integer, Integer> coords = new Group2<>(0, 0);
			int price = Path.distance(from, coords);
			
			for (Group2<Integer, Integer> entry: pumps) {
				int myPrice = Path.distance(from, entry);
				if (myPrice < price) {
					price = myPrice;
					coords = entry;
				}
			}
			return coords;
			
		}
		return null;
	}
	
	// Some point's can't be reached due to limit on max fuel
	// This will return <canReach, possibleToReach>
	public Group2<Boolean, Boolean> isReachable(Group2<Integer, Integer> from,
			Group2<Integer, Integer> coords, int fuelLevel) {
		// Each move costs 2 fuel and car should be able to go there and back
		int pathPrice = Path.distance(from, coords);
		// From path, walk to pump
		Group2<Integer, Integer> pump = getBestCell(CellType.PUMP, coords);
		// Make sure we can reach a pump after said point
		int toPumpPrice = Path.distance(coords, pump);
		int fullPrice = (pathPrice + toPumpPrice) * 2;
		boolean canReach = fullPrice < fuelLevel;
		boolean reachable = canReach;
		
		// Check if it's possible to reach a nearby pump and then reach said station
		if (!reachable) {
			Group2<Integer, Integer> nearPositionPump = getBestCell(CellType.PUMP, from);
			int toNearPump = Path.distance(from, nearPositionPump) * 2;
			if (toNearPump < fuelLevel) {
				// At this point we know that we can reach a nearby pump
				int pumpToCoord = Path.distance(nearPositionPump, coords);
				reachable = (pumpToCoord + toPumpPrice) * 2 < mFuel;
			}
		}
		
		return new Group2<>(canReach, reachable);
	}
	
	// Will get all the stations that are not reserved
	public HashSet<Group2<Integer, Integer>> getFreeStations() {
		HashSet<Group2<Integer, Integer>> keys = new HashSet<>(stations.keySet());
		for (Group2<Integer, Integer> entry: reserved) {
			keys.remove(entry);
		}
		return keys;
	}
	
	public void saveGridCell(Group2<Integer, Integer> tCoords, Cell cell, CellType type, int row, int col) {
		// Change from center
		int xC = cx - row;
		int yC = cy - col;
		
		Group2<Integer, Integer> coords = new Group2<>(tCoords.first + xC, tCoords.second + yC);
		if (type == CellType.PUMP) {
			pumps.add(coords);
		} else if (type == CellType.STATION) {
			stations.put(coords, cell);
		} else {
			wells.add(coords);
		}
	}
	
	public void setUnreachable(Group2<Integer, Integer> coords) {
		unreachable.add(coords);
	}
	
	// Reserve a station, so other wells do not move towards it
	public void reserve(Group2<Integer, Integer> coords, Agent agent) {
		Debug.log(String.format("Reserving station %s by: %s at timestep %d", coords.toString(), agent.toString(), agent.timestep));
		reserved.add(coords);
	}
	
	// Free reservation up
	public void free(Group2<Integer, Integer> coords, Agent agent) {
		Debug.log(String.format("Freeing station %s by: %s at timestep %d", coords.toString(), agent.toString(), agent.timestep));
		reserved.remove(coords);
	}
}
