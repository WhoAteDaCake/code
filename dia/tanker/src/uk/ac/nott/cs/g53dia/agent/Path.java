package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;

import uk.ac.nott.cs.g53dia.library.MoveAction;

public class Path {
	/*
	 * Calculates modifications to global state after move is made Used to track
	 * positions of
	 */
	static Pair<Integer, Integer> moveChange(int direction) {
		int x = 0;
		int y = 0;

		// Taken from MoveAction class
		switch (direction) {
		case MoveAction.NORTH:
			y++;
			break;
		case MoveAction.SOUTH:
			y--;
			break;
		case MoveAction.EAST:
			x++;
			break;
		case MoveAction.WEST:
			x--;
			break;
		case MoveAction.NORTHEAST:
			x++;
			y++;
			break;
		case MoveAction.NORTHWEST:
			x--;
			y++;
			break;
		case MoveAction.SOUTHEAST:
			x++;
			y--;
			break;
		case MoveAction.SOUTHWEST:
			x--;
			y--;
			break;
		}
		return Pair.make(x, y);
	}

	// Calculates the best move towards a specific point
	public static int bestMove(int fromX, int fromY, int toX, int toY) {
		// Check diagonals first
		if (fromX < toX && fromY < toY) {
			return MoveAction.NORTHWEST;
		} else if (fromX > toX && fromY < toY) {
			return MoveAction.NORTHEAST;
		} else if (fromX > toX && fromY > toY) {
			return MoveAction.SOUTHEAST;
		} else if (fromX < toX && fromY > toY) {
			return MoveAction.SOUTHWEST;
			// Now validate singular direction moves
		} else if (fromX > toX) {
			return MoveAction.EAST;
		} else if (fromX < toX) {
			return MoveAction.WEST;
		} else if (fromY > toY) {
			return MoveAction.SOUTH;
		}
		// fromY < toY
		return MoveAction.NORTH;
	}

	// Calculates the best sequence of moves from one point to other
	// Assumes a 2d grid.
	private static ArrayList<Integer> movesToPointAux(int fromX, int fromY, int toX, int toY,
			ArrayList<Integer> current) {
		if (fromX == toX && fromY == toY) {
			return current;
		}
		int nextMove = bestMove(fromX, fromY, toX, toY);
		current.add(nextMove);
		Pair<Integer, Integer> move = moveChange(nextMove);
		return movesToPointAux(fromX + move.first, fromY + move.second, toX, toY, current);
	}

	static ArrayList<Integer> movesToPoint(int fromX, int fromY, int toX, int toY) {
		return movesToPointAux(fromX, fromY, toX, toY, new ArrayList<>());
	}

	ArrayList<Integer> path;
	int x = 0;
	int y = 0;

	static Path make() {
		return new Path(new ArrayList<>());
	}

	public Path(ArrayList<Integer> path) {
		this.path = path;
	}

	public void addMove(int move) {
		this.path.add(move);
	}

	public void addMoves(ArrayList<Integer> moves) {
		this.path.addAll(moves);
	}

	public Pair<Integer, Integer> walk() {
		int x = this.x;
		int y = this.y;
		for (int move : path) {
			Pair<Integer, Integer> modifier = moveChange(move);
			x += modifier.first;
			y += modifier.second;
		}
		return Pair.make(x, y);
	}

}
