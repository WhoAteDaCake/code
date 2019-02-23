package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;

import uk.ac.nott.cs.g53dia.agent.Pair;
import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.MoveAction;

public class World {
	private int viewSize = 1;
	private ArrayList<Cell> cells = new ArrayList<>();
//	private ArrayList<Pair<Integer, Integer>> 
		
	public int tankerX = 0;
	public int tankerY = 0;
	
	World(int size) {
		this.viewSize = size;
	}

	
	/*
	 * Helps to register new special cells
	 * Should be used when scouting the surrounding cells
	 * Because it should be used in nested for loops, we can pass variables
	 * To help calculating absolute positions of these cells
	 * 
	 * Works by:
	 * 1. Calculating the moves 
	 */
	void registerCell(Cell cell, int x, int y) {
		
	}
	
	/*
	 * Helps to track distance to global objects that were encountered
	 */
	int registerMove(int direction) {
		Pair<Integer,Integer> change = Path.moveChange(direction);
		
		this.tankerX += change.first;
		this.tankerY += change.second;
		
		return direction;
	}
}
