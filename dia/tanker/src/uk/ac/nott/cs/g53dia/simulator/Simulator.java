package uk.ac.nott.cs.g53dia.simulator;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

import uk.ac.nott.cs.g53dia.agent.DemoTanker;
import uk.ac.nott.cs.g53dia.agent.IntelligentTanker;
import uk.ac.nott.cs.g53dia.library.Action;
import uk.ac.nott.cs.g53dia.library.Cell;
import uk.ac.nott.cs.g53dia.library.Environment;
import uk.ac.nott.cs.g53dia.library.IllegalActionException;
import uk.ac.nott.cs.g53dia.library.OutOfFuelException;
import uk.ac.nott.cs.g53dia.library.Tanker;
import uk.ac.nott.cs.g53dia.library.TankerViewer;

/**
 * An example of how to simulate execution of a tanker agent in the sample
 * (task) environment.
 * <p>
 * Creates a default {@link Environment}, a {@link DemoTanker} and a GUI window
 * (a {@link TankerViewer}) and executes the Tanker for DURATION days in the
 * environment.
 * 
 * @author Julian Zappala
 */

/*
 * Copyright (c) 2005 Neil Madden. Copyright (c) 2011 Julian Zappala
 * (jxz@cs.nott.ac.uk)
 * 
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

public class Simulator {

	private static boolean REGULAR = true;
	private static int LEVEL = 20;
	/**
	 * Time for which execution pauses so that GUI can update. Reducing this value
	 * causes the simulation to run faster.
	 */
	private static int DELAY = 50;

	/**
	 * Number of timesteps to execute.
	 */
	private static int DURATION = 10000;

	/**
	 * Whether the action attempted at the last timestep failed.
	 */
	private static boolean actionFailed = false;

	public static ArrayList<Integer> iterate(int level, ArrayList<Integer> saved) {
		if (level == 0) {
			return saved;
		}
		System.out.println("Running iteration " + level);
		// Note: to obtain reproducible behaviour, you can set the Random seed
//		Random r = new Random(10);
		Random r = new Random(level);
		// Create an environment
		Environment env = new Environment(Tanker.MAX_FUEL / 2, r);
		// Create a tanker
		Tanker tank = new IntelligentTanker(r);
		// Create a GUI window to show the tanker
//		TankerViewer tv = new TankerViewer(tank);
//		tv.setDefaultCloseOperation(javax.swing.JFrame.EXIT_ON_CLOSE);
		// Start executing the Tanker
		while (env.getTimestep() < DURATION) {
			// Advance the environment timestep
			env.tick();
			// Update the GUI
//					tv.tick(env);
			// Get the current view of the tanker
			Cell[][] view = env.getView(tank.getPosition(), Tanker.VIEW_RANGE);
			// Let the tanker choose an action
			Action act = tank.senseAndAct(view, actionFailed, env.getTimestep());
			// Try to execute the action
			try {
				actionFailed = act.execute(env, tank);
			} catch (OutOfFuelException ofe) {
				System.err.println(ofe.getMessage());
				System.exit(-1);
			} catch (IllegalActionException afe) {
				System.err.println(afe.getMessage());
				actionFailed = false;
			}
		}
		saved.add(tank.getScore());
		return iterate(level - 1, saved);
	}

	public static void regular(int seed, int delay) {
		// Note: to obtain reproducible behaviour, you can set the Random seed
		Random r = new Random(seed);
		// Create an environment
		Environment env = new Environment(Tanker.MAX_FUEL / 2, r);
		// Create a tanker
		Tanker tank = new IntelligentTanker(r);
		// Create a GUI window to show the tanker
		TankerViewer tv = new TankerViewer(tank);
		tv.setDefaultCloseOperation(javax.swing.JFrame.EXIT_ON_CLOSE);
		// Start executing the Tanker
		while (env.getTimestep() < DURATION) {
			// Advance the environment timestep
			env.tick();
			// Update the GUI
			tv.tick(env);
			// Get the current view of the tanker
			Cell[][] view = env.getView(tank.getPosition(), Tanker.VIEW_RANGE);
			// Let the tanker choose an action
			Action act = tank.senseAndAct(view, actionFailed, env.getTimestep());
			// Try to execute the action
			try {
				actionFailed = act.execute(env, tank);
			} catch (OutOfFuelException ofe) {
				System.err.println(ofe.getMessage());
				System.exit(-1);
			} catch (IllegalActionException afe) {
				System.err.println(afe.getMessage());
				actionFailed = false;
			}
			try {
				Thread.sleep(delay);
			} catch (Exception e) {
			}
		}
	}

	public static void main(String[] args) {
		if (REGULAR) {
			regular(9, DELAY);
		} else {
			ArrayList<Integer> results = Simulator.iterate(LEVEL, new ArrayList<>());
			float sum = 0;
			for (int score : results) {
				sum += score;
			}
			sum /= results.size();

			System.out.printf("Ran %d iterations. MIN: %d MAX: %d AVG: %f", LEVEL, Collections.min(results),
					Collections.max(results), sum);
		}
	}
}
