package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;

import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.Point;

// TODO reset should periodically clean up moves.
public class World {
	private ArrayList<Group.Group3<Cell, Integer, Integer>> cells = new ArrayList<>();
	private Path tankerPath = new Path();

	// Centre coordinates of the grid view
	private int cx;
	private int cy;

	public int tankerX = 0;
	public int tankerY = 0;

	World(int size) {
		// Because indices start at 0
		cy = cx = size;
	}

	public Group.Group2<Integer, Integer> viewIndicesToCoords(int row, int col) {
		return Group.make2(cx - col, cy - row);
	}

	boolean hasSeenCell(Cell cell) {
		Point cellP = cell.getPoint();
		for (Group.Group3<Cell, Integer, Integer> g3 : cells) {
			if (cellP.equals(g3.first.getPoint())) {
				return true;
			}
		}
		return false;
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
	int registerCell(Cell cell, int col, int row) {
		Group.Group2<Integer, Integer> coords = viewIndicesToCoords(row, col);
		Path toCoord = Path.movesToPoint(0, 0, coords.first, coords.second);

		Group.Group2<Integer, Integer> globalCoords = new Path(toCoord, tankerX, tankerY).walk();
		cells.add(Group.Group3.from2(cell, globalCoords));

		return Path.bestMove(tankerX, tankerY, globalCoords.first, globalCoords.second);
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
