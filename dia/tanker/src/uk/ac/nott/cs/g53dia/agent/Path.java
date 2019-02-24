package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;

import uk.ac.nott.cs.g53dia.library.MoveAction;

public class Path {
	/*
	 * Calculates modifications to global state after move is made Used to track
	 * positions of
	 */
	static Group.Group2<Integer, Integer> moveChange(int direction) {
		int x = 0;
		int y = 0;

		// Modified from MoveAction class
		switch (direction) {
		case MoveAction.NORTH:
			y++;
			break;
		case MoveAction.SOUTH:
			y--;
			break;
		case MoveAction.EAST:
			x--;
			break;
		case MoveAction.WEST:
			x++;
			break;
		case MoveAction.NORTHEAST:
			x--;
			y++;
			break;
		case MoveAction.NORTHWEST:
			x++;
			y++;
			break;
		case MoveAction.SOUTHEAST:
			x--;
			y--;
			break;
		case MoveAction.SOUTHWEST:
			x++;
			y--;
			break;
		}
		return Group.make2(x, y);
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
	private static Path movesToPointAux(int fromX, int fromY, int toX, int toY, Path path) {
		if (fromX == toX && fromY == toY) {
			return path;
		}
		int nextMove = bestMove(fromX, fromY, toX, toY);
		path.addMove(nextMove);
		Group.Group2<Integer, Integer> move = moveChange(nextMove);
		return movesToPointAux(fromX + move.first, fromY + move.second, toX, toY, path);
	}

	public static Path movesToPoint(int fromX, int fromY, int toX, int toY) {
		return movesToPointAux(fromX, fromY, toX, toY, new Path());
	}

	public static Path combine(Path p1, Path p2) {
		Path p3 = new Path();
		p3.addMoves(p1.path);
		p3.addMoves(p2.path);
		return p3;
	}

	public ArrayList<Integer> path;
	int x = 0;
	int y = 0;

	public Path() {
		this(new ArrayList<>(), 0, 0);
	}

	public Path(ArrayList<Integer> path) {
		this(path, 0, 0);
	}

	public Path(Path path, int x, int y) {
		this(path.path, x, y);
	}

	public Path(ArrayList<Integer> path, int x, int y) {
		this.path = path;
		this.x = x;
		this.y = y;
	}

	public void addMove(int move) {
		this.path.add(move);
	}

	public void addMoves(ArrayList<Integer> moves) {
		this.path.addAll(moves);
	}

	public Group.Group2<Integer, Integer> walk() {
		int x = this.x;
		int y = this.y;
		for (int move : path) {
			Group.Group2<Integer, Integer> modifier = moveChange(move);
			x += modifier.first;
			y += modifier.second;
		}
		return Group.make2(x, y);
	}

}
