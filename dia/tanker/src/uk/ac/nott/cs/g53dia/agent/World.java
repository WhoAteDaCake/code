package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.HashMap;

import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.FuelPump;
import uk.ac.nott.cs.g53dia.library.Station;
import uk.ac.nott.cs.g53dia.library.Well;

// TODO reset should periodically clean up moves.
public class World {
	private HashMap<Group.Group2<Integer, Integer>, Cell> cells = new HashMap<>();
//	private ArrayList<Group.Group3<Cell, Integer, Integer>> cells = new ArrayList<>();
	private Path tankerPath = new Path();

	// Centre coordinates of the grid view
	private int cx;
	private int cy;

	public int tankerX = 0;
	public int tankerY = 0;

	public World(int size) {
		// Because indices start at 0
		cy = cx = size;
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
		return cells.containsKey(coords);
	}

	public Path getPathTo(Group.Group2<Integer, Integer> coords) {
		return Path.movesToPoint(tankerX, tankerY, coords.first, coords.second);
	}

	public ArrayList<Group.Group2<Integer, Integer>> getStations() {
		ArrayList<Group.Group2<Integer, Integer>> stations = new ArrayList<>();
		for (Group.Group2<Integer, Integer> coords : cells.keySet()) {
			Cell cell = cells.get(coords);
			if (cell instanceof Station) {
				stations.add(coords);
			}
		}
		// Sort stations by how close they are
		stations.sort(new Comparator<Group.Group2<Integer, Integer>>() {
			public int compare(Group.Group2<Integer, Integer> c1, Group.Group2<Integer, Integer> c2) {
				return distanceTo(c1) - distanceTo(c2);
			}
		});
		return stations;
	}

	/*
	 * Will return the distance and the path that should be followed to get there
	 */
	public Group.Group2<Integer, Integer> findClosestCell(CellType type) {
		// Coordinate
		Group.Group2<Integer, Integer> selected = null;
		int distance = Integer.MAX_VALUE;

		for (Group.Group2<Integer, Integer> coords : cells.keySet()) {
			Cell cell = cells.get(coords);

			boolean isPump = cell instanceof FuelPump && type == CellType.PUMP;
			boolean isWell = cell instanceof Well && type == CellType.WELL;
			boolean isTaskStation = cell instanceof Station && type == CellType.STATION
					&& ((Station) cell).getTask() != null;

			if (isPump || isWell || isTaskStation) {
				int newDist = distanceTo(coords);
				if (distance > newDist) {
					selected = coords;
					distance = newDist;
				}
			}
		}
		return selected;
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

	/*
	 * Used to update stations
	 */
	public void updateCell(Cell cell, Group.Group2<Integer, Integer> coords) {
		cells.put(coords, cell);
	}

	/*
	 * Helps to register new special cells Should be used when scouting the
	 * surrounding cells Because it should be used in nested for loops, we can pass
	 * variables To help calculating absolute positions of these cells
	 * 
	 * Works by: 1. Calculating the moves from tanker to the point 2. Adding those
	 * moves to the current tanker moves 3. Simulating the moves required to get
	 * coordinates relative to the centre. 4. Return the direction that we should
	 * move towards.
	 */
	int registerCell(Cell cell, Group.Group2<Integer, Integer> coords) {
		updateCell(cell, coords);
		return Path.bestMove(tankerX, tankerY, coords.first, coords.second);
	}

	/*
	 * Helps to track distance to global objects that were encountered
	 */
	int registerMove(int direction) {
		tankerPath.addMove(direction);
		Group.Group2<Integer, Integer> change = Path.moveChange(direction);

		this.tankerX += change.first;
		this.tankerY += change.second;

		return direction;
	}
}
