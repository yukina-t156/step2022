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
    input::read_csv(cities, id);

    std::vector<int> tour;
    for (int i = 0; i < cities.size(); i++)
    {
        tour.push_back(i);
    }
    output::print_tour(tour,id);

    return 0;
}