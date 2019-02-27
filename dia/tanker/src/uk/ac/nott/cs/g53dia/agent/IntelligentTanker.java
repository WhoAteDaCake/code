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
	// When we recursively call sense function
	Cell[][] savedView;
	boolean savedActionFailed;
	long savedTimestep;
	boolean pumpLoaded = false;

	State state = State.ROAMING;
	State savedState = null;
	World world;
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
		world = new World(Tanker.VIEW_RANGE, this);
	}

	/*
	 * Find all cells nearby that are special (well/station/pump) TODO: in the
	 * future probably don't need to run it every single move
	 */
	private void analyseView(Cell[][] view) {
		// Make sure pump is loaded to allow for distance calculations
		if (!pumpLoaded) {
			pumpLoaded = true;
			world.registerCell(getCurrentCell(view), Group.make2(0, 0));
		}
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
					// Only register reachable cells
				} else if (isReachable(coords).second) {
					world.registerCell(current, coords);
				}
			}
		}
	}

	// Some point's can't be reached due to limit on max fuel
	// This will return <canReach, possibleToReach>
	public Group.Group2<Boolean, Boolean> isReachable(Group.Group2<Integer, Integer> from,
			Group.Group2<Integer, Integer> coords) {
		// Each move costs 2 fuel and car should be able to go there and back
		int pathPrice = Path.distance(from, coords) * 2;
		// From path, walk to pump
		Group.Group2<Integer, Integer> pump = world.getBestCell(CellType.PUMP, coords);
		// Make sure we can reach a pump after said point
		int toPumpPrice = Path.distance(coords, pump) * 2;
		int fullPrice = pathPrice + toPumpPrice;
		return Group.make2(fullPrice < getFuelLevel(), fullPrice < MAX_FUEL);
	}

	public Group.Group2<Boolean, Boolean> isReachable(Group.Group2<Integer, Integer> coords) {
		return isReachable(Group.make2(world.tankerX, world.tankerY), coords);
	}

	public MoveAction registeredMove(int direction) {
		return new MoveAction(world.registerMove(direction));
	}

	// Try to find a station that can be reached for roaming
	// Chosen at random
	public Group.Group2<Integer, Integer> getReachableStation() {
		Group.Group2<Integer, Integer> coords;
		ArrayList<Group.Group2<Integer, Integer>> stations = world.getStations();
		// Want to maintain positive bounds
		while (stations.size() > 1) {
			int index = r.nextInt(stations.size() - 1);
			coords = stations.get(index);
			if (prevRoam != coords && isReachable(coords).first) {
				return coords;
			} else {
				stations.remove(index);
			}
		}
		return null;
	}

	public Action followPath() {
		if (activePath.hasSteps()) {
			return registeredMove(activePath.step());
		} else {
			return senseAndAct();
		}
	}

	public Action refuel() {
		state = State.MOVING_TO_FUEL;
		activePath = world.getPathTo(world.getBestCell(CellType.PUMP));
		return followPath();
	}

	// Used for wells and stations
	public Action moveTo(Group.Group2<Integer, Integer> coords, State nextState) {
		// When we find something with a task, we don't care about previous roaming
		if (nextState == State.MOVING_TO_STATION) {
			prevRoam = null;
			roamCoords = null;
		}

		Group.Group2<Boolean, Boolean> result = isReachable(coords);
		if (!result.first && result.second) {
			savedState = nextState;
			return refuel();
		} else if (!result.second) {
			// If the coordinates are unreachable
			world.setUnreachable(coords);
			return roam();
		}
		activePath = world.getPathTo(coords);
		state = nextState;
		return followPath();
	}

	public Action tryToPickupTask() {
		// If we are already at max waste no point checking
		if (getWasteCapacity() == 0) {
			return moveTo(world.getBestCell(CellType.WELL), State.MOVING_TO_WELL);
		}

		// See if there any stations nearby that we can pick up waste from
		ArrayList<Group.Group2<Integer, Integer>> stations = world.getStations();
		ArrayList<Group.Group2<Integer, Integer>> selected = new ArrayList<>();
		Group.Group2<Integer, Integer> myCoords = Group.make2(world.tankerX, world.tankerY);

		for (Group.Group2<Integer, Integer> coords : stations) {
			Task task = world.getTask(coords);
			if (!coords.equals(myCoords) && task != null && isReachable(coords).first
					&& task.getWasteRemaining() < getWasteCapacity()) {
				selected.add(coords);
			}
		}
		Group.Group2<Integer, Integer> wellCoords = world.getBestCell(CellType.WELL);
		// Can't pick up any more
		if (selected.size() < 1) {
			return moveTo(wellCoords, State.MOVING_TO_WELL);
		}
		// Make sure that not better to just deposit now
		Group.Group2<Integer, Integer> coords = selected.get(0);

		//
		int scoreMultiplier = 2;

		if (world.distanceTo(wellCoords) * scoreMultiplier < world.distanceTo(coords)) {
			return moveTo(wellCoords, State.MOVING_TO_WELL);
		}

		return moveTo(coords, State.MOVING_TO_STATION);
	}

	public Action roam() {
		Group.Group2<Integer, Integer> result = world.getBestCell(CellType.STATION);
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

	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep, boolean repeating) {
		savedView = view;
		savedActionFailed = actionFailed;
		savedTimestep = timestep;
		// Because we have no resource restrictions, it's best to just analyse the
		// surrounding area every time we move
		// The check here is added to make sure we don't scan again after refuelling
		if (!repeating) {
			analyseView(view);
		}
		Cell cell = getCurrentCell(view);

		// TODO in the future check if we are standing on fuel pump, if so, refuel
		// If we have a path, should always complete it.
		if (activePath != null && activePath.hasSteps()) {
			return followPath();
		}

		// TEMP
		if (timestep > 184) {
			int b = 2;
			int c = b + b;
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
				return tryToPickupTask();
			}
		} else if (state == State.MOVING_TO_WELL) {
			// We got interrupted
			if (!(cell instanceof Well)) {
				return moveTo(world.getBestCell(CellType.WELL), State.MOVING_TO_WELL);
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

			return senseAndAct();

		}

		return null;
	}

	public Action senseAndAct() {
		return senseAndAct(savedView, savedActionFailed, savedTimestep, true);
	}

	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		return senseAndAct(view, actionFailed, timestep, false);
	}
}
