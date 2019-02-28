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

	private static boolean REGULAR = false;
	private static int SEED = 20;
	private static int THREADS = 10;
	private static int LEVEL = 50;
	/**
	 * Time for which execution pauses so that GUI can update. Reducing this value
	 * causes the simulation to run faster.
	 */
	private static int DELAY = 5;
	/**
	 * Number of timesteps to execute.
	 */
	private static int DURATION = 10000;

	/**
	 * Whether the action attempted at the last timestep failed.
	 */
	private static boolean actionFailed = false;

	public static ArrayList<Integer> iterate(int level, int offset, ArrayList<Integer> saved) {
		if (level == 0) {
			return saved;
		}
		int actual = level + offset;

		System.out.println("Running iteration " + actual);

		Random r = new Random(actual);
		Environment env = new Environment(Tanker.MAX_FUEL / 2, r);
		Tanker tank = new IntelligentTanker(r);

		while (env.getTimestep() < DURATION) {
			// Advance the environment timestep
			env.tick();
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
		return iterate(level - 1, offset, saved);
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
			regular(SEED, DELAY);
		} else {
			ArrayList<Thread> threads = new ArrayList<>();
			ArrayList<Simulate> runners = new ArrayList<>();
			int batchSize = LEVEL / THREADS;
//
			for (int i = 0; i < THREADS; i += 1) {
				final int index = i;
				System.out.println("Batch: " + batchSize + " ,offset " + batchSize * index);
				final Simulate runner = new Simulate(index, batchSize);
				runners.add(runner);
				Thread t = new Thread(runner);
				runners.add(runner);
				threads.add(t);
				t.start();
			}
//
			ArrayList<Integer> results = new ArrayList<>();
			for (int i = 0; i < THREADS; i += 1) {
				try {
					threads.get(i).join();
					results.addAll(runners.get(i).getResults());
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

			float sum = 0;
			for (int score : results) {
				sum += score;
			}
			sum /= results.size();

			System.out.printf("Ran %d iterations. MIN: %d MAX: %d AVG: %f", results.size(), Collections.min(results),
					Collections.max(results), sum);
		}
	}

	public static class Simulate implements Runnable {
		private volatile ArrayList<Integer> results;
		private volatile int index;
		private volatile int batchSize;

		public Simulate(int i, int b) {
			index = i;
			batchSize = b;
		}

		@Override
		public void run() {
			results = Simulator.iterate(batchSize, batchSize * index, new ArrayList<>());
		}

		public ArrayList<Integer> getResults() {
			return results;
		}
	}
}
