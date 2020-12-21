/* MIT License

Copyright (c) 2020 Jonas Hegemann

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#include <cstdlib>
#include <cstdio>

#include "network.h"

int main(int argc, char **argv) {
  std::vector<size_t> topology = {1, 24, 24, 24, 1};
  NeuralNetwork network(topology);
  Adam adam(topology, 1.0e-3, 0.9, 0.999);

  std::vector<double> x;
  std::vector<double> y;

  size_t n = 256;
  for (size_t i = 0; i < n; i++) {
    double a = (double) i / n * 2.0 * M_PI;
    x.emplace_back(a);
    y.emplace_back(0.5 + 0.25 * sin(a));
  }

  Matrix inputs(x.size(), 1);
  Matrix outputs(y.size(), 1);

  for (size_t i = 0; i < x.size(); i++) {
    inputs(i, 0) = x[i];
    outputs(i, 0) = y[i];
  }

  network.Train(inputs, outputs, 32768, adam);

  std::fstream file;
  file.open("sin.txt", std::fstream::out);
  Matrix prediction = network.Predict(inputs);
  file << std::scientific;
  for (size_t i = 0; i < prediction.Rows(); i++) {
    file << x[i] << " " << prediction(i, 0) << std::endl;
  }
  file.close();

  return 0;
}
