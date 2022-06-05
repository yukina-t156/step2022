#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <cmath>
#include <map>
#include "input.h"
#include "output.h"

double distance(std::pair<double, double> city1, std::pair<double, double> city2)
{
    return std::sqrt((city1.first - city2.first) * (city1.first - city2.first) + (city1.second - city2.second) * (city1.second - city2.second));
}

int main()
{
    int id;
    std::cout << "Please input challenge number:";
    std::cin >> id;

    std::vector<std::pair<double, double> > cities;
    input::read_csv(cities, id);
    std::vector<int> tour;
    int len = cities.size();

    /* Write your code!*/
    std::map<int, bool> visited;
    tour.push_back(0);
    visited[0] = true;
    int now = 0;
    while (tour.size() < len)
    {
        int next = 0;
        double min_dist = 1 << 16;
        for (int i = 1; i < len; i++)
        {
            if (!visited[i])
            {
                double tmp_dist = distance(cities[now], cities[i]);
                if (tmp_dist < min_dist)
                {
                    min_dist = tmp_dist;
                    next = i;
                }
            }
        }
        visited[next] = true;
        tour.push_back(next);
        now = next;
    }

    output::print_tour(tour, id, "greedy");

    return 0;
}