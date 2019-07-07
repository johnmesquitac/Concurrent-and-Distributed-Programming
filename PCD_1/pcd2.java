import java.lang.*;
import java.util.Random;

class RunThE implements Runnable
{
    private long N;
    private int MaxThreads;
    public double valor;
    private int inicio, fim;
    private double maior;
    private double PI;

    // construtor
    public RunThE(long aN, int aMaxThreads, int inicio, int fim, double PI, double maior[], double r1[], double r2[], double o1[], double o2[])
    {
        N = aN;
        MaxThreads = aMaxThreads;
        this.inicio = inicio;
        this.fim = fim;
        this.PI = PI;

    }

    public void run()
    {
        int k;

        for(k=inicio; k<fim; k++)
        {
            res[k] = Math.sqrt((Math.pow(r1[k],2) + Math.pow(r2[k],2)) - (2*r1[k]*r2[k]*Math.cos((o1[k]-o2[k])*PI/180)));

            if(maior[k] < res[k])
            {
                maior[k] = res[k];
            }

        }
    }
}

public class Euclidiana
{
    public static void main(String args[])
    {
        double PI = 3.14159265;
        int N = 10000000;
        int MaxThreads = 8, i;
        int controle = N/MaxThreads;
        Thread[] th;
        RunThE[] rh;
        double resfinal;
        double maior_total = 0;
        int inicio, fim;
        Random gerador = new Random();

        rh = new RunThE[MaxThreads];
        th = new Thread[MaxThreads];

        double r1[]  = new double[N];
        double r2[]  = new double[N];
        double o1[]  = new double[N];
        double o2[]  = new double[N];
        double maior[] = new double[MaxThreads];

        for(i=0; i<MaxThreads; i++)
        {
            double r1[]  = Math.random()*100;
            double r2[]  = Math.random()*100;
            double o1[]  = Math.random()*359;
            double o2[]  = Math.random()*359;
        }



        for(i=0; i<MaxThreads; i++)
        {
            inicio = i * controle;
            fim = (i+1) * controle;
            rh[i] = new RunThE(N, MaxThreads, inicio, fim, PI, maior, r1, r2, o1, o2);
            th[i] = new Thread(rh[i]);
            th[i].start();
        }
        try
        {
            for(i=0; i<MaxThreads; i++)
            {
                th[i].join();
                if(maior_total<maior[i])
                {
                    maior_total = maior[i];
                }
            }
        }
        catch (InterruptedException e)
        {
            System.out.println("Excecao");
        }
    }
}

