import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.*;

public class Main {
    private static int NR_OF_THREADS = 1;
    private static ExecutorService executorService = Executors.newFixedThreadPool(NR_OF_THREADS);

    private static Matrix matrix1;
    private static Matrix matrix2;
    private static boolean doSum = false;
    private static int N = 10;

    public static void main(String[] args) {

        String mode = "future";
        matrix1 = new Matrix(N, N, true);
        matrix2 = new Matrix(N, N, true);
//
//        printMatrix(matrix1.getRowNo(),matrix1.getColumnNo(),matrix1);
//        System.out.println();
//        printMatrix(matrix2.getRowNo(),matrix2.getColumnNo(),matrix2);

        Matrix result = new Matrix(matrix1.getRowNo(), matrix1.getColumnNo(), false);
        long startTime = System.currentTimeMillis();
        if (mode.equals("future") && doSum) {
            if(sumValidation()) {
                try {
                    result = execWithFutures(result);
                } catch (InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                }
            }
            else
                return;
        }
        else if(mode.equals("future") && !doSum) {
            if (prodValidation()) {
                try {
                    result = execWithFutures(result);
                } catch (InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                }
            } else
                return;
        }

        else if (mode.equals("threadpool") && doSum) {
            if (sumValidation()) {
                try {
                    result = execWithThreadPool(result);
                } catch (InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                }
            } else
                return;
        }
        else if (mode.equals("threadpool") && !doSum) {
            if (prodValidation()) {
                try {
                    result = execWithThreadPool(result);
                } catch (InterruptedException | ExecutionException e) {
                    e.printStackTrace();
                }
            } else
                return;
        }

        long endTime = System.currentTimeMillis();
//        System.out.println();
//        printMatrix(result.getRowNo(),result.getColumnNo(),result);
        System.out.println();
        double time = (endTime - startTime);
        System.out.println("Execution time: " + time + " milliseconds");
    }

    private static void printMatrix(int x, int y, Matrix a) {
        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++)
                System.out.print(a.getValues()[i][j] + " ");
            System.out.println();
        }
    }

    private static boolean sumValidation() {
        if (matrix1.getValues().length != matrix2.getValues().length) {
            System.out.println("Sum validation failed! Not the same number of lines in the matrices!");
            return false;
        } else
            for (int i = 0; i < matrix1.getRowNo(); i++)
                if (matrix1.getValues()[i].length != matrix2.getValues()[i].length) {
                    System.out.println("Sum validation failed! Not the same number of columns in the matrices");
                    return false;
                }
                return true;
    }

    private static boolean prodValidation() {
        if (matrix1.getColumnNo() != matrix2.getRowNo()) {
            System.out.println("Product validation failed! Number of columns in A must equal the number of rows in B!");
            return false;
        }
        return true;
    }

    public static Matrix execThreaadsSimple(Matrix result) throws InterruptedException, ExecutionException {
        List<ThreadWorker> threadList = new ArrayList<>();
        ThreadWorker[] threads = new ThreadWorker[NR_OF_THREADS];

        for (int i = 0; i < NR_OF_THREADS; i++) {
            threads[i] = new ThreadWorker(matrix1, matrix2, result, doSum);
        }

        int crtThreadAssigned = 0;
        for (int i = 0; i < matrix1.getRowNo(); i++)
            for (int j = 0; j < matrix2.getColumnNo(); j++) {
                threads[crtThreadAssigned].addIndexes(i, j);
                crtThreadAssigned++;
                if (crtThreadAssigned == NR_OF_THREADS)
                    crtThreadAssigned = 0;
            }

        for (Thread thread : threads)
            thread.start();

        for (Thread thread : threads)
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (RuntimeException e) {
                System.out.println(e.getMessage());
            }
        return result;
    }

    private static Matrix execWithFutures(Matrix result) throws InterruptedException, ExecutionException {
        List<WorkerCallable> threads = new ArrayList<>();

        for (int i = 0; i < matrix1.getRowNo(); i++)
            for (int j = 0; j < matrix2.getColumnNo(); j++) {
                threads.add(new WorkerCallable(matrix1, matrix2, i, j, doSum));
            }
        List<Future<Integer>> futures = executorService.invokeAll(threads);

        for (int i = 0; i < matrix1.getRowNo(); i++)
            for (int j = 0; j < matrix2.getColumnNo(); j++)
                result.setValues(i, j, futures.get(i * matrix1.getRowNo() + j).get());

        executorService.shutdown();
        return result;
    }

    private static Matrix execWithThreadPool(Matrix result) throws InterruptedException, ExecutionException {
        List<ExecutorWorker> threads = new ArrayList<>();

        for (int i = 0; i < matrix1.getRowNo(); i++)
            for (int j = 0; j < matrix2.getColumnNo(); j++) {
                threads.add(new ExecutorWorker(matrix1, matrix2, i, j, result, doSum));
            }

        for (ExecutorWorker worker : threads)
            executorService.execute(worker);

        executorService.shutdown();

        try {
            executorService.awaitTermination(3000, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        return result;
    }

}

