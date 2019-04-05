package uk.ac.nott.cs.g53dia.multiagent;

public class Debug {
	public static boolean DEBUG = false;
	public static boolean ERROR = false;
	public static boolean WARN = false;

	
	public static void log(String str) {
		if (DEBUG) {
			System.out.println("DEBUG: " + str);
		}
	}
	
	public static void error(String str) {
		if (ERROR) {
			System.err.println("ERROR: " + str);
		}
	}
	
	public static void warn(String str) {
		if (WARN) {
			System.out.println("WARNING: " + str);
		}
	}
}
