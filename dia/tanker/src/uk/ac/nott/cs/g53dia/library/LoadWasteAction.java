package uk.ac.nott.cs.g53dia.library;

/**
 * Action that loads waste into the tanker from a station.
 *
 * @author Julian Zappala
 */

/*
 * Copyright (c) 2011 Julian Zappala (jxz@cs.nott.ac.uk)
 * 
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

public class LoadWasteAction extends InfallibleAction {

	Task task;

	public LoadWasteAction(Task t) {
		task = t;
	}

	public void actionSucceeds(Environment env, Tanker tank) throws IllegalActionException {

		if (!(tank.getPosition().equals(task.getStationPosition()))) {
			throw new IllegalActionException("LoadWaste: Not at Station");
		}

		if (task.isComplete()) {
			throw new IllegalActionException("LoadWaste: Task already complete");
		}

		if (tank.wasteLevel >= Tanker.MAX_WASTE) {
			throw new IllegalActionException("LoadWaste: Waste tank is full");
		} else if (tank.getWasteCapacity() >= task.getWasteRemaining()) {
			tank.wasteLevel += task.getWasteRemaining();
			task.dispose(task.getWasteRemaining());
		} else {
			task.dispose(tank.getWasteCapacity());
			tank.wasteLevel = Tanker.MAX_WASTE;
		}

		if (task.isComplete()) {
			tank.wasteDisposed += task.amount;
		}
	}

	public String toString() {
		return "LoadWaste";
	}
}
