package uk.ac.nott.cs.g53dia.agent;

public class Group {

	public static <K, V> Group2<K, V> make2(K first, V second) {
		return new Group2<K, V>(first, second);
	}

	public static <K, V, S> Group3<K, V, S> make3(K first, V second, S third) {
		return new Group3<K, V, S>(first, second, third);
	}

	static public class Group2<K, V> {
		public final K first;
		public final V second;

		public Group2(K first, V second) {
			this.first = first;
			this.second = second;
		}
	}

	static public class Group3<K, V, S> {
		public final K first;
		public final V second;
		public final S third;

		public static <K, V, S> Group3<K, V, S> from2(K first, Group2<V, S> g2) {
			return new Group3<K, V, S>(first, g2.first, g2.second);
		}

		public Group3(K first, V second, S third) {
			this.first = first;
			this.second = second;
			this.third = third;
		}
	}

}
