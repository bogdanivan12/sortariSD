#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <vector>

using namespace std;

vector <unsigned long long> aux;

void merge( vector <unsigned long long> &v, int st, int dr )
{
    aux.resize( dr - st + 1 );
    int mij = ( st + dr ) / 2, k = 0;
    int i = st, j = mij + 1;
    while( i <= mij && j <= dr )
        if( v[i] <= v[j] )
            aux[k++] = v[i++];
        else
            aux[k++] = v[j++];
    while( i <= mij )
        aux[k++] = v[i++];
    while( j <= dr )
        aux[k++] = v[j++];
    for( int i = st; i <= dr; ++i )
        v[i] = aux[ i - st ];
}

void mergeSort( vector <unsigned long long> &v, int st, int dr )
{
    if( st >= dr )
        return;
    int mij = ( st + dr ) / 2;
    mergeSort( v, st, mij );
    mergeSort( v, mij + 1, dr );
    merge( v, st, dr );
}

void shellSort( vector <unsigned long long> &v )
{
    int n = v.size(), pas = 0;
    for( int i = n / 2; i > 0; i /= 2 )
        for( int j = i; j < n; ++j )
            if( v[j] < v[ j - i ] )
            {
                swap( v[j], v[ j - i ] );
                if( j - i >= i )
                {
                    pas += i;
                    j -= i + 1;
                }
                else if( pas )
                {
                    j += pas - 1;
                    pas = 0;
                }
            }
            else if( pas )
            {
                j += pas;
                pas = 0;
            }
}

void afisare( vector <unsigned long long> v )
{
    int n = v.size();
    for( int i = 0; i < n; ++i )
        cout << v[i] << " ";
    cout << endl;
}

bool sortat( vector <unsigned long long> v )
{
    int n = v.size();
    bool ok = true;
    for( int i = 1; i < n && ok; ++i )
        if( v[ i - 1 ] > v[i] )
            ok = false;
    return ok;
}

void radixSort( vector <unsigned long long> &v, int baza, unsigned long long putere )
{
    vector <unsigned long long> cifre[baza];
    int n = v.size();
    for( int i = 0; i < n; ++i )
        cifre[ v[i] % putere / ( putere / baza ) ].push_back( v[i] );
    int k = 0;
    for( int i = 0; i < baza; ++i )
    {
        int m = cifre[i].size();
        for( int j = 0; j < m; ++j )
            v[k++] = cifre[i][j];
    }
    if( !sortat(v) )
        radixSort( v, baza, putere * baza );
}

void countingSort( vector <unsigned long long> &v, unsigned long long nmax )
{
    int n = v.size();
    aux.resize( nmax + 1 );
    for( int i = 0; i < n; ++i )
        ++aux[ v[i] ];
    int k = 0;
    for( int i = 0; i <= nmax; ++i )
        for( int j = 0; j < aux[i]; ++j )
            v[k++] = i;
}

void insertionSort( vector <unsigned long long> &v, int st, int dr )
{
    for( int i = st + 1; i <= dr; ++i )
    {
        long long aux = v[i];
        int j;
        for( j = i - 1; j >= st && v[j] > aux; --j )
            v[j + 1] = v[j];
        v[j + 1] = aux;
    }
}

void timSort( vector <unsigned long long> &v, int st, int dr )
{
    if( dr - st < 32 )
    {
        insertionSort( v, st, dr );
        return;
    }
    int mij = ( st + dr ) / 2;
    timSort( v, st, mij );
    timSort( v, mij + 1, dr );
    merge( v, st, dr );
}

int main()
{
    ifstream f("date.in");
    ofstream g("date.out");
    int teste;
    f >> teste;
    for( int t = 0; t < teste; ++t )
    {
        unsigned long long n, nmax;
        f >> n >> nmax;
        // generare test
        vector < unsigned long long > v, cop;
        v.resize( n );
        srand( time( 0 ) );
        for( int i = 0; i < n; ++i )
        {
            v[i] = rand();
            v[i] = v[i] << 32;
            v[i] += rand();
            v[i] %= nmax + 1;
        }

        //testare sortari
        g << "Testul " << t + 1 << ":\nn = " << n << ", nmax = " << nmax << endl << endl;

        g << "TimSort: " << endl;
        cop = v;
        auto start = chrono::steady_clock::now();
        timSort( cop, 0, n - 1 );
        auto stop = chrono::steady_clock::now();
        g << chrono::duration_cast<chrono::milliseconds>(stop - start).count() / 1000.0 << 's' << endl;
        g << sortat( cop ) << endl << endl;
        aux.clear();

        g << "MergeSort: " << endl;
        cop = v;
        start = chrono::steady_clock::now();
        mergeSort( cop, 0, n - 1 );
        stop = chrono::steady_clock::now();
        g << chrono::duration_cast<chrono::milliseconds>(stop - start).count() / 1000.0 << 's' << endl;
        g << sortat( cop ) << endl << endl;
        aux.clear();

        g << "ShellSort: " << endl;
        cop = v;
        start = chrono::steady_clock::now();
        shellSort( cop );
        stop = chrono::steady_clock::now();
        g << chrono::duration_cast<chrono::milliseconds>(stop - start).count() / 1000.0 << 's' << endl;
        g << sortat( cop ) << endl << endl;

        g << "RadixSort (Baza 2): " << endl;
        cop = v;
        start = chrono::steady_clock::now();
        radixSort( cop, 2, 2 );
        stop = chrono::steady_clock::now();
        g << chrono::duration_cast<chrono::milliseconds>(stop - start).count() / 1000.0 << 's' << endl;
        g << sortat( cop ) << endl << endl;

        g << "RadixSort (Baza 10): " << endl;
        cop = v;
        start = chrono::steady_clock::now();
        radixSort( cop, 10, 10 );
        stop = chrono::steady_clock::now();
        g << chrono::duration_cast<chrono::milliseconds>(stop - start).count() / 1000.0 << 's' << endl;
        g << sortat( cop ) << endl << endl;

        g << "RadixSort (Baza 16): " << endl;
        cop = v;
        start = chrono::steady_clock::now();
        radixSort( cop, 16, 16 );
        stop = chrono::steady_clock::now();
        g << chrono::duration_cast<chrono::milliseconds>(stop - start).count() / 1000.0 << 's' << endl;
        g << sortat( cop ) << endl << endl;

        g << "RadixSort (Baza 2^16): " << endl;
        cop = v;
        start = chrono::steady_clock::now();
        radixSort( cop, 1 << 16, 1 << 16 );
        stop = chrono::steady_clock::now();
        g << chrono::duration_cast<chrono::milliseconds>(stop - start).count() / 1000.0 << 's' << endl;
        g << sortat( cop ) << endl << endl;

        g << "CountingSort: " << endl;
        cop = v;
        start = chrono::steady_clock::now();
        countingSort( cop, nmax );
        stop = chrono::steady_clock::now();
        g << chrono::duration_cast<chrono::milliseconds>(stop - start).count() / 1000.0 << 's' << endl;
        g << sortat( cop ) << endl << endl;
        aux.clear();

        g << "-------------------------------------------" << endl << endl;
    }
    f.close();
    g.close();
    return 0;
}