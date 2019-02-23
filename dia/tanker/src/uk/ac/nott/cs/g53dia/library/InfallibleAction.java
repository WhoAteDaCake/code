package uk.ac.nott.cs.g53dia.library;

/**
 * Abstract infallible action for actions that are guaranteed to succeed if their preconditions hold
 *
 * @author Brian Logan
 */
/*
 * Copyright (c) 2019 Brian Logan
 * 
 * See the file "license.terms" for information on usage and redistribution of
 * this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */
public abstract class InfallibleAction implements Action {
	
	/**
	 * Behaviour when the action succeeds.
	 * @param env The Environment that the Tanker inhabits.
	 * @param tanker The Tanker performing this action.
	 * @throws IllegalActionException The action couldn't be performed.
	 */
	public abstract void actionSucceeds(Environment env, Tanker tanker) 
	        throws IllegalActionException;
	
	public boolean execute(Environment env, Tanker tanker) 
	        throws IllegalActionException {

		actionSucceeds(env, tanker);
		return false;
	}
}
