//宿題2
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
using namespace std;
#define all(x) (x).begin(), (x).end()

int score(string word)
{
    int point = 0;
    for (int i = 0; i < word.size(); i++)
    {
        switch (word[i])
        {
        case 'a':
        case 'e':
        case 'h':
        case 'i':
        case 'n':
        case 'o':
        case 'r':
        case 's':
        case 't':
            point += 1;
            break;
        case 'c':
        case 'd':
        case 'l':
        case 'm':
        case 'u':
            point += 2;
            break;
        case 'b':
        case 'f':
        case 'g':
        case 'p':
        case 'v':
        case 'w':
        case 'y':
            point += 3;
            break;
        case 'j':
        case 'k':
        case 'q':
        case 'x':
        case 'z':
            point += 4;
            break;
        default:
            return -1;
            break;
        }
    }
    return point;
}

int main()
{
    FILE *fp_word;

    string buf;
    ifstream ifs;
    ofstream result;
    ifs.open("words.txt");
    result.open("result.txt", ios::out);
    //    result.open("small_result.txt", ios::out);
    //    result.open("medium_result.txt", ios::out);
    //    result.open("large_result.txt", ios::out);
    if (!ifs)
    {
        cerr << "ファイルを開けませんでした" << endl;
        return EXIT_FAILURE;
    }

    vector<string> dictionary(0);    //添え字が主キーになるようなイメージ
    vector<vector<int>> elements(0); //添え字でdictionaryと対応付ける感じ
    int dict_num = 0;
    while ((getline(ifs, buf)))
    {
        vector<int> alpha(26);
        for (int i = 0; i < buf.size(); i++)
        {
            alpha[buf[i] - 'a'] += 1;
        }
        /*ここで辞書を作る*/
        /*先頭だけでなく要素で見るから二分探索にはしない*/
        dictionary.push_back(buf);
        elements.push_back(alpha);
    }


    /*調べる単語のリスト*/
    queue<string> word_list;
    word_list.push("small.txt");
 //   word_list.push("medium.txt");
  //  word_list.push("large.txt");

    while (word_list.size() > 0) // queueが空になるまで
    {
        ifstream words;
        words.open(word_list.front());
        word_list.pop();
        if (!words)
        {
            cerr << "ファイルを開けませんでした" << endl;
            return EXIT_FAILURE;
        }


        /* リストから取得した文字のアナグラムを検索 */
        string input;
        int flug = 0;
        while ((getline(words, input)))
        {
            //flugで改行の管理(入力ファイルと行数を揃えるため)
            if (flug)
            {
                result << endl;
            }
            flug = 1;
            /* 入力ごとに設定するもの */
            int now_score = -1;
            int best_score = -1;
            //            vector<string> anagrams(0); //見つけたanagramを保存する配列(デバッグで使用)
            string best_word;
            //大文字を小文字にする
            transform(input.begin(), input.end(), input.begin(), ::tolower); // stdとグローバルにtolowerがあるから::をつけないと区別できない
//          cout << "[" << input << "]" << endl;
            sort(all(input));
            vector<int> input_word(26);
            for (int i = 0; i < input.size(); i++)
            {
                input_word[input[i] - 'a'] += 1;
            }
            int locate = -1; //見つかった時の場所を保存
            for (int i = 0; i < dictionary.size(); i++)
            { //線形探索
                bool can_make = true;
                for (int j = 0; j < 26; j++)
                {
                    if (input_word[j] < elements[i][j])
                    {
                        can_make = false;
                        break;
                    }
                }
                if (can_make)
                {
                    //                    anagrams.push_back(dictionary[i]); // 後で消す
                    locate = i;
                    now_score = score(dictionary[i]);
                    if (now_score > best_score)
                    {
                        best_score = now_score;
                        best_word = dictionary[locate];
                    }
                }
            }

            /*結果を出力*/
            /*
                        if (anagrams.size() > 0)
                        {
                            cout << "Found:";
                            for (int i = 0; i < anagrams.size(); i++)
                            {
                                cout << "\t"
                                     << "score:" << score(anagrams[i]);
                                cout << "\t" << anagrams[i] << endl;
                            }
                        }
                        if (anagrams.size() == 0)
                        {
                            cout << "Not Found!\n"
                                 << endl;
                        }
            */
            if (now_score >= 0)
            {
//                result << "word: " << best_word << " (score: " << best_score << ")" << endl;
                result << best_word; //endlはいらない
            }
            else
            {
                result << "Not Found." << endl;
            }
        }
    }
    cout << "Finished."<<endl;
    ifs.close();
    result.close();
    return 0;
}