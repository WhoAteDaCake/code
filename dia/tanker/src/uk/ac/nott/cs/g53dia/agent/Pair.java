package uk.ac.nott.cs.g53dia.agent;

public class Pair<K, V>  {
    public final K first;
    public final V second;

    public static <K, V> Pair<K, V> make(K first, V second) {
        return new Pair<K, V>(first, second);
    }

    public Pair(K first, V second) {
        this.first = first;
        this.second = second;
    }
}
