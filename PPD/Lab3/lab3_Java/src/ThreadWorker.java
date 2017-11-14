import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class ThreadWorker extends Thread {

    private Matrix matrix1;
    private Matrix matrix2;
    private Matrix result;
    private List<Tuple> indexes;
    private boolean doSum;

    public ThreadWorker(Matrix matrix1, Matrix matrix2, Matrix result, boolean doSum) {
        this.matrix1 = matrix1;
        this.matrix2 = matrix2;
        this.result = result;
        this.indexes = new ArrayList<>();
        this.doSum = doSum;
    }

    public void addIndexes(int i, int j)
    {
        indexes.add(new Tuple(i,j));
    }

    @Override
    public void run(){
        if (doSum)
            for (Tuple i: indexes)
                result.sum(i.getI(),i.getJ(),matrix1,matrix2);
        else
            for (Tuple i: indexes)
                result.prod(i.getI(),i.getJ(),matrix1,matrix2);
    }
}
