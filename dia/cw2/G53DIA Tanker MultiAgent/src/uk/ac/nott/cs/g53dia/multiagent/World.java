package uk.ac.nott.cs.g53dia.multiagent;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;

import uk.ac.nott.cs.g53dia.multilibrary.Cell;
import uk.ac.nott.cs.g53dia.multilibrary.Station;

public class World {
	private HashSet<Group.Group2<Integer, Integer>> pumps = new HashSet<>();
	private HashSet<Group.Group2<Integer, Integer>> wells = new HashSet<>();
	private HashMap<Group.Group2<Integer, Integer>, Cell> stations = new HashMap<>();
	
	// Stations that have a tanker moving towards
	private HashSet<Group.Group2<Integer, Integer>> reserved = new HashSet<>();
	private HashSet<Group.Group2<Integer, Integer>> unreachable = new HashSet<>();
	
	private int cx;
	private int cy;
	private int mFuel;
	
	public World(int size, int maxFuel) {
		cx = cy = size /2;
		mFuel = maxFuel;
		// Initial pump location
		pumps.add(Group.make2(0, 0));
	}
	
	
	public Group.Group2<Group.Group2<Integer, Integer>, Boolean> getNearestTaskStation(Agent agent) {
		HashSet<Group.Group2<Integer, Integer>> keys = new HashSet<>(stations.keySet());
		int price = Integer.MAX_VALUE;
		Group.Group2<Integer, Integer> coords = null;
		boolean isReachable = false;
		
		for (Group.Group2<Integer, Integer> entry: keys) {
			Station station = (Station) stations.get(entry);
			// Make sure it has a task and a tanker is able to visit it at some point
			Group.Group2<Boolean, Boolean> reachability = isReachable(agent.coords, entry, agent.getFuelLevel());
			if (station.getTask() == null || !reachability.second) {
				continue;
			}
			int myPrice = Path.distance(agent.coords, entry);
			if (myPrice < price) {
				coords = entry;
				price = myPrice;
				isReachable = reachability.first;
			}
		}
		return coords == null ? null : Group.make2(coords, isReachable);
	}
	
	public Group.Group2<Integer, Integer> getBestCell(CellType type, Group.Group2<Integer, Integer> from) {
		if (type == CellType.PUMP) {
			// Initialise to first pump by default
			Group.Group2<Integer, Integer> coords = Group.make2(0, 0);
			int price = Path.distance(from, coords);
			
			for (Group.Group2<Integer, Integer> entry: pumps) {
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
	public Group.Group2<Boolean, Boolean> isReachable(Group.Group2<Integer, Integer> from,
			Group.Group2<Integer, Integer> coords, int fuelLevel) {
		// Each move costs 2 fuel and car should be able to go there and back
		int pathPrice = Path.distance(from, coords);
		// From path, walk to pump
		Group.Group2<Integer, Integer> pump = getBestCell(CellType.PUMP, coords);
		// Make sure we can reach a pump after said point
		int toPumpPrice = Path.distance(coords, pump);
		int fullPrice = (pathPrice + toPumpPrice) * 2;
		return Group.make2(fullPrice < fuelLevel, fullPrice < mFuel);
	}
	
	// Will get all the stations that are not reserved
	public HashSet<Group.Group2<Integer, Integer>> getFreeStations() {
		HashSet<Group.Group2<Integer, Integer>> keys = new HashSet<>(stations.keySet());
		for (Group.Group2<Integer, Integer> entry: reserved) {
			keys.remove(entry);
		}
		return keys;
	}
	
	public void saveGridCell(Group.Group2<Integer, Integer> tCoords, Cell cell, CellType type, int row, int col) {
		// Change from center
		int xC = cx - col;
		int yC = cy - row;
		
		Group.Group2<Integer, Integer> coords = Group.make2(tCoords.first + xC, tCoords.second + yC);
		if (type == CellType.PUMP) {
			pumps.add(coords);
		} else if (type == CellType.STATION) {
			stations.put(coords, cell);
		} else {
			wells.add(coords);
		}
	}
	
	public void setUnreachable(Group.Group2<Integer, Integer> coords) {
		unreachable.add(coords);
	}
	
	// Reserve a station, so other wells do not move towards it
	public void reserve(Group.Group2<Integer, Integer> coords) {
		reserved.add(coords);
	}
	
	// Free reservation up
	public void free(Group.Group2<Integer, Integer> coords) {
		reserved.remove(coords);
	}
}
