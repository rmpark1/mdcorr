#pragma once

namespace parse {

class LammpsReader {
  public:

    std::vector<std::string> paths;
    int **ids;
    int **time_steps;
    double **positions
    **positions;
    **velocities;

    LammpsReader(std::string, std::string);
    ~LammpsReader();

    read(**double);
};

}
