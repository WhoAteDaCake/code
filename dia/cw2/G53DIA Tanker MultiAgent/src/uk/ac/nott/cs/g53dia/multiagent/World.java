package uk.ac.nott.cs.g53dia.multiagent;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;

import uk.ac.nott.cs.g53dia.multilibrary.Cell;

public class World {
	private HashSet<Group.Group2<Integer, Integer>> pumps = new HashSet<>();
	private HashSet<Group.Group2<Integer, Integer>> wells = new HashSet<>();
	private HashMap<Group.Group2<Integer, Integer>, Cell> stations = new HashMap<>();
	
	// Stations that have a tanker moving towards
	private HashSet<Group.Group2<Integer, Integer>> reserved = new HashSet<>();
	private HashSet<Group.Group2<Integer, Integer>> unreachable = new HashSet<>();
	
	private int cx;
	private int cy;
	
	public World(int size) {
		cx = cy = size /2;
		// Initial pump location
		pumps.add(Group.make2(0, 0));
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
