#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include "input.h"
#include "output.h"

int main()
{
    int id;
    std::cout << "Please input challenge number:";
    std::cin >> id;
    
    std::vector<std::pair<double, double> > cities;
    input::read_csv(cities, 0);
    std::cout << "cities.size:" << cities.size() << std::endl;
    std::cout << std::fixed << std::setprecision(15) << std::endl;
    for (int i = 0; i < cities.size(); i++)
    {
        std::cout << cities[i].first << " " << cities[i].second << std::endl;
    }
    std::vector<int> tour;
    for (int i = 0; i < cities.size(); i++)
    {
        tour.push_back(i);
    }
    output::print_tour(tour,0);

    return 0;
}