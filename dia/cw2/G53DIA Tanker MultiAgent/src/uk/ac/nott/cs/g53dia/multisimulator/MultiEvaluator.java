// Written by Haoyang Wang
// Modified by nza, bsl, Simon Castle-Green

package uk.ac.nott.cs.g53dia.multisimulator;

import uk.ac.nott.cs.g53dia.multilibrary.*;
import uk.ac.nott.cs.g53dia.multiagent.*;
import java.text.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

public class MultiEvaluator {

	private static int DURATION = 10000;
	private static int THREADS = 5;
	
	private static int DELAY = 20;
	private static int SEED = 0;
	private static int LEVEL = 100;
	
	private static boolean actionFailed = false;
	
	public static ArrayList<Integer> iterate(int level, int offset, ArrayList<Integer> saved) {
		if (level == 0) {
			return saved;
		}
		int actual = SEED + level + offset;
		String error = "";
		System.out.println("Running iteration " + actual);

		Random r = new Random(actual);
		Environment env = new Environment(Tanker.MAX_FUEL / 2, r);
		Fleet fleet = new MyFleet(r);

		while (env.getTimestep() < DURATION) {
		// Advance the environment timestep
		env.tick();

		for (Tanker t: fleet) {
			// Get the current view of the tanker
			Cell[][] view = env.getView(t.getPosition(), Tanker.VIEW_RANGE);

			try {
				// Let the tanker choose an action
				Action act = t.senseAndAct(view, actionFailed, env.getTimestep());
				// Try to execute the action
				actionFailed = act.execute(env, t);

			} catch (OutOfFuelException ofe) {
				System.err.println(ofe.getMessage());
				System.err.println("Failed at level: " + actual + " timestep: " + env.getTimestep());
				Thread.currentThread().interrupt();
				return saved;
			} catch (IllegalActionException afe) {
				System.err.println(afe.getMessage());
				System.err.println("Failed at level: " + actual + " timestep: " + env.getTimestep());
				Thread.currentThread().interrupt();
				actionFailed = false;
				return saved;
			} catch (Exception e) {
				System.err.println(e.getMessage());
				System.err.println("Failed at level: " + actual + " timestep: " + env.getTimestep());
				Thread.currentThread().interrupt();
				return saved;
			}

		}
		}
		System.out.println("Score: " + fleet.getScore());

		saved.add((int)fleet.getScore());
		return iterate(level - 1, offset, saved);
	}
		
		public static void main(String[] args) {

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

	
	public static final class Simulate implements Runnable {
		private volatile ArrayList<Integer> results;
		private volatile int index;
		private volatile int batchSize;

		public Simulate(int i, int b) {
			index = i;
			batchSize = b;
		}

		@Override
		public void run() {
			results = MultiEvaluator.iterate(batchSize, batchSize * index, new ArrayList<>());
		}

		public ArrayList<Integer> getResults() {
			return results;
		}
	}
}
