package uk.ac.nott.cs.g53dia.multiagent;

public class Debug {
	public static boolean DEBUG = true;
	public static boolean ERROR = true;
	public static boolean WARN = true;

	
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
