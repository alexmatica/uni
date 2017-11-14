public class ExecutorWorker implements Runnable{

    private Matrix matrix1;
    private Matrix matrix2;
    private Tuple indexes;
    private Matrix result;
    private boolean doSum;

    public ExecutorWorker(Matrix matrix1, Matrix matrix2, int i, int j, Matrix result, boolean doSum) {
        this.matrix1 = matrix1;
        this.matrix2 = matrix2;
        this.indexes = new Tuple(i,j);
        this.result = result;
        this.doSum  = doSum;
    }


    @Override
    public void run() {
//        long thrId = Thread.currentThread().getId();
//        System.out.println("Thread " + thrId + "started");
        if(doSum)
            result.sum(indexes.getI(),indexes.getJ(),matrix1,matrix2);
        else
            result.prod(indexes.getI(),indexes.getJ(),matrix1,matrix2);
    }
}
