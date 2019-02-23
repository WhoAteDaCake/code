package uk.ac.nott.cs.g53dia.library;
/**
 * Exception thrown to signal that a tanker runs out of fuel.
 * 
 * @author Neil Madden
 */

/*
 * 
 * Copyright (c) 2011 Julian Zappala (jxz@cs.nott.ac.uk)
 * 
 * See the file "license.terms" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 */

public class OutOfFuelException extends IllegalActionException {

    /**
     * 
     */
    private static final long serialVersionUID = -6629098452878508894L;

    public OutOfFuelException(String reason) {
        super(reason);
    }
}
