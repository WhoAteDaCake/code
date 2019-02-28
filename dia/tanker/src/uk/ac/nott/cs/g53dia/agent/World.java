package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;

import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.FuelPump;
import uk.ac.nott.cs.g53dia.library.Station;
import uk.ac.nott.cs.g53dia.library.Tanker;
import uk.ac.nott.cs.g53dia.library.Task;
import uk.ac.nott.cs.g53dia.library.Well;

// TODO reset should periodically clean up moves.
public class World {
	@Deprecated
	private HashMap<Group.Group2<Integer, Integer>, Cell> cells = new HashMap<>();

	private HashMap<Group.Group2<Integer, Integer>, Cell> pumps = new HashMap<>();
	private HashMap<Group.Group2<Integer, Integer>, Cell> wells = new HashMap<>();
	private HashMap<Group.Group2<Integer, Integer>, Cell> stations = new HashMap<>();

	private ArrayList<Group.Group2<Integer, Integer>> unreachable = new ArrayList<>();

	IntelligentTanker tanker;

	// Centre coordinates of the grid view
	private int cx;
	private int cy;

	public int tankerX = 0;
	public int tankerY = 0;

	public World(int size, IntelligentTanker t) {
		// Because indices start at 0
		cy = cx = size;
		tanker = t;
	}

	public Group.Group2<Integer, Integer> viewIndicesToCoords(int row, int col) {
		return Group.make2(cx - col, cy - row);
	}

	public int distanceTo(Group.Group2<Integer, Integer> g) {
		return Path.distance(tankerX, tankerY, g.first, g.second);
	}

	public boolean standingOn(Group.Group2<Integer, Integer> coords) {
		return coords.first == tankerX && coords.second == tankerY;
	}

	public boolean hasSeenCell(Group.Group2<Integer, Integer> coords) {
		return wells.containsKey(coords) || stations.containsKey(coords) || pumps.containsKey(coords);
	}

	public Path getPathTo(Group.Group2<Integer, Integer> coords) {
		return Path.movesToPoint(tankerX, tankerY, coords.first, coords.second);
	}

	public void setUnreachable(Group.Group2<Integer, Integer> coords) {
		unreachable.add(coords);
	}

	public void resetUnreachable() {
		unreachable = new ArrayList<>();
	}

//	public Cell getCell(Group.Group2<Integer, Integer> coords) {
//		return cells.get(coords);
//	}

	// Assumes a station is passed
	public Task getTask(Group.Group2<Integer, Integer> coords) {
		Station cell = (Station) stations.get(coords);
		return cell.getTask();
	}

	@Deprecated
	public boolean validateCell(Group.Group2<Integer, Integer> coords, CellType type, boolean checkTask) {
		Cell cell = cells.get(coords);
		boolean isPump = cell instanceof FuelPump && type == CellType.PUMP;
		boolean isWell = cell instanceof Well && type == CellType.WELL;
		boolean isStation = cell instanceof Station && type == CellType.STATION
				&& (checkTask ? getTask(coords) != null : true);
		return isPump || isWell || isStation;
	}

	// Filters out unreachable objects
	public ArrayList<Group.Group2<Integer, Integer>> getCoords(CellType type) {
		ArrayList<Group.Group2<Integer, Integer>> keys;

		if (type == CellType.PUMP) {
			keys = new ArrayList<>(pumps.keySet());
		} else if (type == CellType.STATION) {
			keys = new ArrayList<>(stations.keySet());
		} else {
			keys = new ArrayList<>(wells.keySet());
		}

		for (Group.Group2<Integer, Integer> coords : unreachable) {
			if (keys.contains(coords)) {
				keys.remove(coords);
			}
		}

		return keys;
	}

	@Deprecated
	public ArrayList<Group.Group2<Integer, Integer>> getCells(CellType type, boolean checkTask) {
		ArrayList<Group.Group2<Integer, Integer>> list = new ArrayList<>();
		for (Group.Group2<Integer, Integer> coords : getCellKeys()) {
			if (validateCell(coords, type, checkTask)) {
				list.add(coords);
			}
		}
		return list;
	}

