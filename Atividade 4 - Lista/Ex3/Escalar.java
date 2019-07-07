package Calc;
public class Escalar {
    public static void main(String args[]) {
        int N = 100000;
        int i, MAX_THREADS = 4;
        Thread[ ] thread_vet = new Thread[MAX_THREADS];
        Calc[] calc_vet = new Calc[ MAX_THREADS ];
        int[] A = new int[N];
        int[] B = new int[N];
        long start_time, final_time;
        for( i = 0; i < N; ++i ) {
            A[i] = (int)Math.round(Math.random() * 5 );
            B[i] = (int)Math.round(Math.random() * 5 );
        }
        start_time = System.currentTimeMillis( );
        for(i = 0; i < MAX_THREADS; i++) {
            calc_vet[i]= new Calc(A,B,N,MAX_THREADS,i+1);
            thread_vet[i] = new Thread( calc_vet[i] );
            thread_vet[i].start();
        }
        try {
            for(i = 0; i < MAX_THREADS;i++ ) {
                thread_vet[i].join( );
            }
        } catch( InterruptedException e ) {
            System.out.println( "Erro na thread:" + i );
        }
        final_time = System.currentTimeMillis( ) - start_time;
        System.out.println( "resultado:" + calc_vet[i-1].get_somatotal());
        System.out.println( "tempo paralelo" + final_time + "ms" );
    }
}