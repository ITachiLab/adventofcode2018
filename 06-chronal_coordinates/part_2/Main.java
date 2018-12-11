import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
    public static class Coords {
        int x;
        int y;
        int id;
        int size;

        Coords(int x, int y, int id) {
            this.x = x;
            this.y = y;
            this.id = id;
            this.size = 0;
        }

        int distanceTo(int x, int y) {
            return Math.abs(this.x - x) + Math.abs(this.y - y);
        }

        void addField() {
            this.size++;
        }
    }

    public static int[] getBounds(List<Coords> points) {
        final int[] bounds = new int[]{Integer.MAX_VALUE, 0, 0, Integer.MAX_VALUE};

        points.forEach(p -> {
            if (p.y < bounds[0]) {
                bounds[0] = p.y;
            }

            if (p.y > bounds[2]) {
                bounds[2] = p.y;
            }

            if (p.x > bounds[1]) {
                bounds[1] = p.x;
            }

            if (p.x < bounds[3]) {
                bounds[3] = p.x;
            }
        });

        return bounds;
    }

    public static void main(String[] args) {
        List<Coords> points = new ArrayList<>();

        try {
            FileReader fileReader = new FileReader("input.txt");
            BufferedReader reader = new BufferedReader(fileReader);
            Pattern pattern = Pattern.compile("^(\\d+), (\\d+)");
            final int[] id = {1};

            reader.lines().forEach(s -> {
                Matcher matcher = pattern.matcher(s);

                if (matcher.find()) {
                    points.add(new Coords(Integer.parseInt(matcher.group(1)), Integer.parseInt(matcher.group(2)), id[0]++));
                }
            });

            int[] bounds = getBounds(points);
            int regions = 0;

            for (int y = bounds[0]; y <= bounds[2]; y++) {
                for (int x = bounds[3]; x <= bounds[1]; x++) {
                    int distanceSum = 0;

                    for (Coords c : points) {
                        distanceSum += c.distanceTo(x, y);
                    }

                    if (distanceSum < 10000) {
                        regions++;
                    }
                }
            }

            System.out.println("Region size: " + regions);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }
    }
}
