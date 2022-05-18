#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <regex> //正規表現に必要

typedef struct node
{
    std::string url; //自身のURL
    node *prev;      //初期値はNULL
    node *next;      //初期値はNULL
} node;

typedef struct f_and_e
{
    node *front;
    node *end;
} f_and_e;

typedef struct cache_data
{
    std::string url;
    std::string website;
    node *node_ptr;
} hash_data;

struct node make_node(std::string S); // nodeを作る
void print_node(struct node X);       // nodeのurlと前後のurlを出力する
void print_list(struct f_and_e ptrs); // listを先頭から順にurlで出力する

//デバッグ用 front,endの内容確認
void printFE(struct f_and_e ptrs)
{
    std::cout << "_________________"<<std::endl;
    std::cout << "front:" << std::endl;
    print_node(*ptrs.front);
    std::cout << "end:" << std::endl;
    print_node(*ptrs.end);
    std::cout << "_________________"<<std::endl;
}
int main()
{
    int cache_size = 5;//キャッシュのサイズ
    struct f_and_e ptrs;
    std::vector<node> list(cache_size); //テスト用データの管理のための配列
    std::unordered_map<std::string, hash_data> Hash_table;

    //テストデータ作成
    for (int i = 0; i < cache_size; i++)
    {
        std::string name(1, 'A' + i);
        list[i] = make_node(name);
        if (i != 0)
            list[i].prev = &list[i - 1];
        if (i != 0)
            list[i - 1].next = &list[i];
        struct cache_data data;
        data.url = name + ".com";
        data.website = "page_" + name;
        data.node_ptr = &list[i];
        Hash_table[data.url] = data;
    }
    ptrs.front = &list[0];
    ptrs.end = &list[cache_size - 1];

    std::cout << "List:" << std::endl;
    print_list(ptrs);
    bool is_exist;
    struct node* X;
    struct cache_data data;
    struct f_and_e save_ptrs;

    while (1)
    {


        std::cout << "input url (xxx.com) \n (exit : input \"0\")\n>>";
        std::string input;

        std::cin >> input;
        if (input == "0")
        {
            break;
        }

        // mapで検索
        if (Hash_table.find(input) != Hash_table.end())
        {
            data = Hash_table[input];
            data.node_ptr->url = input;


            std::cout << "data " << data.url << " " << data.website << std::endl;

            print_node(*data.node_ptr);

            X = Hash_table[input].node_ptr;//ここでptrsが変わってしまっていた?


            X->url = input;
            std::cout << input << std::endl;

            print_node(*X);

            is_exist = true;
        }
        else
        {
            is_exist = false;
        }
        if (is_exist)
        {
            std::cout << input << " is exist!" << std::endl;

            // Xを削除
            if (X->prev != NULL)X->prev->next = X->next;
            else ptrs.front = X->next;
            if (X->next != NULL) X->next->prev = X->prev;
            else ptrs.end = X->prev;
            X->prev = NULL;
            X->next = NULL;

            // Xを最後尾に
            X->prev = ptrs.end;
            X->prev->next = X;
            X->next = NULL;
            ptrs.end = X;
            //ハッシュテーブルの更新はなし
        }
        if (!is_exist)
        {
            std::cout << input << " is not exist!" << std::endl;

            //ハッシュテーブルの更新
            //データの追加
            //データ例としてURLの前の部分をページ名などに使用
            //正規表現 参考(https://qiita.com/yohm/items/7c82693b83d4c055fa7b)
            std::string name = std::regex_replace(input,std::regex(".com"),"");
            std::cout << __LINE__ <<std::endl;//ok
            printFE(ptrs);
            struct node save_end = *ptrs.end;
            struct node new_node = make_node(name); //ここでptrs.endがXに勝手に変わっている?
            ptrs.end=&save_end;//endを保存しといて読み込む
            std::cout << __LINE__ <<std::endl;//ng
            printFE(ptrs);
            X = &new_node;
            std::cout << __LINE__ <<std::endl;//ng
            printFE(ptrs);

            struct cache_data data;
            data.url = input;
            data.website = "page_" + name;
            data.node_ptr = X;
            Hash_table[data.url] = data;

            //データの削除
            Hash_table.erase(ptrs.front->url);

            //先頭をdelete
            struct node *tmp1 = ptrs.front;
            ptrs.front = ptrs.front->next;
            ptrs.front->prev = NULL;
            tmp1->next=NULL;
            tmp1->prev=NULL;

            //最後尾にXをadd
            struct node tmp2 = *ptrs.end;
            //struct node* tmp2 = ptrs.end;とどっちがいいんだ？

            X->prev = &tmp2;
            X->prev->next = X;
            
            /*
            std::cout <<"X->prev:" << std::endl;//X->prev->nextがNULLになってる？
            print_node(*X->prev);
            */

            ptrs.end = X;
            X->next = NULL;
            //printFE(ptrs);
            //ここではX->prev->nextも正常に動いている
            //print_listした時にptrs.end->prevからptrs.endへのアクセスがうまくいっていないみたい


        }
        
        std::cout << "List:" << std::endl;
        print_list(ptrs);
        std::cout << std::endl;

    }

    return 0;
}
void print_node(struct node X)
{
    std::cout << "url:\t" << X.url << std::endl;
    std::cout << "prev:\t";
    if (X.prev != NULL)
        std::cout << X.prev->url << std::endl;
    else
        std::cout << "NULL" << std::endl;
    std::cout << "next:\t";
    if (X.next != NULL)
        std::cout << X.next->url << std::endl;
    else
        std::cout << "NULL" << std::endl;
    std::cout << std::endl;
}

void print_list(struct f_and_e ptrs)
{
    node now = *ptrs.front;
    std::cout << now.url;
    while (now.next != NULL)
    {
        std::cout << " -> " << now.next->url;
        now = *now.next;
    }
    std::cout << std::endl;
}

struct node make_node(std::string S)
{
    struct node X;
    X.url = S + ".com";
    X.prev = NULL;
    X.next = NULL;
    return X;
}
