package alg;

import java.util.*;

public class Main {
    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_GREEN = "\u001B[42m";
    public static final String ANSI_YELLOW = "\u001B[43m";
    private static int fin = 0;

    public static void main(String[] args) {
        Scanner s = new Scanner(System.in);
        Map<Shape, Integer> shapes = new HashMap<>();
        int shapeTypes;

        /* read rows and columns info */
        int rows = s.nextInt();
        int cols = s.nextInt();

        /* read the whole array */
        int[][] array = new int[rows][cols];
        readArr(rows, cols, array, s);
        //printArr(rows, cols, array);

        /* adding shapes */
        shapeTypes = s.nextInt();
        readShapes(shapes, s, shapeTypes);
        s.close();

        /* make summed area table */
        int[][] summed = summedArea(array, rows, cols);
        //printArr(rows, cols, summed);

        /* sort the shapes by their area */
        Map<Shape, Integer> sortedShapes = sortShapes(shapes);

        /* shapes to array */
        ArrayList<Shape> shapeList = new ArrayList<>();
        for (Map.Entry<Shape, Integer> e : sortedShapes.entrySet()) {
            for (int i = 0; i < e.getValue(); i++) {
                shapeList.add(e.getKey());
            }
        }

        //findSolution(array, shapeList, 0);
        findSolutionV2(array, sortedShapes, 0);
        System.out.print(fin);
    }

    private static void findSolutionV2(int[][] matrix, Map<Shape, Integer> shapes, int sum) {
        boolean solutionFlag = false;
        for (int row = 0; row < matrix.length; row++) {
            for (int col = 0; col < matrix[0].length; col++) {
                for (Map.Entry<Shape, Integer> type : shapes.entrySet()) {
                    if (type.getValue() != 0) {
                        Pos pos = new Pos(col, row);
                        if (checkPosition(pos, type.getKey(), matrix)) {
                            //int[][] copy = arrCopy(matrix);
                            int local = sum;
                            ShapeData sd = putShape(type.getKey(), matrix, pos);
                            local += sd.local_sum;
                            int curr = type.getValue();
                            type.setValue(curr - 1);
                            solutionFlag = true;
                            findSolutionV2(matrix, shapes, local);
                            // todo remove shape
                            removeShape(type.getKey(), matrix, pos, sd.shape_data);
                            type.setValue(curr);
                        }
                    }
                }
            }
        }
        if (!solutionFlag) {
            if (fin < sum) fin = sum;
            //printArr(matrix);
        }
    }

    private static boolean checkPosition(Pos pos, Shape nextShape, int[][] array) {

        // if I can't even put it -> then don't lol
        if (pos.y + nextShape.height > array.length ||
                pos.x + nextShape.width > array[0].length) {
            return false;
        }

        // if the position free to put
        for (int i = pos.y; i < pos.y + nextShape.height; i++) {
            for (int j = pos.x; j < pos.x + nextShape.width; j++) {
                if (array[i][j] == 0 || array[i][j] == -1) {
                    return false;
                }
            }
        }
        return true;
    }

    private static ShapeData putShape(Shape shape, int[][] array, Pos pos) {
        // put the shape
        int y_from = pos.y;
        int x_from = pos.x;
        int y_to = y_from + shape.height;
        int x_to = x_from + shape.width;

        int[][] shape_data = new int[shape.height + 2][shape.width + 2];
        int local_sum = 0;

        for (int y = y_from - 1, i = 0; y <= y_to; y++, i++) {
            for (int x = x_from - 1, j = 0; x <= x_to; x++, j++) {
                // If I exceed the boundaries of the matrix
                if (y < 0 || x < 0 || y == array.length || x == array[0].length) continue;
                // If I'm on the boundaries of the matrix
                shape_data[i][j] = array[y][x];
                if (y == y_from - 1 || x == x_from - 1 || x == x_to || y == y_to) {
                    array[y][x] = -1;
                } else {
                    // the shape element
                    local_sum += array[y][x];
                    array[y][x] = 0;
                }
            }
        }
        return new ShapeData(shape_data, local_sum);
    }

