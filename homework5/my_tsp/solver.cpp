#include <iostream>
#include <vector>
#include <fstream>
#include <string.h>
#include <cmath>
#include "input.h"
#include "output.h"

double distance(std::pair<double, double> city1, std::pair<double, double>city2){
    return std::sqrt((city1.first - city2.first) *(city1.first - city2.first) + (city1.second - city2.second) * (city1.second - city2.second));
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
    tour[0]=0;




    output::print_tour(tour,id);

    return 0;
}