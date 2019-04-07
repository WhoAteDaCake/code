package uk.ac.nott.cs.g53dia.multiagent;

public class Pair<K, V> {
	public final K first;
	public final V second;

	public Pair(K first, V second) {
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
		Pair<K, V> g2 = (Pair<K, V>) o;
		return first.equals(g2.first) && second.equals(g2.second);
	}

	public String toString() {
		return String.format("(%s,%s)", first.toString(), second.toString());
	}
}