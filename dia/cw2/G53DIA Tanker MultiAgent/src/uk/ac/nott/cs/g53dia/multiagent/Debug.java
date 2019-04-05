package uk.ac.nott.cs.g53dia.multiagent;

public class Debug {
	public static boolean DEBUG = false;
	
	
	public static void log(String str) {
		if (DEBUG) {
			System.out.println(str);
		}
	}
	
	public static void error(String str) {
		if (DEBUG) {
			System.err.println(str);
		}
	}
}
