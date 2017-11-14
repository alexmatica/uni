import java.util.concurrent.Callable;

public class WorkerCallable implements Callable<Integer>{
    private Matrix matrix1;
    private Matrix matrix2;
    private Tuple indexes;
    private boolean doSum;

    public WorkerCallable(Matrix matrix1, Matrix matrix2, int i,int j, boolean sum) {
        this.matrix1 = matrix1;
        this.matrix2 = matrix2;
        this.indexes = new Tuple(i,j);
        this.doSum = sum;
    }

    @Override
    public Integer call(){
//        long thrId = Thread.currentThread().getId();
//        System.out.println("Thread " + thrId + "started");
        if (this.doSum)
            return matrix1.sumForFuture(indexes.getI(),indexes.getJ(),matrix1,matrix2);
        else
            return matrix1.prodForFuture(indexes.getI(),indexes.getJ(),matrix1,matrix2);
    }
}
