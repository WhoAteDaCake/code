package uk.ac.nott.cs.g53dia.library;

/**
 * Abstract fallible action for actions that may fail, even if their preconditions hold
 *
 * @author Brian Logan
 */
/*
 * Copyright (c) 2017 Brian Logan
 * 
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */
public abstract class FallibleAction implements Action {
	
	/**
	 * Probability that a fallible action will fail
	 */
	final static double ACTION_FAILURE_PROBABILITY = 0.5;

	/**
	 * Behaviour when the action succeeds.
	 * @param env The Environment that the Tanker inhabits.
	 * @param tanker The Tanker performing this action.
	 * @throws IllegalActionException The action couldn't be performed.
	 */
	public abstract void actionSucceeds(Environment env, Tanker tanker) 
	        throws IllegalActionException;

	/**
	 * Behaviour when the action fails.
	 * @param env The Environment that the Tanker inhabits.
	 * @param tanker The Tanker performing this action.
	 * @throws IllegalActionException The action couldn't be performed.
	 */
	public abstract void actionFails(Environment env, Tanker tanker) 
	        throws IllegalActionException;


	public boolean execute(Environment env, Tanker tanker) 
	        throws IllegalActionException {

		if (tanker.r.nextDouble() < ACTION_FAILURE_PROBABILITY) {
			actionFails(env, tanker);
			return true;
		} else {
			actionSucceeds(env, tanker);
			return false;
		}
	}
}
