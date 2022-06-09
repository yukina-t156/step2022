#pragma once

namespace output
{
    //フォーマットして出力
    // Usage: output::print_tour(tour, id, name<optional>); //tourが書き込まれる
    void print_tour(std::vector<int> &tour,int id, std::string name = "output"){
        std::string dstfile = "results/" + name + "_" + std::to_string(id) + ".csv";
        std::cout << "Write result in " + dstfile << std::endl;

        std::ofstream ofs(dstfile);
        if (!ofs)
        {
            std::cerr << "error: open " << dstfile << std::endl;
            std::exit(1);
        }
        ofs << "index" << std::endl;
        for(int i = 0;i<tour.size();i++){
            ofs << tour[i] << std::endl;
        }

        std::cout << "Finished." << std::endl;
    }
}
