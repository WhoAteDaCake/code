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

		public int hashCode() {
			return first.hashCode() + second.hashCode();
		}

		public boolean equals(Object o) {
			if (this == o) {
				return true;
			} else if (o == null) {
				return false;
			} else if (this.getClass() != o.getClass()) {
				return false;
			}
			@SuppressWarnings("unchecked")
			Group2<K, V> g2 = (Group2<K, V>) o;
			return first.equals(g2.first) && second.equals(g2.second);
		}

		public String toString() {
			return String.format("(%s,%s)", first.toString(), second.toString());
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

		public Group2<K, V> head() {
			return new Group2<K, V>(this.first, this.second);
		}

		public Group2<V, S> tail() {
			return new Group2<V, S>(this.second, this.third);
		}

		public int hashCode() {
			return first.hashCode() + second.hashCode() + third.hashCode();
		}

		public boolean equals(Object o) {
			if (this == o) {
				return true;
			} else if (o == null) {
				return false;
			} else if (this.getClass() != o.getClass()) {
				return false;
			}
			@SuppressWarnings("unchecked")
			Group3<K, V, S> g = (Group3<K, V, S>) o;
			return first.equals(g.first) && second.equals(g.second) && third.equals(g.third);
		}

		public String toString() {
			return String.format("(%s,%s, %s)", first.toString(), second.toString(), third.toString());
		}
	}
}
