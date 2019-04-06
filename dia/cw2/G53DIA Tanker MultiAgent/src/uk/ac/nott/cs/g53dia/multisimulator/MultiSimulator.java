package uk.ac.nott.cs.g53dia.multisimulator;

import uk.ac.nott.cs.g53dia.multiagent.*;
import uk.ac.nott.cs.g53dia.multilibrary.*;

import java.util.Random;

/**
 * An example of how to simulate execution of a fleet of tanker agents in the sample
 * (task) environment.
 * <p>
 * Creates a default {@link Environment}, a {@link MyFleet} and a GUI window
 * (a {@link FleetViewer}) and executes the DemoFleet for DURATION days in the
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

public class MultiSimulator {

	/**
	 * Time for which execution pauses so that GUI can update. Reducing this
	 * value causes the simulation to run faster.
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

    public static void main(String[] args) {
    	// Note: to obtain reproducible behaviour, you can set the Random seed
    	Random r = new Random(55);
    	// Create an environment
    	Environment env = new Environment(Tanker.MAX_FUEL/2, r);
    	// Create a fleet
    	Fleet fleet = new MyFleet(r);
    	// Create a GUI window to show the fleet
    	FleetViewer fv = new FleetViewer(fleet);
    	// TMP
    	// 1 is the one that gets null ref
    	fv.setTanker(fleet.get(2));
    	
    	fv.setDefaultCloseOperation(javax.swing.JFrame.EXIT_ON_CLOSE);
    	// Start executing the tankers in the Fleet
    	while (env.getTimestep() < DURATION) {
    		// Advance the environment timestep
    		env.tick();
    		// Update the GUI
    		fv.tick(env);
    		for (Tanker t:fleet) {
    			// Get the current view of the tanker
    			Cell[][] view = env.getView(t.getPosition(), Tanker.VIEW_RANGE);
    			// Let the tanker choose an action
    			Action act = t.senseAndAct(view, actionFailed, env.getTimestep());
    			// Try to execute the action
    			try {
    				actionFailed = act.execute(env, t);
    			} catch (OutOfFuelException ofe) {
    				System.err.println(ofe.getMessage());
    				System.exit(-1);
    			} catch (IllegalActionException afe) {
    				System.err.println(afe.getMessage());
    				actionFailed = false;
    			}
    			try {
    				Thread.sleep(DELAY);
    			} catch (Exception e) { }
    		}
    	}
    	System.out.println("Simulation completed at timestep " + env.getTimestep() + " , score: " + fleet.getScore());
    }
}

