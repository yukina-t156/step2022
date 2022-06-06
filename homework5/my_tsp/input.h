#pragma once

namespace input
{
    // string"x,y"をdoubleの組(x,y)にする
    std::pair<double, double> csv_to_pair(std::string s)
    {
        //参考: https://www.delftstack.com/ja/howto/cpp/how-to-parse-string-using-delimeter-in-cpp/
        std::pair<double, double> ret;
        size_t pos = s.find(',');
        ret.first = stod(s.substr(0, pos));
        s.erase(0, pos + 1);
        ret.second = stod(s);
        return ret;
    }

    // Usage: input::read_csv(cities, id); //citiesに座標リストが作成される
    void read_csv(std::vector<std::pair<double, double>> &dst, int id)
    {
        std::string srcfile = "input_" + std::to_string(id) + ".csv";
        std::cout << "Reading " << srcfile << "..." << std::endl;
        std::ifstream ifs(srcfile);
        if (!ifs)
        {
            std::cerr << "error: open " << srcfile << std::endl;
            std::exit(1);
        }
        std::string str_buf;
        getline(ifs, str_buf); // 1行目を無視
        while (getline(ifs, str_buf))
        {
            dst.push_back(input::csv_to_pair(str_buf));
        }
        std::cout << "Finished. Cities data made." << std::endl;
    }

    // resultから取得
    void get_tour_from_result(std::vector<int> &tour, int id, std::string name = "output")
    {
        std::string srcfile = name + "_" + std::to_string(id) + ".csv";
        std::cout << "Reading " << srcfile << "..." << std::endl;
        std::ifstream ifs(srcfile);
        if (!ifs)
        {
            std::cerr << "error: open " << srcfile << std::endl;
            std::exit(1);
        }
        std::string str_buf;
        getline(ifs, str_buf); // 1行目を無視
        while (getline(ifs, str_buf))
        {
            tour.push_back(std::stoi(str_buf));
        }
        std::cout << "Finished. input tour data." << std::endl;
    }

}