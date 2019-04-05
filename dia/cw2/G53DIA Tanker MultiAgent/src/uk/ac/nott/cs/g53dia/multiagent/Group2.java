package uk.ac.nott.cs.g53dia.multiagent;

public class Group2<K, V> {
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