    private static void removeShape(Shape shape, int[][] array, Pos pos, int[][] shape_data) {
        // put the shape
        int y_from = pos.y;
        int x_from = pos.x;
        int y_to = y_from + shape.height;
        int x_to = x_from + shape.width;

        for (int y = y_from - 1, i = 0; y <= y_to; y++, i++) {
            for (int x = x_from - 1, j = 0; x <= x_to; x++, j++) {
                // If I exceed the boundaries of the matrix
                if (y < 0 || x < 0 || y == array.length || x == array[0].length) continue;
                // If I'm on the boundaries of the matrix
                    array[y][x] = shape_data[i][j];
            }
        }
    }

    private static Pos nextPos(int[][] array) {
        int row = array.length;
        int col = array[0].length;

        // diagonal matrix parsing
        for (int diag = 0; diag < 2 * row; diag++) {
            for (int a = 0; a < row; a++) {
                for (int b = 0; b < col; b++) {
                    if (diag == a + b) {
                        if (array[a][b] != 0 && array[a][b] != -1) {
                            // returns new suitable position
                            return new Pos(b, a);
                        }
                    }
                }
            }
        }
        return null;
    }

    private static Map<Shape, Integer> sortShapes(Map<Shape, Integer> shapes) {
        List<Shape> list = new ArrayList<>(shapes.keySet());
        list.sort(Comparator.comparingInt(o -> o.area));
        Collections.reverse(list);
        Map<Shape, Integer> sorted = new LinkedHashMap<>();
        for (Shape key : list) {
            sorted.put(key, shapes.get(key));
        }
        return sorted;
    }

    /* makes deep copy */
    private static int[][] arrCopy(int[][] arr) {
        int[][] copy = new int[arr.length][];
        for (int i = 0; i < arr.length; i++) {
            copy[i] = Arrays.copyOf(arr[i], arr[i].length);
        }
        return copy;
    }

    /* makes summed-area table
     * link: https://en.wikipedia.org/wiki/Summed-area_table
     * unfortunately useless :[
     */
    private static int[][] summedArea(int[][] array, int rows, int cols) {
        int[][] summed = new int[rows][cols];
        for (int x = 0; x < rows; x++) {
            for (int y = 0; y < cols; y++) {
                if (x > 0 && y > 0) {
                    summed[x][y] = array[x][y] + summed[x - 1][y] + summed[x][y - 1] - summed[x - 1][y - 1];
                } else if (x > 0) {
                    summed[x][y] = array[x][y] + summed[x - 1][y];
                } else if (y > 0) {
                    summed[x][y] = array[x][y] + summed[x][y - 1];
                } else {
                    summed[x][y] = array[x][y];
                }
            }
        }
        return summed;
    }

    /**/
    private static void readShapes(Map<Shape, Integer> shapes, Scanner s, int shapeTypes) {
        for (int i = 0; i < shapeTypes; i++) {
            int heigth = s.nextInt();
            int weigth = s.nextInt();
            int num = s.nextInt();
            shapes.put(new Shape(heigth, weigth), num);
        }
    }

    private static void readArr(int rows, int cols, int[][] array, Scanner s) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                array[i][j] = s.nextInt();
            }
        }
    }

    private static void printArr(int[][] array) {
        for (int i = 0; i < array.length; i++) {
            for (int j = 0; j < array[0].length; j++) {
                if (array[i][j] == 0) {
                    System.out.print(ANSI_GREEN + "O" + "  " + ANSI_RESET);
                } else if (array[i][j] == -1) {
                    System.out.print(ANSI_YELLOW + "X" + "  " + ANSI_RESET);
                } else {
                    System.out.print(array[i][j] + "  ");
                }
            }
            System.out.println();
        }
        System.out.println();
    }
}
