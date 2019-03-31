// Written by Haoyang Wang
// Modified by nza, bsl, Simon Castle-Green

package uk.ac.nott.cs.g53dia.multisimulator;

import uk.ac.nott.cs.g53dia.multilibrary.*;
import uk.ac.nott.cs.g53dia.multiagent.*;
import java.text.*;
import java.util.Random;

public class MultiEvaluator {

	private static int DURATION = 10000;
	private static int NRUNS = 20;
	private static int SEED = 55;

	private static boolean actionFailed = false;

	public static void main(String[] args) {

		long score = 0;

		DecimalFormat df = new DecimalFormat("0.000E00");
		Boolean actionFailed = false;

		// run the fleet for 10 times and compute the average score per agent
		for (int i = 0; i < NRUNS; i++) {
			String error = "";
			
			Random r = new Random((long) i + SEED);
			// Create an environment
			Environment env = new Environment(Tanker.MAX_FUEL/2, r);
			// Create a fleet
			Fleet fleet = new MyFleet(r);
			// Start executing the tankers in the Fleet
			run:
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
							error = " " + ofe.getMessage() + " at timestep " + env.getTimestep();
							break run;

						} catch (IllegalActionException afe) {
							System.err.println(afe.getMessage());
							actionFailed = false;

						} catch (Exception e) {
							System.err.println(e.getMessage());
							error = " " + e.getMessage() + " at timestep " + env.getTimestep();
							break run;
						}

					}
				}

			System.out.println("Run: " + i +
					" score per agent: " + fleet.getScore() + error);
			score = score + fleet.getScore();

		}
//		System.out.println("\nTotal average score per agent: "
//				+ df.format(score / NRUNS));
		System.out.println("\nTotal average score per agent: "
				+ score / NRUNS);
	}
}
