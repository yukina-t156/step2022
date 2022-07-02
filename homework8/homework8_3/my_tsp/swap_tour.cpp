#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <cmath>
#include <map>
#include <random>
#include "input.h"
#include "output.h"

double distance(std::pair<double, double> city1, std::pair<double, double> city2)
{
    return std::sqrt((city1.first - city2.first) * (city1.first - city2.first) + (city1.second - city2.second) * (city1.second - city2.second));
}
double sum_dist(std::vector<int> &tour, std::vector<std::pair<double, double>> cities)
{
    double ret = 0.0;
    ret += distance(cities[tour[0]], cities[tour[tour.size() - 1]]);
    for (int i = 0; i < tour.size() - 1; i++)
    {
        ret += distance(cities[tour[i]], cities[tour[i + 1]]);
    }
    return ret;
}

int main()
{
    std::random_device rnd;
    int id;
    std::cout << "Please input challenge number:";
    std::cin >> id;

    std::vector<std::pair<double, double>> cities;
    input::read_csv(cities, id);
    int len = cities.size();
    std::vector<int> tour;
    input::get_tour_from_result(tour, id, "results/greedy"); //参照元の結果csvファイル
    // indexのリザルトを取得してtourに再格納する
    output::print_tour(tour, id, "swaped");
    double before_dist = sum_dist(tour, cities);
    double after_dist;
    for (int i = 0; i < 10000 * len; i++)
    {
        int city1, city2;
        
        city1 = rnd() % len;
        city2 = rnd() % len;
        
       /*
        city1 = (rnd() % (len/10))+len/10*1;//tourの前半10個
        city2 = (rnd() % (len/10))+len/10*1;
        */
        
        std::swap(tour[city1], tour[city2]);
        after_dist = sum_dist(tour, cities);
        if (before_dist - after_dist > 0) // afterの方が小さい時
        {
            std::cout << before_dist << " -> " << after_dist << std::endl;
            std::cout << "shoter! (swap: " << tour[city1] << " and " << tour[city2] << " )" << std::endl;
            output::print_tour(tour, id, "swaped");
            before_dist = after_dist;
        }
        else
        {
            std::swap(tour[city1], tour[city2]);
        }
    }

    return 0;
}