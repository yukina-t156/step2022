#pragma once

namespace output
{
    //フォーマットして出力
    // Usage: output::print_tour(tour, id); //tourが書き込まれる
    void print_tour(std::vector<int> &tour,int id){
        std::string result_name = "sample";
        std::string dstfile = "results/" + result_name + "_" + std::to_string(id) + ".csv";
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
