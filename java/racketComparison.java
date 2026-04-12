public class racketComparison {
    public static void main(String[] args) {
        int num[] = new int[] {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int sum = java.util.Arrays.stream(num).reduce(0, Integer::sum);
        System.out.println(sum);
    }
}
