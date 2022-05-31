// homework4_1 BFS
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <queue> //queueを使用

//回数のランキングを書き出す関数
void show_ranking(std::pair<std::string, std::string> &start, std::pair<std::string, std::string> &goal,
std::map<std::string, std::string> &pages, std::map<std::string, long long> &checked)
{
    std::priority_queue<std::pair<long long, std::string>,
                        std::vector<std::pair<long long, std::string> >,
                        std::less<std::pair<long long, std::string> > > fleq;
    // 値が大きい順に取り出せるpriority_que
    std::cout << "\"rank.txt\"に出力します" << std::endl;
    FILE *fp_output;
    std::ofstream ofs;
    ofs.open("rank.txt"); //書き込み用のファイルを開く
    if (!ofs)
    {
        //エラー処理
        std::cerr << "Error: 出力用ファイルが開けませんでした" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "出力中..." << std::endl;
    long long counter = 0;
    long long sum = 0;
    for (auto iter = checked.begin(); iter != checked.end(); iter++)//mapの全要素を取り出す
    {
        // first: key, second: value
        if (iter->second != 0)//回数が1回以上の要素のみ
        {
            fleq.push(std::make_pair(iter->second, iter->first)); //priority_queに(参照回数,ID)で格納
            sum += iter->second; // 合計に加える
            counter++; // カウンタを増やす
        }
    }

    ofs << "start: " << start.second << " (" << start.first << ")" << std::endl;
    ofs << "goal: " << goal.second << " (" << goal.first << ")" << std::endl;
    ofs << "Ave: " << float(sum) / float(counter) << " (" << counter << "pages)"<< std::endl; //平均回数とページ数
    ofs << "______________________" << std::endl;
    while (fleq.size() != 0) //priority_queを全て取り出す
    {
        //回数が大きい順に取り出し出力
        auto tmp = fleq.top();
        fleq.pop(); //忘れずpopする
        if (tmp.first != 0)
        {
            ofs << tmp.first << "回 :" << pages[tmp.second] << " (" << tmp.second << ")" << std::endl;
        }
    }
    std::cout << "出力が完了しました。" << std::endl;
    ofs.close();
}

//経路を書き出す関数
void show_route(std::pair<std::string, std::string> &start, std::pair<std::string, std::string> &goal,
                std::map<std::string, std::pair<std::string, std::string> > &route, long long &visited_num)
{
    std::cout << "\"route.txt\"に出力します" << std::endl;
    FILE *fp_output;
    std::ofstream ofs;
    ofs.open("route.txt"); //書き込み用のファイルを開く
    if (!ofs)
    {
        //エラー処理
        std::cerr << "Error: 出力用ファイルが開けませんでした" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "出力中..." << std::endl;
    ofs << "start: " << start.second << " (" << start.first << ")" << std::endl;
    ofs << "goal: " << goal.second << " (" << goal.first << ")" << std::endl;

    std::pair<std::string, std::string> now;
    now = goal;                                               // ゴールから探し始める
    std::stack<std::pair<std::string, std::string> > route_st; // ルートを辿るstackを作る
    route_st.push(goal);                                      // ゴールに近いものからstackに入れていく
    while (now != start)
    {
        now = route[now.first]; //探索中にどこからnowへ来たか
        route_st.push(now);     // 一つ前に探索した場所へ戻る
    }

    ofs << visited_num << " pages visited and via " << route_st.size() - 1 << " pages. (BFS)" << std::endl; //訪ねたページ数を表示
    //経路の出力開始
    ofs << "___[route]____________" << std::endl;
    ofs << start.second << " (" << start.first << ")" << std::endl;
    route_st.pop(); // stackのtopにはfirstが入っていたため
    while (route_st.size() > 0)
    {
        // stackを上(スタートに近い場所)から取り出す
        now = route_st.top();
        route_st.pop();
        ofs << "-> " << now.second << " (" << now.first << ")" << std::endl;
    }
    ofs << "______________________" << std::endl;
    std::cout << "出力が完了しました。" << std::endl;
    ofs.close();
}

int main()
{
    /* データの読み込み */
    std::map<std::string, std::string> pages;
    std::map<std::string, std::set<std::string> > links;

    {
        // pagesを読み込む
        //処理が長くなることがあるのでメッセージを出しておく
        std::cout << "Now: reading pages... (Please wait a moment)" << std::endl;

        std::ifstream file(".gitignore/data/pages.txt");
        // std::ifstream file(".gitignore/data/pages_small.txt"); //小さいテストケース
        std::string data;
        while (std::getline(file, data))
        {                                                                     // 1行読み込む
            auto index = data.find('\t');                                     // tabが何要素目かを探す
            auto id = data.substr(0, index);                                  //先頭からindexの要素分取得
            auto title = data.substr(index + 1, data.size() - id.size() - 1); // tabの次から最後まで取得
            pages[id] = title;                                                //{id : 'title'}で格納
        }
        std::cout << "Completed: reading pages\n"
                  << std::endl;
    }

    {
        // linksを読み込む
        std::cout << "Now: reading links... (Please wait a moment)" << std::endl;
        std::ifstream file(".gitignore/data/links.txt");
        // std::ifstream file(".gitignore/data/links_small.txt"); //小さいテストケース
        std::string data;
        while (std::getline(file, data))
        {
            auto index = data.find('\t');
            auto from = data.substr(0, index);
            auto to = data.substr(index + 1, data.size() - from.size() - 1);
            links[from].insert(to); // links[from]の配列にtoを挿入
            // linksはsetをそれぞれ要素に持つmap
        }
        std::cout << "Completed: reading links\n"
                  << std::endl;
    }

    /* 開始地点を見つける */
    std::string start_str = "Google"; //開始地点の名前
    std::cout << "Please input start (example:Google)\n>> ";
    std::cin >> start_str;
    std::pair<std::string, std::string> start = std::make_pair("-1", start_str); //開始地点を設定

    for (const auto &page : pages)
    {
        // pagesの情報を一つずつ取り出しpageに格納
        if (page.second == start_str) // page = (id, "Google")
        {
            start = page;
            break;
        }
    }
    if (start.first == "-1")
    {
        //開始地点が見つからなかった場合
        std::cout << "Not found: " << start.second << std::endl;
        exit(EXIT_FAILURE);
    }
    //    std::cout << start.second << " " << start.first << std::endl; //開始地点の出力

    /* ターゲットのデータを作成 */
    std::string goal_str = "渋谷"; //ターゲットの名前
    std::cout << "Please input target (example:渋谷)\n>> ";
    std::cin >> goal_str;
    std::pair<std::string, std::string> goal = std::make_pair("-1", goal_str); //ゴール地点を設定
    bool goal_is_exist = false;
    for (const auto &page : pages){
        // pagesの情報を一つずつ取り出しpageに格納
        if (page.second == goal_str){
            goal_is_exist = true;
            break;
        }
    }
    if(!goal_is_exist){
        //開始地点が見つからなかった場合
        std::cout << "Not found: " << goal.second << std::endl;
        exit(EXIT_FAILURE);
    }

    /*探索開始*/
    std::cout << "Now exploring..." << std::endl;
    long long visit_counter = 0;                                      //訪れたページ数
    std::map<std::string, std::pair<std::string, std::string> > route; // どこから来たかを保存するmap linkがx->yのとき、route[y]=x
    std::map<std::string, long long> checked;                         // 訪れた回数を記録するmap
    checked[start.first] = 1;

    std::queue<std::pair<std::string, std::string> > que; //キューを宣言
    que.push(start);                                     // 開始地点をプッシュ
    while (que.size() != 0)
    {
        visit_counter++;        //カウントしておく
        auto now = que.front(); // queの先頭のペアをnowに取り出す
        que.pop();              // queから削除
        if (now.second == goal.second)
        {
            //見つかった時
            goal = now;
            break;
        }
        checked[now.first] += 1; // 探索済みにする

        for (std::string next_id : links[now.first])
        {
            // links[now.first]はnow.firstから辿れるリンクのset
            // links[now.first]の中身を一つずつ取り出してnext_idに格納しループ
            if (checked[next_id] == 0)
            {
                // next_idを未探索の場合
                route[next_id] = now; // now.firstからnext_idに飛ぶので
                // next_idに対応するページを探しペアを作る
                std::pair<std::string, std::string> next = std::make_pair(next_id, pages[next_id]);
                que.push(next); // queに格納
            }
        }
    }

    int show = 0;
    /* 結果の出力 */
    if (goal.first != "-1")
    {
        //見つかった場合
        std::cout << "Found: " << goal.second << "(" << goal.first << ")" << std::endl;
        std::cout << "(" << visit_counter << " pages visited)" << std::endl; //訪ねたページ数を表示

        //経路が長くなる可能性もあるので選べるようにしておく
        std::cout << "\nShow route? ([0]:No [1]:Yes) \n>> ";
        std::cin >> show;
        if (show)
            show_route(start, goal, route, visit_counter);
    }
    else
    {
        //見つからなかった場合
        std::cout << "Not found: " << goal.second << std::endl;
        std::cout << "(" << visit_counter << " pages visited)" << std::endl; //訪ねたページ数を表示
    }

    /* 訪れた回数 */
    //長くなる可能性もあるので選べるようにしておく
    std::cout << "\nShow ranking? ([0]:No [1]:Yes) \n>> ";
    std::cin >> show;
    if (show)
        show_ranking(start, goal, pages, checked);

    return 0;
}
