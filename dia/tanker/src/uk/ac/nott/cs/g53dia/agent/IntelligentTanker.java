package uk.ac.nott.cs.g53dia.agent;

import java.util.ArrayList;
import java.util.Random;

import uk.ac.nott.cs.g53dia.library.Action;
import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.DisposeWasteAction;
import uk.ac.nott.cs.g53dia.library.EmptyCell;
import uk.ac.nott.cs.g53dia.library.FuelPump;
import uk.ac.nott.cs.g53dia.library.LoadWasteAction;
import uk.ac.nott.cs.g53dia.library.MoveAction;
import uk.ac.nott.cs.g53dia.library.RefuelAction;
import uk.ac.nott.cs.g53dia.library.Station;
import uk.ac.nott.cs.g53dia.library.Tanker;
import uk.ac.nott.cs.g53dia.library.Task;

public class IntelligentTanker extends Tanker {
	// When we recursively call sense function
	Cell[][] savedView;
	boolean savedActionFailed;
	long savedTimestep;
	boolean pumpLoaded = false;
	boolean refueled = false;

	State state = State.ROAMING;
	World world;

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
			world.setCell(Group.make2(0, 0), getCurrentCell(view), CellType.PUMP);
		}
		for (int x = 0; x < view.length; x += 1) {
			for (int y = 0; y < view[x].length; y += 1) {
				Cell current = view[x][y];
				CellType type;

				if (current instanceof EmptyCell) {
					continue;
				} else if (current instanceof Station) {
					type = CellType.STATION;
				} else if (current instanceof FuelPump) {
					type = CellType.PUMP;
				} else {
					type = CellType.WELL;
				}
				world.setCell(world.getGlobalCoords(x, y), current, type);
			}
		}
	}

	// Some point's can't be reached due to limit on max fuel
	// This will return <canReach, possibleToReach>
	public Group.Group2<Boolean, Boolean> isReachable(Group.Group2<Integer, Integer> from,
			Group.Group2<Integer, Integer> coords) {
		// Each move costs 2 fuel and car should be able to go there and back
		int pathPrice = Path.distance(from, coords);
		// From path, walk to pump
		Group.Group2<Integer, Integer> pump = world.getBestCell(CellType.PUMP, coords);
		// Make sure we can reach a pump after said point
		int toPumpPrice = Path.distance(coords, pump);
		int fullPrice = (pathPrice + toPumpPrice) * 2;
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
	public Group.Group2<Integer, Integer> getRoamingPoint() {
		Group.Group2<Integer, Integer> coords;
		ArrayList<Group.Group2<Integer, Integer>> stations = world.getStations(false);

		// Want to maintain positive bounds
		while (stations.size() > 1) {
			int index = r.nextInt(stations.size() - 1);
			coords = stations.get(index);
			if (!coords.equals(prevRoam) && isReachable(coords).first) {
				return coords;
			} else {
				stations.remove(index);
			}
		}

		int modifier = r.nextInt(10) > 5 ? -20 : 20;
		return Group.make2(world.tankerX + modifier, world.tankerY + modifier);
	}

	public Action followPath() {
		return registeredMove(activePath.step());
	}

	public Action goTo(Group.Group2<Integer, Integer> coords) {
		activePath = world.getPathTo(coords);
		// Means we are already at the needed place
		if (!activePath.hasSteps()) {
			return senseAndAct();
		}

		return followPath();
	}

	public Action refuel() {
		state = State.MOVING_TO_FUEL;
		return goTo(world.getBestCell(CellType.PUMP));
	}

	// Checks we can consume task at coordinates
	public boolean canConsume(Group.Group2<Integer, Integer> coords) {
		Task task = world.getTask(coords);
		if (task == null) {
			return false;
		}
		return task.getWasteRemaining() < getWasteCapacity();
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
			return refuel();
			// It's easier to save the coordinates as unreachable, rather than to manually
			// recalculate
		} else if (!result.second) {
			// If the coordinates are unreachable
			world.setUnreachable(coords);
			return roam();
		}
		state = nextState;
		return goTo(coords);
	}

	public Action tryToPickupTask() {
		Group.Group2<Integer, Integer> wellCoords = world.getBestCell(CellType.WELL);
		// Verify that the well is reachable
		if (wellCoords == null) {
			return roam();
		} else if (!isReachable(wellCoords).second) {
			// Verify that the well is reachable
			// This means we need to check for a nearest pump station to that well and try
			// to move there
			wellCoords = world.findClosestCell(CellType.PUMP, wellCoords);
		}

		// If we are already at max waste no point checking
		if (getWasteCapacity() == 0) {
			return moveTo(wellCoords, State.MOVING_TO_WELL);
		}

		Group.Group2<Integer, Integer> coords = world.getBestStation(world.tankerPos());
		// Make sure that not better to just deposit now
		if (coords == null || world.distanceTo(wellCoords) * 2 < world.distanceTo(coords)) {
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

		if (roamCoords == null || world.standingOn(roamCoords)) {
			prevRoam = roamCoords;
			roamCoords = getRoamingPoint();
			// If no reachable station exists, refuel
			if (roamCoords == null || !isReachable(roamCoords).first) {
				// Need to make sure that we aren't just in a situation, where there aren't any
				// reachable stations
				return refuel();
			}
		}
		state = State.ROAMING;
		return registeredMove(Path.bestMove(world.tankerX, world.tankerY, roamCoords.first, roamCoords.second));
	}

	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep, boolean repeating) {
		// Cache results
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

		if (!refueled && cell instanceof FuelPump && getFuelLevel() != MAX_FUEL) {
			refueled = true;
			return new RefuelAction();
		}
		// If we have a path, should always complete it.
		if (activePath != null && activePath.hasSteps()) {
			refueled = false;
			return followPath();
		}
		refueled = false;

		// Safe to assume that at this point we stand on the needed cell
		if (state == State.ROAMING || state == State.DISPOSING) {
			return roam();
		}
		prevRoam = null;
		roamCoords = null;
		if (state == State.MOVING_TO_STATION) {
			state = State.CONSUMING;
			return new LoadWasteAction(((Station) cell).getTask());
		} else if (state == State.CONSUMING) {
			return tryToPickupTask();
		} else if (state == State.MOVING_TO_WELL) {
			state = State.DISPOSING;
			return new DisposeWasteAction();
		}
		// Because we check for refuelling before, by this point we already refuelled
		if (getWasteCapacity() != MAX_WASTE) {
			return tryToPickupTask();
		}
		return roam();
	}

	public Action senseAndAct() {
		return senseAndAct(savedView, savedActionFailed, savedTimestep, true);
	}

	@Override
	public Action senseAndAct(Cell[][] view, boolean actionFailed, long timestep) {
		return senseAndAct(view, actionFailed, timestep, false);
	}
}
