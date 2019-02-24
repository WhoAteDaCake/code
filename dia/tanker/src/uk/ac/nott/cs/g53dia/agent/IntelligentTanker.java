package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;
import java.util.Random;

import uk.ac.nott.cs.g53dia.library.Action;
import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.DisposeWasteAction;
import uk.ac.nott.cs.g53dia.library.EmptyCell;
import uk.ac.nott.cs.g53dia.library.LoadWasteAction;
import uk.ac.nott.cs.g53dia.library.MoveAction;
import uk.ac.nott.cs.g53dia.library.RefuelAction;
import uk.ac.nott.cs.g53dia.library.Station;
import uk.ac.nott.cs.g53dia.library.Tanker;
import uk.ac.nott.cs.g53dia.library.Task;

public class IntelligentTanker extends Tanker {
	State state = State.SCOUTING;
	State prevState = State.SCOUTING;
	State savedState = null;
	World world = new World(Tanker.VIEW_RANGE);

	Path activePath;

	ArrayList<Group.Group2<Integer, Integer>> scoutedStations = new ArrayList<>();
	Path scoutePath;
	Task task;

	public IntelligentTanker() {
		this(new Random(50));
	}

	/**
	 * The tanker implementation makes random moves. For reproducibility, it can
	 * share the same random number generator as the environment.
	 * 
	 * @param r The random number generator.
	 */
	public IntelligentTanker(Random r) {
		this.r = r;
	}

	/*
	 * Find all cells nearby that are special (well/station/pump) TODO: in the
	 * future probably don't need to run it every single move
	 */
	private void analyseView(Cell[][] view) {
		for (int x = 0; x < view.length; x += 1) {
			for (int y = 0; y < view[x].length; y += 1) {
				Cell current = view[x][y];

				if (current instanceof EmptyCell) {
					continue;
				}
				Group.Group2<Integer, Integer> coords = world.getGlobalCoords(x, y);
				if (world.hasSeenCell(coords)) {
					// If it's a station, it should be updated in order to make sure any new tasks
					// are retrieved
					if (current instanceof Station) {
						world.updateCell(current, coords);
					}

				} else {
					world.registerCell(current, coords);
				}

				// This means that it's a special cell
//				world.registerCell(current, x, y);
			}
		}
	}

	// TODO check distance to nearest fuelling station as well
	public boolean canAfford(Path path) {
		// Each move costs 2 fuel and car should be able to go there and back
		int pathPrice = path.stepCount() * 2;
		Group.Group2<Integer, Integer> pathCoords = new Path(path, world.tankerX, world.tankerY).walk();
		Group.Group2<Integer, Integer> pump = world.getNearestPump();
		int toPumpPrice = Path.distance(pathCoords, pump);
		int fuel = getFuelLevel();
		return pathPrice + toPumpPrice < fuel;
	}

	public MoveAction registeredMove(int direction) {
		return new MoveAction(this.world.registerMove(direction));
	}

	public MoveAction scout() {
		int randMove = this.r.nextInt(8);
		int nextMove = -1;
		// If we are looking to fill some waste up
		State nextState = null;
		CellType lookFor = null;
		if (this.prevState == State.SCOUTING || this.prevState == State.MOVING_TO_WELL) {
			nextState = State.MOVING_TO_STATION;
			lookFor = CellType.STATION;
		} else if (this.prevState == State.LOADING_WASTE) {
			nextState = State.MOVING_TO_WELL;
			lookFor = CellType.WELL;
		}

		Group.Group2<Integer, Path> result = world.findClosestCell(lookFor);
		if (result != null) {
			scoutePath = null;
			activePath = result.second;
			nextMove = activePath.step();
			this.state = nextState;
			// If we found the cell, reset scouting paths
			scoutePath = null;
		} else {
			// TODO check for refueling
			// Check if we are weakly moving towards a station
			if (scoutePath == null || !scoutePath.hasSteps()) {
				// If not check if we have a list of stations to check
				if (scoutedStations.size() == 0) {
					scoutedStations = world.getStations();
				}
				// If we still have to stations registered, just move randomly
				if (scoutedStations.size() == 0) {
					return this.registeredMove(randMove);
				}
				// Otherwise set the sourcePath to the first station
				Group.Group2<Integer, Integer> head = scoutedStations.get(0);
				scoutedStations.remove(0);
				scoutePath = Path.movesToPoint(world.tankerX, world.tankerY, head.first, head.second);
				// If we can't afford the scoutePath we need to refuel
				if (!this.canAfford(scoutePath)) {
					activePath = world.getPathTo(world.getNearestPump());
					// TODO maybe not needed?
					this.savedState = this.state;
					this.state = State.REFUELING;
					return this.registeredMove(activePath.step());
				}
			}
			return this.registeredMove(scoutePath.step());
		}

		return this.registeredMove(nextMove < 0 ? randMove : nextMove);
	}

	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		// Because we have no resource restrictions, it's best to just analyse every
		// time we move
		analyseView(view);

		// If we have a path, should always complete it.
		if (this.activePath != null && this.activePath.hasSteps()) {
			return this.registeredMove(activePath.step());
		}

//		if (this.savedState != null) {
//			this.state = this.savedState;
//			this.savedState = null;
//		}

		if (this.state == State.REFUELING) {
			this.state = this.savedState;
			return new RefuelAction();
		} else if (this.state == State.SCOUTING) {
			return this.scout();
		} else if (this.state == State.MOVING_TO_STATION) {
			// Begin loading
			this.prevState = state;
			this.state = State.LOADING_WASTE;
			this.task = ((Station) getCurrentCell(view)).getTask();
			return new LoadWasteAction(this.task);
		} else if (this.state == State.MOVING_TO_WELL) {
			this.prevState = this.state;
			this.state = State.SCOUTING;
			return new DisposeWasteAction();
		} else if (this.state == State.LOADING_WASTE) {
			// Means we still have some waste left
			if (!this.task.isComplete()) {
				return new LoadWasteAction(this.task);
			}
			this.task = null;
			this.prevState = this.state;
			this.state = State.SCOUTING;
			return this.scout();
		}

		return null;
	}

}
