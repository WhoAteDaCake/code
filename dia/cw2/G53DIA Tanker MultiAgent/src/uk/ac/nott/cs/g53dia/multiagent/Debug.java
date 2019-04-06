package uk.ac.nott.cs.g53dia.multiagent;

public class Debug {
	
	public static boolean DISABLE_ALL = true;
	public static boolean DEBUG = false;
	public static boolean ERROR = true;
	public static boolean WARN = true;

	
	public static void log(String str) {
		if (!DISABLE_ALL && DEBUG) {
			System.out.println("DEBUG: " + str);
		}
	}
	
	public static void error(String str) {
		if (!DISABLE_ALL && ERROR) {
			System.err.println("ERROR: " + str);
		}
	}
	
	public static void warn(String str) {
		if (!DISABLE_ALL && WARN) {
			System.out.println("WARNING: " + str);
		}
	}
}