	public ArrayList<Group.Group2<Integer, Integer>> getStations() {
		ArrayList<Group.Group2<Integer, Integer>> stations = getCoords(CellType.STATION);
		// Sort stations by how close they are
		stations.sort(new Comparator<Group.Group2<Integer, Integer>>() {
			public int compare(Group.Group2<Integer, Integer> c1, Group.Group2<Integer, Integer> c2) {
				return distanceTo(c1) - distanceTo(c2);
			}
		});
		return stations;
	}

	/*
	 * Will return the coordinates of the closest entity from given coordinates
	 */
	public Group.Group2<Integer, Integer> findClosestCell(CellType type, Group.Group2<Integer, Integer> from) {
		Group.Group2<Integer, Integer> selected = null;
		int distance = Integer.MAX_VALUE;

		for (Group.Group2<Integer, Integer> coords : getCoords(type)) {
			if (!from.equals(coords)) {
				int newDist = Path.distance(from, coords);
				if (distance > newDist) {
					selected = coords;
					distance = newDist;
				}
			}
		}
		return selected;
	}

	public Group.Group2<Integer, Integer> findClosestCell(CellType type) {
		return findClosestCell(type, Group.make2(tankerX, tankerY));
	}

	public Group.Group2<Integer, Integer> getBestWell(Group.Group2<Integer, Integer> from) {
		ArrayList<Group.Group2<Integer, Integer>> list = getCells(CellType.WELL, false);
		double cost = Double.MAX_VALUE;
		double wasteMultiplier = tanker.getWasteCapacity() > (Tanker.MAX_WASTE / 2) ? 0.5 : 0.75;
		double fuelMultiplier = tanker.getFuelLevel() > (Tanker.MAX_FUEL / 2) ? 0.25 : 0.5;
		Group.Group2<Integer, Integer> selected = null;

		for (Group.Group2<Integer, Integer> coords : list) {
			// Check whether is reachable, otherwise we might get false paths
			if (!tanker.isReachable(from, coords).first) {
				continue;
			}
			Group.Group2<Integer, Integer> pump = findClosestCell(CellType.PUMP, coords);
			Group.Group2<Integer, Integer> station = findClosestCell(CellType.STATION, coords);
			double pathCost = Path.distance(from, coords);
			double pumpCost = Path.distance(coords, pump) * fuelMultiplier;
			double stationCost = pumpCost * 2;
			if (station != null) {
				stationCost = Path.distance(coords, station) * wasteMultiplier;
			}
			double myCost = pathCost + pumpCost + stationCost;
			if (cost > myCost) {
				selected = coords;
				cost = myCost;
			}
		}

		return selected != null ? selected : findClosestCell(CellType.WELL, from);
	}

	// Runs scoring methods
	public Group.Group2<Integer, Integer> getBestCell(CellType type, Group.Group2<Integer, Integer> from) {
		if (type == CellType.WELL) {
			return getBestWell(from);
		}
		return findClosestCell(type, from);
	}

	public Group.Group2<Integer, Integer> getBestCell(CellType type) {
		return getBestCell(type, Group.make2(tankerX, tankerY));
	}

	/*
	 * Calculates the absolute position of a cell, when given it's coordinates in
	 * the grid
	 */
	Group.Group2<Integer, Integer> getGlobalCoords(int col, int row) {
		Group.Group2<Integer, Integer> coords = viewIndicesToCoords(row, col);
		Path toCoord = Path.movesToPoint(0, 0, coords.first, coords.second);
		return new Path(toCoord, tankerX, tankerY).walk();
	}

	public void setCell(Group.Group2<Integer, Integer> coords, Cell cell, CellType type) {
		if (type == CellType.PUMP) {
			pumps.put(coords, cell);
		} else if (type == CellType.STATION) {
			stations.put(coords, cell);
		} else {
			wells.put(coords, cell);
		}
	}

	/*
	 * Helps to track distance to global objects that were encountered
	 */
	int registerMove(int direction) {
		Group.Group2<Integer, Integer> change = Path.moveChange(direction);

		this.tankerX += change.first;
		this.tankerY += change.second;

		return direction;
	}
}
