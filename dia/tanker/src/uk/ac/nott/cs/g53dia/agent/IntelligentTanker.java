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
import uk.ac.nott.cs.g53dia.library.Well;

public class IntelligentTanker extends Tanker {
	State state = State.ROAMING;
	State savedState = State.ROAMING;
	World world = new World(Tanker.VIEW_RANGE);
	Task task;

	Path activePath;
	Group.Group2<Integer, Integer> roamCoords = null;
	Group.Group2<Integer, Integer> prevRoam = null;

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
					// are retrieved (Environment does a deep clone, so our reference becomes
					// invalid)
					if (current instanceof Station) {
						world.updateCell(current, coords);
					}

				} else {
					world.registerCell(current, coords);
				}
			}
		}
	}

	public boolean canAfford(Path path) {
		// Each move costs 2 fuel and car should be able to go there and back
		int pathPrice = path.stepCount() * 2;
		Group.Group2<Integer, Integer> pathCoords = new Path(path, world.tankerX, world.tankerY).walk();
		Group.Group2<Integer, Integer> pump = world.findClosestCell(CellType.PUMP);
		// Make sure we can reach a pump after said point
		int toPumpPrice = Path.distance(pathCoords, pump);
		return pathPrice + toPumpPrice < getFuelLevel();
	}

	public MoveAction registeredMove(int direction) {
		return new MoveAction(world.registerMove(direction));
	}

	// Try to find a station that can be reached for roaming
	// TODO check if we need to maintain the coordinates of last visited station
	public Group.Group2<Integer, Integer> getReachableStation() {
		Group.Group2<Integer, Integer> coords;
		ArrayList<Group.Group2<Integer, Integer>> stations = world.getStations();
		// Want to maintain positive bounds
		while (stations.size() > 1) {
			int index = r.nextInt(stations.size() - 1);
			coords = stations.get(index);
			if (prevRoam != coords && canAfford(world.getPathTo(coords))) {
				return coords;
			} else {
				stations.remove(index);
			}
		}
		return null;
	}

	public Action followPath() {
		return registeredMove(activePath.step());
	}

	public Action refuel() {
		state = State.MOVING_TO_FUEL;
		activePath = world.getPathTo(world.findClosestCell(CellType.PUMP));
		return followPath();
	}

	// Used for wells and stations
	public Action moveTo(Group.Group2<Integer, Integer> coords, State nextState) {
		// When we find something with a task, we don't care about previous roaming
		if (nextState == State.MOVING_TO_WELL) {
			prevRoam = null;
			roamCoords = null;
		}
		if (!canAfford(world.getPathTo(coords))) {
			savedState = nextState;
			return refuel();
		}
		activePath = world.getPathTo(coords);
		state = nextState;
		return followPath();
	}

	public Action roam() {
		Group.Group2<Integer, Integer> result = world.findClosestCell(CellType.STATION);
		// Station with a task found
		// TODO maybe use supplied distance?
		if (result != null) {
			return moveTo(result, State.MOVING_TO_STATION);
		}
		// Make sure we have a roam target
		if (roamCoords == null) {
			roamCoords = getReachableStation();
			// If no reachable station exists, refuel
			if (roamCoords == null) {
				return refuel();
			}
			// If we are already here, find another station
		} else if (world.standingOn(roamCoords)) {
			prevRoam = roamCoords;
			roamCoords = null;
			return roam();
		}
		state = State.ROAMING;
		return registeredMove(Path.bestMove(world.tankerX, world.tankerY, roamCoords.first, roamCoords.second));
	}

	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		// Because we have no resource restrictions, it's best to just analyse the
		// surrounding area every time we move
		analyseView(view);
		Cell cell = getCurrentCell(view);

		// TODO in the future check if we are standing on fuel pump, if so, refuel
		// If we have a path, should always complete it.
		if (activePath != null && activePath.hasSteps()) {
			followPath();
		}

		// Safe to assume that at this point we stand on the needed cell
		if (state == State.ROAMING) {
			return roam();
		} else if (state == State.MOVING_TO_STATION) {
			// Means we got interrupted
			if (!(cell instanceof Station)) {
				return roam();
			}
			// If not start consuming
			task = ((Station) cell).getTask();
			state = State.CONSUMING;
			return new LoadWasteAction(task);
		} else if (state == State.CONSUMING) {
			// Means we still have some waste left
			if (!task.isComplete()) {
				return new LoadWasteAction(task);
			} else {
				return this.moveTo(world.findClosestCell(CellType.WELL), State.MOVING_TO_WELL);
			}
		} else if (state == State.MOVING_TO_WELL) {
			// We got interrupted
			if (!(cell instanceof Well)) {
				return this.moveTo(world.findClosestCell(CellType.WELL), State.MOVING_TO_WELL);
			}
			// Otherwise start disposing
			state = State.DISPOSING;
			return new DisposeWasteAction();
		} else if (state == State.DISPOSING) {
			if (getWasteLevel() != 0) {
				return new DisposeWasteAction();
			}
			// When disposed, go back to roaming
			return roam();
		} else if (state == State.MOVING_TO_FUEL) {
			state = State.REFUELING;
			return new RefuelAction();
		} else if (state == State.REFUELING) {
			if (!(getFuelLevel() == MAX_FUEL)) {
				return new RefuelAction();
			}
			state = savedState;
			savedState = null;
			// No saved state means we were not interrupted
			if (state == null) {
				state = State.ROAMING;
			}
			;

			return senseAndAct(view, actionFailed, timestep);

		}

		return null;
	}

}
