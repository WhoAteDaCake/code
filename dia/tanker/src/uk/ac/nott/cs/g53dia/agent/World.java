package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;

import uk.ac.nott.cs.g53dia.library.Cell;

// TODO reset should periodically clean up moves.
public class World {
	private int viewSize = 1;
	private ArrayList<Pair<Cell, Pair<Integer, Integer>>> cells = new ArrayList<>();
	private Path tankerPath = new Path();

	// Centre coordinates of the world view
	private int cx;
	private int cy;

	public int tankerX = 0;
	public int tankerY = 0;

	World(int size) {
		this.viewSize = size;
		// Because indices start at 0
		cy = cx = size;
	}

	public Pair<Integer, Integer> viewIndicesToCoords(int row, int col) {
		return Pair.make(cx - col, cy - row);
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
	int registerCell(Cell cell, int row, int col) {
		Pair<Integer, Integer> coords = viewIndicesToCoords(row, col);
		Path toCoord = Path.movesToPoint(0, 0, coords.first, coords.second);
		Pair<Integer, Integer> globalCoords = new Path(toCoord, tankerX, tankerY).walk();
		cells.add(Pair.make(cell, globalCoords));
		return Path.bestMove(tankerX, tankerY, globalCoords.first, globalCoords.second);
	}

	/*
	 * Helps to track distance to global objects that were encountered
	 */
	int registerMove(int direction) {
		tankerPath.addMove(direction);
		Pair<Integer, Integer> change = Path.moveChange(direction);

		this.tankerX += change.first;
		this.tankerY += change.second;

		return direction;
	}
}
