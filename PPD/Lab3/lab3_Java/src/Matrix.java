import java.util.Random;

public class Matrix extends Thread
{
    private int rowNo;
    private int columnNo;
    private int[][] values;

    public Matrix(int rowNo,int columnNo, boolean nonzero){
        this.rowNo = rowNo;
        this.columnNo = columnNo;
        this.values = new int[this.rowNo][this.columnNo];

        if(nonzero){
            Random random = new Random();
            for(int i = 0; i < rowNo; i++)
                for(int j = 0; j < columnNo; j++)
                    values[i][j] = random.nextInt(10);
        }
    }

    public int getRowNo() {
        return rowNo;
    }

    public int getColumnNo() {
        return columnNo;
    }

    public int[][] getValues() {
        return values;
    }

    public void setValues(int i, int j, int value) {
        this.values[i][j] += value;
    }

    public void sum(int i, int j, Matrix a, Matrix b) {

        this.values[i][j] += a.values[i][j] + b.values[i][j];
    }

    public Integer sumForFuture(int i, int j, Matrix a, Matrix b) {

        return a.values[i][j] + b.values[i][j];
    }

    public void prod(int i, int j, Matrix a, Matrix b) {

        for (int k = 0; k < a.getColumnNo(); k++) // aColumn
            this.values[i][j] += a.values[i][k] * b.values[k][j];
    }

    public Integer prodForFuture(int i, int j, Matrix a, Matrix b) {

        Integer result = 0;
        for (int k = 0; k < a.getColumnNo(); k++) // aColumn
            result += a.values[i][k] * b.values[k][j];
        return result;
    }

}
