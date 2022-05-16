//宿題1
// NxN行列 C++で計測
// 時間計測の参考 http://www.sanko-shoko.net/note.php?id=rnfd

#include <iostream>
#include <chrono> //時間計測に使用
#include <vector>

int main()
{
    int N;
    std::cin >> N;
    std::chrono::system_clock::time_point start, end;
    std::vector<std::vector<int> > A(N, std::vector<int>(N));
    std::vector<std::vector<int> > B(N, std::vector<int>(N));
    std::vector<std::vector<int> > C(N, std::vector<int>(N, 0));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // A,Bの値を適当に設定
            A[i][j] = i + j;
            B[i][j] = i + j * 2;
        }
    }

    int test_kaisuu = 10; // test_kaisuu回計測して割る

    //計測開始
    start = std::chrono::system_clock::now();
    for (int i = 0; i < test_kaisuu; i++)
    {
        //掛け算をする
        // O(N^3)
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                int c = 0;
                // i行目j列目の値を求める
                for (int p = 0; p < N; p++)
                {
                    c += (A[i][p] * B[p][j]);
                }
                C[i][j] = c; 
            }
        }
    }

    //ここまで計測
    end = std::chrono::system_clock::now();

    //各行列の出力 要らないかも
    /*
    std::cout << "A" << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            std::cout << A[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "B" << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            std::cout << B[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "C" << std::endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            std::cout << C[i][j] << " ";
        }
        std::cout << std::endl;
    }
    */
    printf("%dx%d行列の掛け算 (%d回)\n", N, N, test_kaisuu);
    double time = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.0);
    printf("time %lf[ms]\n", time);
    printf("time %.6lf[s]\n", time / 1000);

    return 0;
}