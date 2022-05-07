/*方法1 : 二分探索で全通り試す*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
#define all(x) (x).begin(), (x).end()

int main()
{
    FILE *fp_word;

    string buf;
    ifstream ifs;
    ifs.open("word.txt");
    if (!ifs)
    {
        cerr << "ファイルを開けませんでした" << endl;
        return EXIT_FAILURE;
    }

    vector<string> dictionary(0);
    while ((getline(ifs, buf)))
    {
        dictionary.push_back(buf);
    }
    cout << "\nExit: please input \"0\"\n"
         << endl;
    while (1)
    {
        /*入力文字を検索*/
        cout << ">> ";
        string input;
        cin >> input;
        if (input == "0" || input == "")
        {
            cout << "Bye:)" << endl;
            break;
        }
        //大文字を小文字にする
        transform(input.begin(), input.end(), input.begin(), ::tolower); // stdとグローバルにtolowerがあるから::をつけないと区別できない
        cout << "[" << input << "]" << endl;

        sort(all(input));           // next_permutationに必要
        vector<string> anagrams(0); //見つけたanagramを保存する配列
        do
        {
            /*二分探索*/
            int left = 0, right = dictionary.size(), mid;
            while (left < right)
            {
                mid = (left + right) / 2;
                if (dictionary[mid] == input)
                {
                    anagrams.push_back(input);
                    break;
                }
                else if (input < dictionary[mid])
                    right = mid;
                else
                    left = mid + 1;
            }
        } while (next_permutation(all(input)));

        /*結果を出力*/
        if (anagrams.size() == 0)
        {
            cout << "Not Found!\n"
                 << endl;
        }
        if (anagrams.size() > 0)
        {
            cout << "Found:";
            for (int i = 0; i < anagrams.size(); i++)
            {
                cout << "\t" << anagrams[i] << endl;
            }
        }
    }

    ifs.close();
    return 0;
}