package Calc;
class Calc extends Thread {
    private int start, end, thread_id, DIV;
    public int N;
    public int[] A, B;
    private int somaparcial;
    private static int somatotal;
    public Calc(int[] A, int[] B, int N , int N_THREADS, int thread_id ) {
        this.N = N;
        this.A = A;
        this.B = B;
        this.thread_id = thread_id;
        DIV = N/N_THREADS;
        if(thread_id == N_THREADS) {
            start = ( N_THREADS - 1 ) * DIV;
            end = N_THREADS * DIV;}
        else if((thread_id < N_THREADS) && (thread_id>1)) {
            start = (thread_id - 1 )* DIV;
            end = thread_id * DIV;}
        else {
            start = 0;
            end = DIV;}
        somaparcial = 0;
        somatotal = 0;}
    public void run() {
        for( int id_thread = start; id_thread != end; ++id_thread)
            somaparcial += A[id_thread] * A[id_thread];
        evaluate();
    }
    public int get_somatotal() {
        return somatotal;
    }
    public int get_somaparcial() {
        return this.somaparcial;
    }
    public int get_thread_id() {
        return this.thread_id;
    }
    public synchronized void evaluate() {
        somatotal += get_somaparcial();
    }
}



