#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <cmath>
#include <map>
#include "input.h"
#include "output.h"

double distance(std::pair<double, double> city1, std::pair<double, double>city2){
    return std::sqrt((city1.first - city2.first) *(city1.first - city2.first) + (city1.second - city2.second) * (city1.second - city2.second));
}

double manhattan(std::pair<double, double> city,double x,double y){
    //cityの座標と直線x,yのマンハッタン距離の合計を求める
    return (abs(city.first-x)+abs(city.second-y));
}

// initialize block tour!
int initialize(std::vector<int> &dev_cities,std::vector<int> &tour,std::vector<std::pair<double, double> > &cities){
    //failed:return -1 successed:return 0
    if(dev_cities.size()==0) return -1;
    std::map<int, bool> visited;
    int len = dev_cities.size();
    int now = 0;
    visited[dev_cities[now]]=true;
    tour.push_back(dev_cities[now]);
    while(tour.size() < len){
        int next = 0;
        double min_dist = 1<<16;
        for(int i=1;i<len;i++){
            if(!visited[dev_cities[i]]){
                double tmp_dist = distance(cities[dev_cities[now]],cities[dev_cities[i]]);
                if(tmp_dist < min_dist){
                    min_dist = tmp_dist;
                    next = i;
                }
            }
        }
        visited[dev_cities[next]]=true;
        tour.push_back(dev_cities[next]);
        now=next;
    }
    return 0;
}


    // 左上i,jを指定して[i][j],[i+1][j],[i][j+1],[i+1][j+1]を統合したtourを返す関数
    // dev_citiesの中身は加えるだけ
std::vector<int> merged_tour(int y,int x,
    std::vector<std::vector<std::vector<int> > > &dev_cities,
    std::vector<std::vector<std::vector<int> > > &tour,
    std::vector<std::pair<double, double> > &cities,std::pair<double, double> border){
    //左上のブロックの番号i,j、main関数内の外側のfor文で毎回更新されるdev_cities、tour、4ブロックの境界になるborder(x,y) 
    // border.first = yの境界

    std::vector<int> ret;
    std::vector<std::vector<int> > num(2,std::vector<int>(2));
    int sum = 0;
        for(int i=0;i<2;i++){
            for(int j=0;j<2;j++){
                num[i][j]=dev_cities[y+i][x+j].size();
            sum ++;
        }
    }
    if(!sum){return ret;} //0個の時
    if(sum<=3){
        //3個以下の時、順番は気にせずそのまま入れて返して良い
        for(int i=0;i<2;i++){
            for(int j=0;j<2;j++){
                for(int k=0;k<num[i+j];k++){
                    ret.push_back(dev_cities[y+i][x+j][k]);
                }
            }
        }
        return ret;
    }
    
    double min_manh = 1<<16,tmp_manh;
    int now_min;
    std::vector<std::vector<std::pair<int, int> > >  edge(2,std::vector<std::pair<int, int> >(2));
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            tmp_manh = 1<<16;
            if(num[i][j]==0){edge[i][j].first=-1;edge[i][j].second=-1;}//この時の処理どうしよう
            else if(num[i][j]==1){edge[i][j].first=tour[y+i][x+j][0];edge[i][j].second=tour[y+i][x+j][0];}
            else if(num[i][j]==2){edge[i][j].first=tour[y+i][x+j][0];}
            if(num[i][j]>2){
                for(int k=0;k<num[i][j];k++){
                    /* マンハッタン */
                    tmp_manh = manhattan(cities[tour[y+i][x+j][k]],border.first, border.second);
                    if(tmp_manh<=min_manh){
                        min_manh = tmp_manh;
                        now_min=k;
                        //tour[y+i][x+j][k]がマンハッタン距離最小に
                    }
                }
            }
        }
    }
    




}

int main()
{
    int id;
    std::cout << "Please input challenge number:";
    std::cin >> id;
    
    std::vector<std::pair<double, double> > cities;
    input::read_csv(cities, id);
    std::vector<int> tour(cities.size());

    /* Write your code!*/
    int N = 2; // devide 4*N
    N *= 2; // 縦横２倍 ex:N=1のとき横2縦2の2*2ブロック
    float min_x=1<<16,max_x= -1,min_y=1<<16,max_y=-1;
    for(int i = 0;i<cities.size();i++){
        if(cities[i].first>=max_x)max_x=cities[i].first;
        if(cities[i].first<=min_x)min_x=cities[i].first;
        if(cities[i].second>=max_x)max_y=cities[i].second;
        if(cities[i].second<=max_x)min_y=cities[i].second;
    }
    float width = max_x-min_x,height = max_y-min_y;
    float block_width = width/N,block_height=height/N;
    std::vector<std::vector<std::vector<int> > > devided_cities(N+1,std::vector(N+1, std::vector<int>(0)));
    for(int i = 0;i<cities.size();i++){
        devided_cities[(cities[i].second-min_y)/block_height][(cities[i].first-min_x)/block_width].push_back(i);
    }
    std::cout << "devided cities!" << std::endl;
    for(int i=0;i<N+1;i++){
        for(int j=0;j<N+1;j++){
            std::cout << "i:" << i << " j:" << j << std::endl;
            for(int k = 0;k<devided_cities[i][j].size();k++){
                std::cout << devided_cities[i][j][k] << " ";
            }
            std::cout << std::endl << "---------------------" << std::endl;
        }
    }

    /* 初期ブロック内部での貪欲な解 */
    std::vector<std::vector<std::vector<int> > > devided_tour(N+1,std::vector(N+1, std::vector<int>(0)));
    std::map<int, bool> visited;
    for(int i=0;i<N+1;i++){
        for(int j=0;j<N+1;j++){
            // 所属する頂点idの集合 -> devided_cities[i][j]
            /* initialize (degree) */
            initialize(devided_cities[i][j],devided_tour[i][j],cities);
        }
    }
    

    std::cout << "devided tour!" << std::endl;
    for(int i=0;i<N+1;i++){
        for(int j=0;j<N+1;j++){
            std::cout << "i:" << i << " j:" << j << std::endl;
            for(int k = 0;k<devided_tour[i][j].size();k++){
                std::cout << devided_tour[i][j][k] << " ";
            }
            std::cout << std::endl << "---------------------" << std::endl;
        }
    }

    //一回で縦横1/2になる 1個になるまでやる! -> 全ブロックが各N回ずつ統合される -> 2重forぶん
    // 左上i,jを指定して[i][j],[i+1][j],[i][j+1],[i+1][j+1]を統合したtourを返す関数
    //配列の数が1/4になったdevided_citiesとdevided_tourが毎回必要? -> N回再宣言?
    std::vector<std::pair<double, double> > border;
    /*
    border.first = max_y - (i * block_height);
    border.second = max_x - (j * block_width);
    */
    
    output::print_tour(tour,id,"devided");

    return 0;
}