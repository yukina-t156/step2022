/*方法2 anagram用の辞書を作る*/
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
#define all(x) (x).begin(), (x).end()
typedef pair<string, string> P;

int main()
{
    FILE *fp_word;

    string buf;
    ifstream ifs;
    ifs.open("words.txt");
    if (!ifs)
    {
        cerr << "ファイルを開けませんでした" << endl;
        return EXIT_FAILURE;
    }

    vector<P> dictionary(0);
    while ((getline(ifs, buf)))
    {
        string sorted = buf;
        sort(all(sorted));
        /*ここで辞書を作る*/
        dictionary.push_back(make_pair(sorted, buf));
    }
    sort(all(dictionary));

    cout << "\nExit: please input \"0\"\n"
         << endl;
    do
    {
        /*入力文字を検索*/
        cout << ">> ";
        string input;
        cin >> input;
        if (input == "0")
        {
            cout << "Bye :)" << endl;
            break;
        }
        if (input == "")
        {
            cout << "Please input a word!" << endl;
            break;
        }
        //大文字を小文字にする
        transform(input.begin(), input.end(), input.begin(), ::tolower); // stdとグローバルにtolowerがあるから::をつけないと区別できない
        cout << "[" << input << "]" << endl;

        sort(all(input));
        vector<string> anagrams(0); //見つけたanagramを保存する配列
        /*二分探索*/
        int left = 0, right = dictionary.size(), mid;
        int locate = -1; //見つかった時の場所を保存
        while (left < right)
        {
            // cout << left << " " << right << endl;
            mid = (left + right) / 2;
            if (input > dictionary[mid].first)
                left = mid + 1;
            else if (input < dictionary[mid].first)
                right = mid;
            else
            {
                anagrams.push_back(dictionary[mid].second);
                locate = mid;
                break;
            }
        }

        /*一致する時に前後を調べる*/
        if (locate >= 0)
        {
            int before = locate - 1, after = locate + 1;
            while (before >= 0)
            {
                if (dictionary[before].first == input)
                    anagrams.push_back(dictionary[before].second);
                else
                    break;
                before--;
            }
            while (after < dictionary.size())
            {
                if (dictionary[after].first == input)
                {
                    anagrams.push_back(dictionary[after].second);
                }
                else
                    break;
                after++;
            }
        }

        /*結果を出力*/
        if (anagrams.size() > 0)
        {
            cout << "Found:";
            for (int i = 0; i < anagrams.size(); i++)
            {
                cout << "\t" << anagrams[i] << endl;
            }
        }
        if (anagrams.size() == 0)
        {
            cout << "Not Found!\n"
                 << endl;
        }

    } while (1);

    ifs.close();
    return 0;
}